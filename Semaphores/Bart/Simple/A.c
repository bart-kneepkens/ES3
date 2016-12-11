#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include "grade_t.h"

int main(){
    int shm_fd;
    struct grade_t *vaddr;
    
    // Get shared memory file descriptor.
    if ((shm_fd = shm_open("grade", O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }
    
    // Set the shared memory size to the size of grade_t struct.
    if (ftruncate(shm_fd, sizeof(grade_t)) != 0){
        perror("cannot set size");
        return -1;
    }
    
    // Map shared memory in address space.
    if ((vaddr = (struct grade_t *)mmap(0, sizeof(grade_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
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
    
    // Initialize semaphore 'semaphore' with value 1.
    if(sem_init(&(vaddr->semaphore), 1, 1) != 0){
        perror("cannot sem_init");
        return -1;
    }
    
    // Semaphore is ready for use.
    printf("Semaphore successfully Initialized with value 1.\n");
    
    
    int i = 0;
    
    while(1){
        sem_wait(&(vaddr->semaphore));
        vaddr->value = i;
        switch (i) {
            case 0:
                strcpy(vaddr->note, "zero");
                break;
            case 1:
                strcpy(vaddr->note, "one");
                break;
            case 2:
                strcpy(vaddr->note, "two");
                break;
            case 3:
                strcpy(vaddr->note, "three");
                break;
            case 4:
                strcpy(vaddr->note, "four");
                break;
            case 5:
                strcpy(vaddr->note, "five");
                break;
            case 6:
                strcpy(vaddr->note, "six");
                break;
            case 7:
                strcpy(vaddr->note, "seven");
                break;
            case 8:
                strcpy(vaddr->note, "eight");
                break;
            case 9:
                strcpy(vaddr->note, "nine");
                break;
        }
        
        printf("%i:%s\n",vaddr->value, vaddr->note);
        
        sem_post(&(vaddr->semaphore));
        
        i++;
        
        if(i == 10){
            i = 0;
        }
    }
}
