#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> /* For O_* constants */
#include <semaphore.h>
#include <stdlib.h>

struct grade_t{
    int value;
    char note[6];
    sem_t semaphore;
};

int main(){
    int shm_fd;
    struct grade_t *vaddr;
    
    /* get shared memory handle */
    if ((shm_fd = shm_open("my_shm", O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }
    printf("opened\n");
    
    /* set the shared memory size to SHM_SIZE */
    if (ftruncate(shm_fd, sizeof(grade_t)) != 0){
        perror("cannot set size");
        return -1;
    }
    printf("truncated\n");
    
    /* Map shared memory in address space. MAP_SHARED flag tells that this is a
     * shared mapping */
    if ((vaddr = (struct grade_t *)mmap(0, sizeof(grade_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("cannot mmap");
        return -1;
    }
    printf("mapped\n");
    
    /* lock the shared memory */
    if (mlock(vaddr, sizeof(grade_t)) != 0){
        perror("cannot mlock");
        return -1;
    }
    printf("locked\n");
    
    /* Shared memory is ready for use */
    printf("READY FOR USE\n");
    
    sleep(2);
    
    int i = 0;
    
    while(1){
        //sem_wait(&(vaddr->semaphore));
       vaddr->value = i;
        
       //char numberString[6];
       //ddr->note = "Hello!";
        
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
        
        
        //strcpy(vaddr->note,numberString);
        
        //sem_post(&(vaddr->semaphore));
        
        printf("%i:%s\n",vaddr->value, vaddr->note);
        
        //printf("%i\n",vaddr->value);
        
        i++;
        
        if(i == 10){
            i = 0;
        }
        
        sleep(1);
 
    }
}
