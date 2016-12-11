#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <signal.h>
#include "grade_t.h"

const char* SHM_NAME = "grade";

struct grade_t *vaddr;
int shm_fd;

void on_interrupt(int a){
    // Clean up the shared memory
    munmap(vaddr, sizeof(grade_t));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    
    // Semaphore handled by A.
    exit(0);
}


int main(){
    // Get shared memory file descriptor.
    if ((shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }
    
    // Set the shared memory size to the size of grade_t struct.
    if (ftruncate(shm_fd, sizeof(grade_t)) != 0){
        perror("cannot set size");
        return -1;
    }
    
    // Map shared memory in address space.
    if ((vaddr = (struct grade_t *) mmap(0, sizeof(grade_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("cannot mmap");
        return -1;
    }
    
    // Lock the shared memory.
    if (mlock(vaddr, sizeof(grade_t)) != 0){
        perror("cannot mlock");
        return -1;
    }
    
    // Shared memory is ready for use.
    printf("Shared Memory successfully opened.\n");
    
    
    while(1){
		sem_wait(&(vaddr->semaphore));
		printf("%i=%s\n", vaddr->value, vaddr->note);	
		sem_post(&(vaddr->semaphore));
		usleep(200000);
	}
}
