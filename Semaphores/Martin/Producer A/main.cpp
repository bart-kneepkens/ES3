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
    if ((vaddr = (cijfer_t*) mmap(0, sizeof(cijfer_t), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("cannot mmap");
        return -1;
    }
    //mlock(vaddr, sizeof(cijfer_t));

    //vaddr = new cijfer_t();

    //munmap(vaddr, sizeof(cijfer_t));
    //close(shm_fd);
    //shm_unlink("my_shm");

    unsigned int loop = 0;
    while (1) {
        //sem_wait (sem);

        if (++loop > 9) {
            loop = 0;
        }
        vaddr->waarde = loop;

        std::string uitspraak;
        switch (vaddr->waarde) {
            case 0:
                uitspraak = "zero";
                break;
            case 1:
                uitspraak = "one";
                break;
            case 2:
                uitspraak = "two";
                break;
            case 3:
                uitspraak = "three";
                break;
            case 4:
                uitspraak = "four";
                break;
            case 5:
                uitspraak = "five";
                break;
            case 6:
                uitspraak = "six";
                break;
            case 7:
                uitspraak = "seven";
                break;
            case 8:
                uitspraak = "eight";
                break;
            case 9:
                uitspraak = "nine";
                break;
        }
        char * cstr = new char [uitspraak.length() + 1];
        std::strcpy(cstr, uitspraak.c_str());
        vaddr->uitspraak = cstr;

        std::cout << vaddr->waarde << " = " << vaddr->uitspraak << std::endl;

        //sem_post (sem);
        usleep(1000000); // Sleep for 1 second.
    }
}