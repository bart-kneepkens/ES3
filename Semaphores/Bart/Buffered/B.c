#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <signal.h>
#include "grade_t.h"
#include "grades_t.h"

const char* SHM_NAME = "grades";

struct grades_t *vaddr;
int shm_fd;

// Takes the next up item from the buffer.
// Pushes the other items forward (towards 0) by 1.
struct grade_t takeNextFromBuffer(){
    struct grade_t last = vaddr->grades[0];
    
    for (int i = 0; i < 9; i++){
        vaddr->grades[i] = vaddr->grades[i+1];
    }
    
    return last;
}

void on_interrupt(int a){
    // Clean up the shared memory
    munmap(vaddr, sizeof(grades_t));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    
    // Semaphores handled by A.
    exit(0);
}

int main(){
    // Get shared memory file descriptor.
    if ((shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }
    
    // Set the shared memory size to the size of grades_t struct.
    if (ftruncate(shm_fd, sizeof(grades_t)) != 0){
        perror("cannot set size");
        return -1;
    }
    
    // Map shared memory in address space.
    if ((vaddr = (struct grades_t *) mmap(0, sizeof(grades_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("cannot mmap");
        return -1;
    }
    
    // Lock the shared memory.
    if (mlock(vaddr, sizeof(grades_t)) != 0){
        perror("cannot mlock");
        return -1;
    }
    
    // Shared memory is ready for use.
    printf("Shared Memory successfully opened.\n");
    
    // Call 'on_interrupt' when CTRL + C is pressed
    signal(SIGINT, on_interrupt);
    
    
    while(1){
        sem_wait(&(vaddr->filledCount));
        
        struct grade_t item = takeNextFromBuffer();
        
        sem_post(&(vaddr->emptyCount));
        
        printf("%i=%s\n", item.value, item.note);	
    }
}
