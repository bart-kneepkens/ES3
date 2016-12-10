#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string>
#include <cstring>
#include <iostream>

const int SHM_SIZE = 1024;

struct cijfer_t {
    int waarde;
    char *uitspraak;
};

int main(){
    int shm_fd;
    cijfer_t *vaddr;
    sem_t *sem;

    // get shared memory handle
    if ((shm_fd = shm_open("my_shm", O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }

    // initialize semaphore
    if ((sem = sem_open("pSem", O_CREAT, 0600, 1)) == SEM_FAILED) {
        perror("failed to initialize semaphore");
        return -1;
    }
    sem_unlink("pSem");

    // set the shared memory size to SHM_SIZE
    if (ftruncate(shm_fd, sizeof(cijfer_t)) != 0){
        perror("cannot set size");
        return -1;
    }

    // Map shared memory in address space. MAP_SHARED flag tells that this is a shared mapping
    if ((vaddr = (cijfer_t*) mmap(0, sizeof(cijfer_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == NULL){
        perror("cannot mmap");
        return -1;
    }

    //mlock(vaddr, sizeof(cijfer_t));

    //vaddr = new cijfer_t();

    //munmap(vaddr, sizeof(cijfer_t));
    //close(shm_fd);
    //shm_unlink("my_shm");

    while (1) {
        //sem_wait (sem);
        //std::cout << vaddr->waarde << " = " << vaddr->uitspraak << std::endl;
        std::cout << "cocks" << std::endl;
        //sem_post (sem);
        usleep(1000000); // Sleep for 1 second.
    }
}