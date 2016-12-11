#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
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
    
    /* set the shared memory size to SHM_SIZE */
    if (ftruncate(shm_fd, sizeof(grade_t)) != 0){
        perror("cannot set size");
        return -1;
    }
    
    /* Map shared memory in address space. MAP_SHARED flag tells that this is a
     * shared mapping */
    if ((vaddr = (struct grade_t *) mmap(0, sizeof(grade_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("cannot mmap");
        return -1;
    }
    
    /* lock the shared memory */
    if (mlock(vaddr, sizeof(grade_t)) != 0){
        perror("cannot mlock");
        return -1;
    }
    
    /* Shared memory is ready for use */
    printf("Shared Memory successfully opened.\n");
    
    while(1){
		sem_wait(&(vaddr->semaphore));
		printf("%i=%s\n", vaddr->value, vaddr->note);	
		sem_post(&(vaddr->semaphore));
		usleep(200000);
	}
}
