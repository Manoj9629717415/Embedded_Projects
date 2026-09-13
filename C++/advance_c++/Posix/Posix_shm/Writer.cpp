#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "Data.h"
#include <thread>
#include <chrono>
#include <semaphore.h>

int main()
{

    int fd = shm_open(shm_name,O_CREAT | O_RDWR,0666);

    if(fd == -1){
        perror("shm_open");
        return 1;
    }

    if(ftruncate(fd,sizeof(VehicleData)) == -1)
    {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    sem_t* sem = sem_open(sem_name,O_CREAT,0666,1);

    if(sem == SEM_FAILED)
    {
        perror("semphore");
        close(fd);
        return 1;
    }

    VehicleData* data = static_cast<VehicleData*>(mmap(nullptr,sizeof(VehicleData),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0));

    if(data == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    sem_wait(sem);

    data->rpm = 1000;
    sleep(5);
    data->speed = 30;

    sem_post(sem);

    sleep(5);

    munmap(data,sizeof(VehicleData));

    sem_close(sem);

    close(fd);

    // shm_unlink(name);

    sem_unlink(sem_name);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    

}