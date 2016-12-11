#include <semaphore.h>

struct grades_t{
    struct grade_t grades[10];
    sem_t filledCount;
    sem_t emptyCount;
};
