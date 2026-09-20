#include <iostream>
#include <mqueue.h>
#include <unistd.h>
#include <cerrno>
#include <ctime>
#include "Data.h"
#include <string>


int main()
{

    mqd_t request_queue = mq_open(request_queue_name,O_WRONLY);

    if(request_queue == (mqd_t)-1)
    {
        perror("mq_open");
        return 1;
    }

    std::string name = client_response_prefix+std::to_string(Client1)+client_response_suffix;

    mqd_t response_queue = mq_open(name.c_str(),O_RDONLY);

    if(response_queue == (mqd_t)-1)
    {
        perror("mq_open");
        mq_close(request_queue);
        return 1;
    }

    int flag = 0;
    while(true)
    {
        VehicleRequest request{};
        request.client_id = Client1;

        if(flag == 0 )
        {
            request.request_id = RequestId::DID_0x100;
            request.command = COMMAND_RPM;
            flag++;
        }
        else if(flag == 1)
        {
            request.request_id = RequestId::DID_0x101;
            request.command = COMMAND_SPEED;
            flag++;
        }
        else
        {
            request.request_id = RequestId::DID_0x102;
            request.command = 0;
            flag = 0;
        }
        int retry = 3;

        if(mq_send(request_queue,reinterpret_cast<const char*>(&request),sizeof(VehicleRequest),5) != -1)
        {
            std::cout<<" request send for command "<<request.command<<"\n";
            VehicleResponse response{};
            uint prio;
            timespec ts;
            clock_gettime(CLOCK_REALTIME,&ts);
            ts.tv_sec += 4 ;
            ssize_t recevied_packets = mq_timedreceive(response_queue,reinterpret_cast<char*>(&response),sizeof(VehicleResponse),&prio,&ts);

            if(recevied_packets == -1)
            {
                if(errno == ETIMEDOUT)
                {
                    std::cout<<"response not recived o time for command "<<request.command<<" \n";
                }
                else{
                    perror("mq_receieve");
                    mq_close(request_queue);
                    mq_close(response_queue);
                    return 1;
                }
            }
            else{
                if(request.command != response.request_id)
                {
                    std::cout<<"stale message received requested id "<<request.command<<"recived response for  "<<response.request_id<<"\n";
                }
                else{
                    if(response.response_code == ResponseType::Negative)
                    {
                        std::cout<<"negative resonse from server for request "<<response.request_id<<"\n";
                    }
                    else
                    {
                        std::cout<<"valid response validate the resppnse \n";
                        u_int16_t result = response.value;
                        u_int8_t request_id = response.request_id;
                        std::cout<<" response for request id "<<request_id<<" from server "<<result<<"\n";
                    }
                }
            }
        }
        else{
            mq_close(request_queue);
            mq_close(response_queue);
            return 1;
        }
        sleep(2);
    }

    mq_close(request_queue);
    mq_close(response_queue);
    return 0;

}