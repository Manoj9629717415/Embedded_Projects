#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include "Data.h"
#include <unistd.h>
#include <semaphore.h>


int main(){
    int fd = shm_open(shm_name,O_RDONLY,0666);

    

    if(fd==-1){
        perror("shm_open");
        return 1;
    }

    VehicleData* data = static_cast<VehicleData*>(mmap(nullptr,sizeof(VehicleData),PROT_READ,MAP_SHARED,fd,0));

    if(data == MAP_FAILED)
    {
        perror("mmap failed");
        munmap(data,sizeof(VehicleData));
        close(fd);
        return 1;
    }

    sem_t* sem = sem_open(sem_name,0);

    if(sem == SEM_FAILED){ 
        perror("sem_error");
        close(fd);
        return 1;
    }

    sem_wait(sem);
    std::cout<<" rpm "<<data->rpm<<"\n";
    std::cout<<" speed "<<data->speed<<"\n";
    sem_post(sem);

    munmap(data,sizeof(VehicleData));

    close(fd);
    sem_close(sem);

    return 0;
}