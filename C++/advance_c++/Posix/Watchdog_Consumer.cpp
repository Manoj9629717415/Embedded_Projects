#include <iostream>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <pthread.h>
#include <ctime>
#include <chrono>

#include "Data.h"


int main()
{

    int fd = shm_open(shm_watchdog_name,O_RDWR,0666);

    if(fd == -1)
    {
        perror("shm_open");
        return 1;
    }
    
    SharedMemeory_Watchdog* heartbeat = static_cast<SharedMemeory_Watchdog*>(mmap(nullptr,sizeof(SharedMemeory_Watchdog),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0));

    if(heartbeat == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    uint16_t local_seq_number = 0;
    int m_retry = 3;

    while(true)
    {
        timespec m_ts;
        clock_gettime(CLOCK_REALTIME,&m_ts);
        m_ts.tv_sec += 2;

        int ret = pthread_mutex_timedlock(&heartbeat->mutex,&m_ts);
        if(ret == EOWNERDEAD)
        {
            std::cerr<<" mutex locked and holder crashed"<<std::strerror(ret)<<"\n";

            //since it is hreatbeat ,just reset the sequence number to 0
            pthread_mutex_consistent(&heartbeat->mutex);
        }
        else if(ret == ETIMEDOUT)
        {
            std::cerr<<" mutex locked timeout "<<std::strerror(ret)<<"\n";
            if(m_retry > 0 )
            {
                m_retry--;
                continue;
            }
            else{
                std::cerr<<" 3 retry finished for mutex locked timeout "<<std::strerror(ret)<<"\n";
                munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
                close(fd);
                return 1;
            }
        }
        else if(ret != 0)
        {
            std::cerr<<"  mutex locked error "<<std::strerror(ret)<<"\n";
            munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
            close(fd);
            return 1;
        }
        m_retry = 3;
        std::cout<<" localSequence_number "<<local_seq_number<<" heartbeat->sequence "<<heartbeat->sequence<<" \n";
        while(local_seq_number == heartbeat->sequence)
        {
            timespec c_ts;
            clock_gettime(CLOCK_REALTIME,&c_ts);
            c_ts.tv_sec += 3;
            int c_ret = pthread_cond_timedwait(&heartbeat->c_var_consumer,&heartbeat->mutex,&c_ts);

            if(c_ret == ETIMEDOUT)
            {
                if(m_retry > 0)
                {
                    m_retry--;
                    continue;
                }
                else
                {
                    std::cerr<<" 3 retry finished for consumer condition timeout "<<std::strerror(ret)<<"\n";
                    pthread_mutex_unlock(&heartbeat->mutex);
                    munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
                    close(fd);
                    return 1;
                }
            }
            else if(c_ret != 0)
            {
                std::cerr<<" consumer condition error "<<std::strerror(ret)<<"\n";
                pthread_mutex_unlock(&heartbeat->mutex);
                munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
                close(fd);
                return 1;
            }
        }

        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        if(heartbeat->sequence == 65535)
        {
            heartbeat->sequence = 0;
        }
        else{
            heartbeat->sequence++;
        }
        auto age = now - heartbeat->timestamp;
        std::cout<<" hreadbeat reacvied in "<<age<<" ms\n";
        local_seq_number = heartbeat->sequence;
        m_retry = 3;
        pthread_cond_signal(&heartbeat->c_var_producer);
        pthread_mutex_unlock(&heartbeat->mutex);

    }

    munmap(heartbeat,sizeof(SharedMemeory_Watchdog));
    close(fd);
    return 0;

}