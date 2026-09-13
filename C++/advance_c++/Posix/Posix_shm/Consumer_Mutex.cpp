#include <iostream>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <cerrno>
#include <cstring>
#include <ctime>

#include "Data.h"


bool is_data_valid(const SharedMemory_mutex* data)
{
    if (data->count < 0 ||
        data->count > BUFFER_SIZE)
    {
        return false;
    }

    if (data->read_index < 0 ||
        data->read_index >= BUFFER_SIZE)
    {
        return false;
    }

    if (data->write_index < 0 ||
        data->write_index >= BUFFER_SIZE)
    {
        return false;
    }

    return true;
}


int main(){

    int fd = shm_open(shm_name,O_RDWR,0666);

    if(fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    SharedMemory_mutex* data = static_cast<SharedMemory_mutex*>(mmap(nullptr,sizeof(SharedMemory_mutex),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0));

    if(data == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    while(true)
    {
        timespec m_ts;
        clock_gettime(CLOCK_REALTIME,&m_ts);
        m_ts.tv_sec += 2;

        // int ret = pthread_mutex_lock(&data->mutex);
        int ret = pthread_mutex_timedlock(&data->mutex,&m_ts);
        if(ret == EOWNERDEAD)
        {
            std::cout << "Owner Dead while hoilding mutex"<< '\n';
            std::cout << "count      = " << data->count << '\n';
            std::cout << "read_index = " << data->read_index << '\n';
            std::cout << "write_index= " << data->write_index << '\n';
            if(data->update_inprogress)
            {
                std::cout << "Shared state is INVALID,incomplete transactaion\n";
                 // Discard the item being written
                data->update_inprogress = false;
                data->write_index = data->prev_write_index;
                data->count = data->prev_count;

                // IMPORTANT:
                // Do NOT increment count
                // Do NOT advance write_index

            }
            else{
                std::cout << "Shared state appears valid\n";
            }
            int c_ret = pthread_mutex_consistent(&data->mutex);
            if(c_ret != 0)
            {
                std::cerr<<"consisitent failed "<<std::strerror(c_ret)<<"\n";
                pthread_mutex_unlock(&data->mutex);
                return 1;
            }
        }
        else if(ret == ETIMEDOUT)
        {
            std::cout << "mutex not aqquired with 2 seconds"<< '\n';
            pthread_mutex_unlock(&data->mutex);
            continue; // just for experimwent puepose i m doing re-try
        }
        else if(ret != 0)
        {
            std::cerr<<" lock failed"<<std::strerror(ret)<<"\n";
            munmap(data,sizeof(SharedMemory_mutex));
            close(fd);
            return 1;
        }

        timespec ts;
        clock_gettime(CLOCK_REALTIME,&ts);
        ts.tv_sec += 3;
        
        while(data->count == 0 && !data->shutdown)
        {
            int c_ret = pthread_cond_timedwait(&data->not_empty,&data->mutex,&ts);
            if(c_ret == ETIMEDOUT)
            {
                std::cout<<"no vehicle data recived on time ,so timeout \n";
                pthread_mutex_unlock(&data->mutex);
                munmap(data,sizeof(SharedMemory_mutex));
                close(fd);
                return 1;
            }
            else if(c_ret != 0)
            {
                std::cerr<<"condition_wait failed"<<std::strerror(c_ret)<<"\n";
                pthread_mutex_unlock(&data->mutex);
                munmap(data,sizeof(SharedMemory_mutex));
                close(fd);
                return 1;
            }
        }

        if(data->shutdown == true && data->count == 0)
        {
            pthread_mutex_unlock(&data->mutex);
            break;
        }

        std::cout<<" rpm "<<data->buffer[data->read_index].rpm<<" speed "<<data->buffer[data->read_index].speed<<"\n";
        data->read_index = (data->read_index + 1 ) % 4;
        data->count--;

        pthread_cond_signal(&data->not_full);
        pthread_mutex_unlock(&data->mutex);
    }

    // pthread_mutex_destroy(&data->mutex);
    munmap(data,sizeof(SharedMemory_mutex));
    close(fd);

    return 0;
 

}