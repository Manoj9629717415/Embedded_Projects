#include <iostream>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
#include "Data.h"

int main()
{
    mqd_t mq = mq_open(queue_name,O_RDONLY); // Blocking queue

    // mqd_t mq = mq_open(queue_name,O_RDONLY|O_NONBLOCK); // Non Blocking queue

    if(mq == (mqd_t)-1)
    {
        perror("mq_open");
        return 1;
    }

    VehicleMessage msg{};
    unsigned int priority = 0;
    timespec ts;

    while(true)
    {
        clock_gettime(CLOCK_REALTIME,&ts);
        ts.tv_sec += 5;
        // ssize_t recevied_packet = mq_receive(mq,reinterpret_cast<const char *>(&msg),sizeof(VehicleMessage),&priority);
        ssize_t recevied_packet = mq_timedreceive(mq,reinterpret_cast<char*>(&msg),sizeof(VehicleMessage),&priority,&ts);

        if(recevied_packet == -1)
        {
            if(errno == ETIMEDOUT)
            {
                perror("mq_receive");
            }
            else{
                perror("mq_receive");
                mq_close(mq);
                return 1;
            }
        }
        else{
            std::cout<<"received message rpm "<<msg.rpm<<"speed "<<msg.speed<<"prio "<<priority<<"\n";
        }
        sleep(2);
    }

    mq_close(mq);

    return 0;


}