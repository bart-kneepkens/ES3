#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>	/* For O_* constants */

const int SHM_SIZE = 1024;

int main(){
    int shm_fd;
    void *vaddr;

    /* get shared memory handle */
    if ((shm_fd = shm_open("my_shm", O_CREAT | O_RDWR, 0666)) == -1){
        perror("cannot open");
        return -1;
    }
    printf("opened\n");

    /* set the shared memory size to SHM_SIZE */
    if (ftruncate(shm_fd, SHM_SIZE) != 0){
        perror("cannot set size");
        return -1;
    }
    printf("truncated\n");

    /* Map shared memory in address space. MAP_SHARED flag tells that this is a
    * shared mapping */
    if ((vaddr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        perror("cannot mmap");
        return -1;
    }
    printf("mapped\n");

    /* lock the shared memory */
    if (mlock(vaddr, SHM_SIZE) != 0){
        perror("cannot mlock");
        return -1;
    }
    printf("locked\n");

    /* Shared memory is ready for use */
    printf("READY FOR USE\n");

    /* unmap from address space */
    munmap(vaddr, SHM_SIZE);
    printf("unmapped\n");
    close(shm_fd);
    printf("closed\n");

    /* remove shared memory segment. Last call to shm_unlink will delete the shared memory */
    shm_unlink("my_shm");
    printf("unlinked\n");

    return 0;
}