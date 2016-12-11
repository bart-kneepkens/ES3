#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <signal.h>
#include "grade_t.h"
#include "grades_t.h"

const char* SHM_NAME = "grades";

struct grades_t *vaddr;
int shm_fd;

// Add a grade_t struct to the end of the array in vaddr.
// Pushes the other items forward (towards 0) by one.
void addToBuffer(struct grade_t grade){
    for (int i = 0; i < 9; i++){
        vaddr->grades[i] = vaddr->grades[i+1];
    }
    vaddr->grades[9] = grade;
}

void on_interrupt(int a){
    // Clean up the shared memory
    munmap(vaddr, sizeof(grades_t));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    
    // Clean up the semaphores
    sem_close(&(vaddr->filledCount));
    sem_close(&(vaddr->emptyCount));
    
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
    if ((vaddr = (struct grades_t *)mmap(0, sizeof(grades_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
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
    
    // Initialize semaphore 'filledCount' with value 0.
    if(sem_init(&(vaddr->filledCount), 1, 0) != 0){
        perror("Can not init semaphore filledCount");
        return -1;
    }
    
    // Initialize semaphore 'emptyCount' with value 10.
    if(sem_init(&(vaddr->emptyCount), 1, 10) != 0){
        perror("Can not init semaphore emptyCount");
        return -1;
    }
    
    // Semaphores are ready for use.
    printf("Semaphores successfully Initialized with value 0 and 10.\n");
    
    // Call 'on_interrupt' when CTRL + C is pressed
    signal(SIGINT, on_interrupt);
    
    
    int i = 0;
    struct grade_t currentGrade;
    
    while(1){
        currentGrade.value = i;
        switch (i) {
            case 0:
                strcpy(currentGrade.note, "zero");
                break;
            case 1:
                strcpy(currentGrade.note, "one");
                break;
            case 2:
                strcpy(currentGrade.note, "two");
                break;
            case 3:
                strcpy(currentGrade.note, "three");
                break;
            case 4:
                strcpy(currentGrade.note, "four");
                break;
            case 5:
                strcpy(currentGrade.note, "five");
                break;
            case 6:
                strcpy(currentGrade.note, "six");
                break;
            case 7:
                strcpy(currentGrade.note, "seven");
                break;
            case 8:
                strcpy(currentGrade.note, "eight");
                break;
            case 9:
                strcpy(currentGrade.note, "nine");
                break;
        }
        
        sem_wait(&(vaddr->emptyCount));
        
        addToBuffer(currentGrade);
        
        sem_post(&(vaddr->filledCount));
        
        i++;
        
        if(i == 10){
            i = 0;
        }
    }
}
