#include <iostream>
#include <mqueue.h>
#include <unistd.h>
#include <cerrno>
#include <ctime>
#include <vector>
#include <cstring>
#include <string>
#include <set>
#include <map>

#include "Data.h"


int main()
{
    mq_attr attr{};
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(VehicleRequest);

    mqd_t request_queue = mq_open(request_queue_name,O_CREAT | O_RDONLY,0666,&attr);

    if(request_queue == (mqd_t)-1)
    {
        perror("mq_open");
        return 1;
    }

    mq_attr res_attr{};
    res_attr.mq_flags = 0;
    res_attr.mq_maxmsg = 10;
    res_attr.mq_msgsize = sizeof(VehicleResponse);

    std::vector<mqd_t> response_queues;
    for(int i = 0;i<number_of_clients;i++)
    {
        std::string name = client_response_prefix+std::to_string(i)+client_response_suffix;
        mqd_t response_queue = mq_open(name.c_str(),O_CREAT | O_WRONLY,0666,&res_attr);
        if(response_queue == (mqd_t)-1)
        {
            perror("mq_open: response_queue ");
            mq_close(request_queue);
            if(!response_queues.empty())
            {
                for(auto mq : response_queues)
                {
                    mq_close(mq);
                }
            }
            return 1;
        }
        else{
            response_queues.push_back(std::move(response_queue));
        }
    }

    std::set<std::pair<uint8_t,uint8_t>> request_ids;

    while(true)
    {
        VehicleRequest request{};
        uint prio = 0;
        ssize_t received_packets = mq_receive(request_queue,reinterpret_cast<char*>(&request),sizeof(VehicleRequest),&prio);

        if(received_packets != -1 )
        {
            VehicleResponse response{};
            if(request.command == COMMAND_RPM)
            {

                std::cout<<"request received for COMMAND_RPM \n";
                response.response_code = ResponseType::Positive;
                response.request_id = request.command;
                u_int16_t rpm = 1000;
                response.value = rpm;
            }
            else if(request.command == COMMAND_SPEED )
            {
                std::cout<<"request received for COMMAND_SPEED \n";
                response.response_code = ResponseType::Positive;
                response.request_id = request.command;
                u_int16_t speed = 100;
                response.value = speed;
            }
            else
            {
                std::cout<<"request received for 0 \n";
                response.response_code = ResponseType::Negative;
                response.request_id = request.command;
            }

            if(request.client_id < response_queues.size())
            {
                timespec s_ts;
                clock_gettime(CLOCK_REALTIME,&s_ts);
                s_ts.tv_sec += 2;
                sleep(1);
                if(mq_timedsend(response_queues[request.client_id],reinterpret_cast<const char*>(&response),sizeof(VehicleResponse),5,&s_ts) == -1)
                {
                    if(errno == ETIMEDOUT)
                    {
                        perror("mq_timedsend");
                        continue;
                    }
                    else
                    {
                        perror("mq_timedsend");
                        mq_close(request_queue);
                        return 1;
                    }
                }
            }
            else
            {
                std::cerr << "Invalid client ID: "
                        << static_cast<int>(request.client_id)
                        << '\n';
            }
            
        }
        else{
            perror("mq_receive");
            if (errno == EINTR)
                continue;

            break;
        }
    }
    mq_close(request_queue);
    if(!response_queues.empty())
    {
        for(auto mq : response_queues)
        {
            mq_close(mq);
        }
    }

}