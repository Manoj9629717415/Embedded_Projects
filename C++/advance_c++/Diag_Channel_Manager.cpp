#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>


class Diag_ChannelManager {
public:
    void sendRequest(std::stop_token token,std::string component)
    {
        while(!token.stop_requested())
        {
            std::cout<<" trying to get channel for compoent "<<component<<std::endl;
            if (m_channel_slot.try_acquire())
            {            
                std::cout<<" got channel for compoent and request sent "<<component<<std::endl;
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_queue.push(component);
                }
                m_cv.notify_one();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

    }

    void process_diag(std::stop_token token)
    {
        while(!token.stop_requested())
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock,token,[this](){
                return !m_queue.empty();
            });

            if(m_queue.empty() && token.stop_requested()) break;

            std::string component_reso = std::move(m_queue.front());
            m_queue.pop();
            lock.unlock();
            // send request and wait for response so sleep 100ms
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout<<" Response received for component "<<component_reso<<std::endl;
            m_channel_slot.release();
        }
    }



private:
    std::counting_semaphore<3> m_channel_slot{3};
    std::queue<std::string> m_queue{};
    std::mutex m_mutex;
    std::condition_variable_any m_cv;

};


int main(){
    Diag_ChannelManager diag_manager;
    std::vector<std::string> components{"com1","com2","com3","com4","com5","com6","com7","com8"};
    std::vector<std::jthread> sender;
    for(int i = 0;i<8;i++)
    {
        sender.emplace_back([&components,&diag_manager,i](std::stop_token token){
            diag_manager.sendRequest(token,components[i]);
        });
    }

    std::jthread receiver([&diag_manager](std::stop_token token){
        diag_manager.process_diag(token);
    });


    std::this_thread::sleep_for(std::chrono::seconds(10));

    for(auto& th: sender)
    {
        th.request_stop();
    }

    receiver.request_stop();
    

}