#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

#include "Data.h"




int main()
{
    sem_t* sem_pro = sem_open(sem_name,O_CREAT,0666,0);

    if(sem_pro == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }

    std::cout<<"sem opened in producer \n";

    sleep(2);

    sem_post(sem_pro);

    std::cout << "Producer: done\n";

    sem_close(sem_pro);


}