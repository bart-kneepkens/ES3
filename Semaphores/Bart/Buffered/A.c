#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
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

void addToBuffer(struct grade_t grade){
	for (int i = 0; i < 9; i++){
		vaddr->grades[i] = vaddr->grades[i+1];
	}
	
	vaddr->grades[9] = grade;
}

void PrintBuffer(){
	printf("{");
	
	for (int i = 0; i < 10; i++){
		printf("%i,", vaddr->grades[i].value);
	}
	
	printf("}\n");
}

int main(){
    int shm_fd;
    /* get shared memory handle */
    if ((shm_fd = shm_open("my_shm", O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }
    
    /* set the shared memory size to the size of grade_t struct */
    if (ftruncate(shm_fd, sizeof(grades_t)) != 0){
        perror("cannot set size");
        return -1;
    }
    
    /* Map shared memory in address space. MAP_SHARED flag tells that this is a
     * shared mapping */
    if ((vaddr = (struct grades_t *)mmap(0, sizeof(grades_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
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
    
    if(sem_init(&(vaddr->filledCount), 1, 0) != 0){
		perror("Can not init semaphore filledCount");
		return -1;
	}
	
	if(sem_init(&(vaddr->emptyCount), 1, 10) != 0){
		perror("Can not init semaphore emptyCount");
		return -1;
	}
	
	printf("Semaphores successfully Initialized with value 0 and 10.\n");
    
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
