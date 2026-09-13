#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <iostream>
#include <cstring>
#include <ctime>
#include <chrono>

#include "Data.h"



int main(){

    int fd = shm_open(shm_watchdog_name,O_CREAT | O_EXCL | O_RDWR,0666);

    bool creator = false;

    if(fd != -1)
    {
        std::cout<<"shm_watchdog_name opened \n";
        creator = true;
    }
    else if(errno == EEXIST)
    {
        fd = shm_open(shm_watchdog_name,O_RDWR,0666);
        if(fd == -1)
        {
            perror("shm_open");
            return 1;
        }
    }
    else{
        perror("shm_open");
        return 1;
    }

    if(creator)
    {
        if(ftruncate(fd,sizeof(SharedMemeory_Watchdog)) == -1)
        {
            perror("ftruncate");
            close(fd);
            return 1;
        }
    }

    SharedMemeory_Watchdog* heartbeat = static_cast<SharedMemeory_Watchdog*>(mmap(nullptr,sizeof(SharedMemeory_Watchdog),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0));

    if(heartbeat == MAP_FAILED)
    {
        std::cerr<<"mmap failed \n";
        close(fd);
        return 1;
    }

    if(creator)
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
        pthread_mutexattr_setrobust(&attr,PTHREAD_MUTEX_ROBUST);

        pthread_mutex_init(&heartbeat->mutex,&attr);
        pthread_mutexattr_destroy(&attr);

        pthread_condattr_t c_attr;
        pthread_condattr_init(&c_attr);
        pthread_condattr_setpshared(&c_attr,PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&heartbeat->c_var_producer,&c_attr);
        pthread_cond_init(&heartbeat->c_var_consumer,&c_attr);
        pthread_condattr_destroy(&c_attr);

        heartbeat->sequence = 0;
    }

    int retry = 5;
    int localSequence_number = 1;

    while(true)
    {
        int ret = pthread_mutex_lock(&heartbeat->mutex);
        if(ret == EOWNERDEAD)
        {
            std::cerr<<" mutex locked and holder crashed"<<std::strerror(ret)<<"\n";
            pthread_mutex_consistent(&heartbeat->mutex);
        }
        else if(ret != 0)
        {
            std::cerr<<" mutex lock failed"<<std::strerror(ret)<<"\n";
            munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
            close(fd);
            return 1;
        }
        std::cout<<" localSequence_number "<<localSequence_number<<" heartbeat->sequence "<<heartbeat->sequence<<" \n";
        while(localSequence_number == heartbeat->sequence)
        {
             timespec ts;
            clock_gettime(CLOCK_REALTIME,&ts);
            ts.tv_sec += 3;

            int c_ret = pthread_cond_timedwait(&heartbeat->c_var_producer,&heartbeat->mutex,&ts);

            if(c_ret == ETIMEDOUT)
            {
                std::cerr<<" producer condition var not waked on time,do retry 3 times "<<std::strerror(c_ret)<<"\n";
                pthread_mutex_unlock(&heartbeat->mutex);
                retry--;
                if(retry > 0)
                {
                    continue;
                }
                else{
                    munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
                    close(fd);
                    return 1;
                }
            }
            else if(c_ret != 0)
            {
                pthread_mutex_unlock(&heartbeat->mutex);
                munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
                close(fd);
                return 1;

            }
        }

        if(heartbeat->sequence == 65535)
        {
            heartbeat->sequence = 0;
        }
        else{
            heartbeat->sequence++;
        }
        heartbeat->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        localSequence_number = heartbeat->sequence;

        pthread_cond_signal(&heartbeat->c_var_consumer);
        pthread_mutex_unlock(&heartbeat->mutex);

        retry = 3;

        sleep(1);
    }

    munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
    close(fd);
    return 0;


}