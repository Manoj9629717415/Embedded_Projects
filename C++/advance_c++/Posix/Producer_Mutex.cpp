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
    int fd = shm_open(shm_name,O_CREAT | O_EXCL | O_RDWR,0666);

    bool creator = false;
    if(fd != -1)
    {
        std::cout<<" i am the creator of shared memeory \n";
        creator = true;
    }
    else if(errno == EEXIST)
    {
        std::cout<<"shared memory already created,so just open it \n";
        fd = shm_open(shm_name,O_RDWR,0666);
        if(fd == -1)
        {
            perror("shm_open");
            return 1;
        }
    }
    else
    {
        perror("shm_open");
        return 1;
    }
    if(creator == true)
    {
        if(ftruncate(fd,sizeof(SharedMemory_mutex)) == -1)
        {
            perror("allocation faled");
            close(fd);
            return 1;
        }
    }

    SharedMemory_mutex* data =static_cast<SharedMemory_mutex*>(mmap(nullptr,sizeof(SharedMemory_mutex),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0));

    if(data == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    if(creator)
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
        pthread_mutexattr_setrobust(&attr,PTHREAD_MUTEX_ROBUST);

        pthread_mutex_init(&data->mutex,&attr);

        pthread_mutexattr_destroy(&attr);

        pthread_condattr_t c_attr;
        pthread_condattr_init(&c_attr);
        pthread_condattr_setpshared(&c_attr,PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&data->not_empty,&c_attr);
        pthread_cond_init(&data->not_full,&c_attr);
        pthread_condattr_destroy(&c_attr);
        pthread_mutex_lock(&data->mutex);
        
        data->read_index = 0;
        data->write_index = 0;
        data->count = 0;
        pthread_mutex_unlock(&data->mutex);

    }

    


    int count = 0;
    while(count < 10)
    {
        int ret = pthread_mutex_lock(&data->mutex);

        if(ret == EOWNERDEAD)
        {
            std::cerr<<" owner crashed while holding mutex, needs to be recivered "<<std::strerror(ret)<<"\n";
            pthread_mutex_consistent(&data->mutex);
        }
        else if(ret != 0)
        {
            std::cerr<<" mutex locks fails "<<std::strerror(ret)<<"\n";
            munmap(data,sizeof(SharedMemory_mutex));
            close(fd);
            return 1;
        }

        while(data->count == BUFFER_SIZE && !data->shutdown)
        {
             int c_ret = pthread_cond_wait(&data->not_full,&data->mutex);

             if(c_ret != 0)
            {
                std::cerr<<" mutex locks fails "<<std::strerror(c_ret)<<"\n";
                pthread_mutex_unlock(&data->mutex);
                munmap(data,sizeof(SharedMemory_mutex));
                close(fd);
                return 1;
            }
        }

        if(data->shutdown == true)
        {
            pthread_mutex_unlock(&data->mutex);
            break;
        }

        data->update_inprogress = true;
        data->prev_write_index = data->write_index;
        data->prev_count = data->count;
        
        data->buffer[data->write_index].rpm = (count+1) * 1000 ;
        sleep(10);

        data->buffer[data->write_index].speed = (count+1) * 10 ;

        std::cout<<" data filled on index "<<data->write_index<<"\n";

        data->write_index = (data->write_index + 1) % 4;
        data->count++;
        data->update_inprogress = false;

        std::cout<<" data count  "<<data->count<<"\n";



        pthread_cond_signal(&data->not_empty);
        pthread_mutex_unlock(&data->mutex);

        count++;
    }

    if(creator)
    {
        pthread_mutex_lock(&data->mutex);
        data->shutdown = true;
        pthread_cond_broadcast(&data->not_empty);
        pthread_cond_broadcast(&data->not_full);
        pthread_mutex_unlock(&data->mutex);
    }


    munmap(data,sizeof(SharedMemory_mutex));
    close(fd);
    return 0;
}