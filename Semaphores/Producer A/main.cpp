#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>	/* For O_* constants */

const int SHM_SIZE = 1024;

struct cijfer_t {
    int waarde;
    char *uitspraak;
};

int main(){
    int shm_fd;
    cijfer_t *vaddr;

    /* get shared memory handle */
    if ((shm_fd = shm_open("my_shm", O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }

    /* set the shared memory size to SHM_SIZE */
    if (ftruncate(shm_fd, SHM_SIZE) != 0){
        perror("cannot set size");
        return -1;
    }

    /* Map shared memory in address space. MAP_SHARED flag tells that this is a
    * shared mapping */
    if ((vaddr = (cijfer_t*) mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("cannot mmap");
        return -1;
    }

    int loop = 0;
    while (1) {
        vaddr->waarde = loop;
        if (++loop > 9) {
            loop = 0;
        }
    }
}