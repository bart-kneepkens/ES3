#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h> /* For O_* constants */
#include <semaphore.h>
#include <stdlib.h>

struct grades_t *vaddr;

struct grade_t{
    int value;
    char note[6];
};

struct grades_t{
	struct grade_t grades[10];
	sem_t filledCount;
	sem_t emptyCount;
};

struct grade_t removeLastFromBuffer(){
	struct grade_t last = vaddr->grades[0];
	
	for (int i = 0; i < 9; i++){
		vaddr->grades[i] = vaddr->grades[i+1];
	}
	
	return last;
}

int main(){
    int shm_fd;
    
    /* get shared memory handle */
    if ((shm_fd = shm_open("my_shm", O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }
    
    /* set the shared memory size to SHM_SIZE */
    if (ftruncate(shm_fd, sizeof(grades_t)) != 0){
        perror("cannot set size");
        return -1;
    }
    
    /* Map shared memory in address space. MAP_SHARED flag tells that this is a
     * shared mapping */
    if ((vaddr = (struct grades_t *) mmap(0, sizeof(grades_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("cannot mmap");
        return -1;
    }
    
    /* lock the shared memory */
    if (mlock(vaddr, sizeof(grades_t)) != 0){
        perror("cannot mlock");
        return -1;
    }
    
    /* Shared memory is ready for use */
    printf("Shared Memory successfully opened.\n");
    
    while(1){
		//sem_wait(&(vaddr->semaphore));
		//printf("%i=%s\n", vaddr->grades[0].value, vaddr->grades[0].note);	
		//printf("%i=%s\n", vaddr->value, vaddr->note);	
		//sem_post(&(vaddr->semaphore));
		//sleep(1);
		
		sem_wait(&(vaddr->filledCount));
		
		struct grade_t item = removeLastFromBuffer();
		
		sem_post(&(vaddr->emptyCount));
		
		
		printf("%i=%s\n", item.value, item.note);	
	}
}
