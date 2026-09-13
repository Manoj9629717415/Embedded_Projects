#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Data.h"

int main()
{
    sem_t* sem = sem_open(
        sem_name,
        O_CREAT,
        0666,
        0);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }

    std::cout << "Consumer: waiting...\n";

    sem_wait(sem);

    std::cout << "Consumer: received semaphore\n";

    sem_close(sem);

    return 0;
}