#include <semaphore.h>

struct grade_t{
    int value;
    char note[6];
    sem_t semaphore;
};
