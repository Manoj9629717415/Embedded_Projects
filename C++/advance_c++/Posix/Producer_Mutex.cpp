#include <iostream>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>

#include "Data.h"


int main()
{
    int fd = shm_open(shm_open,O_CREAT | O_EXCL | O_RDWR,0666);

    if(fd != -1)
    {
        if(errno == EEXIST)
        {
            std::cout<<"shared memory already created,so just open it \n";
            fd = shm_open(shm_open,O_RDWR,0666);
            if(fd == -1)
            {
                perror("shm_open");
                return 1;
            }
        }
    }
    else
    {
        perro("shm open");
        return 1;
    }

    SharedMemory_mutex* data =static_cast<SharedMemory_mutex*>(mmap(nullptr,sizeof(SharedMemory_mutex),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0));

    if(data == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_setrobust(&attr,PTHREAD_MUTEX_ROBUST);

    pthread_mutex_init(&data->mutex,&attr);

    pthread_mutexattr_destroy(&attr);

    int count = 0;
    while(count < 10)
    {
        pthread_mutex_lock(&data->mutex);
        data->buffer[data->write_index].rpm = (count+1) * 1000 ;
        data->buffer[data->write_index].speed = (count+1) * 10 ;
        data->write_index = (data->write_index + 1) % 4;
        pthread_mutex_unlock(&data->mutex);
    }





}