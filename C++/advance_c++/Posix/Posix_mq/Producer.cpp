#include <iostream>
#include <mqueue.h>
#include <fcntl.h>
#include <cstring>
#include <sys/stat.h>
#include <ctime>

#include "Data.h"


int main()
{
    mq_attr attr{};
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(VehicleMessage);
    attr.mq_curmsgs = 0;

    mqd_t mq = mq_open(queue_name,O_CREAT | O_WRONLY,0666,&attr);  // Blocking message queue

    // mqd_t mq = mq_open(queue_name,O_CREAT | O_WRONLY | O_NONBLOCK,0666,&attr);  //Non Blocking message queue

    if(mq == (mqd_t)-1)
    {
        perror("mqd_open");
        return 1;
    }
    int count = 1;
    while(count < 4)
    {
        VehicleMessage msg{1500,60};
        timespec ts;
        clock_gettime(CLOCK_REALTIME,&ts);
        ts.tv_sec += 2;

        // if(mq_send(mq,reinterpret_cast<const char*>(&msg),sizeof(VehicleMessage),1) == -1)
        if(mq_timedsend(mq,reinterpret_cast<const char*>(&msg),sizeof(VehicleMessage),1,&ts) == -1)
        {
            perror("mq_send");
            mq_close(mq);
            return 1;
        }
        std::cout<<"producer sent rpm: "<<msg.rpm<<" speed "<<msg.speed<<"\n";

        msg.rpm = 2000;
        msg.speed = 80;
        clock_gettime(CLOCK_REALTIME,&ts);
        ts.tv_sec += 2;
        // if(mq_send(mq,reinterpret_cast<const char*>(&msg),sizeof(VehicleMessage),5) == -1)
        if(mq_timedsend(mq,reinterpret_cast<const char*>(&msg),sizeof(VehicleMessage),5,&ts) == -1)
        {
            perror("mq_send");
            mq_close(mq);
            return 1;
        }
        std::cout<<"producer sent rpm: "<<msg.rpm<<" speed "<<msg.speed<<"\n";

        msg.rpm = 1000;
        msg.speed = 40;
        clock_gettime(CLOCK_REALTIME,&ts);
        ts.tv_sec += 2;
        // if(mq_send(mq,reinterpret_cast<const char*>(&msg),sizeof(VehicleMessage),2) == -1)
        if(mq_timedsend(mq,reinterpret_cast<const char*>(&msg),sizeof(VehicleMessage),2,&ts) == -1)
        {
            perror("mq_send");
            mq_close(mq);
            return 1;
        }

        std::cout<<"producer sent rpm: "<<msg.rpm<<" speed "<<msg.speed<<"\n";
        count++;
    }

    mq_close(mq);

    return 0;


}