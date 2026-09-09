#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include "Data.h"
#include <fcntl.h>
#include <iostream>

int main()
{
    int fd = shm_open(shm_name,O_CREAT | O_EXCL | O_RDWR,0666);
    
    bool creator = false;

    if(fd != -1)
    {
        creator = true;
        std::cout<<"I am only creating shared memory \n";
    }
    else if(errno == EEXIST)
    {
       fd = shm_open(shm_name, O_RDWR,0666);
       if (fd == -1) {
            perror("shm_open existing");
            return 1;
        }
        std::cout<<"already existing shared memory ,just opening it \n";
    }
    else
    {
        perror("shm_open");
        return 1;
    }

    if(creator == true)
    {
        if(ftruncate(fd,sizeof(SharedBuffer)) == -1)
        {
            perror("ftruncate ");
            close(fd);
            return 1;
        }
    }
    

    SharedBuffer* shared_buffer;
    shared_buffer = static_cast<SharedBuffer*>(mmap(nullptr,sizeof(SharedBuffer),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0));

    if (shared_buffer == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    if(creator==true){
        shared_buffer->write_index = 0;
        shared_buffer->read_index = 0;
    }

    sem_t* empty_slot = sem_open(producer_sem,O_CREAT | O_EXCL,0666,4);

    bool semaphore_creator = false;

    if(empty_slot == SEM_FAILED)
    {
        if(errno == EEXIST)
        {
            empty_slot = sem_open(producer_sem,0);
            if(empty_slot == SEM_FAILED)
            {
                perror(" producer_sem sem error");
                munmap(shared_buffer,sizeof(SharedBuffer));
                close(fd);
                return 1;
            }
            std::cout<<"already existing empty_slot semaphore  ,just opening it \n";
        }
        else{
            perror(" producer_sem sem error");
            munmap(shared_buffer,sizeof(SharedBuffer));
            close(fd);
            return 1;
        }
    }
    else {semaphore_creator = true;}

    sem_t* filled_slot = sem_open(consumer_sem,O_CREAT | O_EXCL,0666,0); 
    if(filled_slot == SEM_FAILED)
    {
        if(errno == EEXIST)
        {
            filled_slot = sem_open(consumer_sem,0);
            if(filled_slot == SEM_FAILED)
            {
                perror(" consumer_sem sem error");
                munmap(shared_buffer,sizeof(SharedBuffer));
                sem_close(empty_slot);
                close(fd);
                return 1;
            }
            std::cout<<"already existing filled_slot semaphore  ,just opening it \n";
        }
        else{
            perror(" consumer_sem sem error");
            munmap(shared_buffer,sizeof(SharedBuffer));
            sem_close(empty_slot);
            close(fd);
            return 1;
        }
    }



    int count = 0;
    while(count <10)
    {
        sem_wait(empty_slot);
        shared_buffer->buffer[shared_buffer->write_index].rpm = count  * 1000;
        shared_buffer->buffer[shared_buffer->write_index].speed = count * 10;
        shared_buffer->write_index = (shared_buffer->write_index + 1) % BUFFER_SIZE;
        sem_post(filled_slot);
        sleep(2);
        count++;
    }

    sleep(5);

    sem_close(empty_slot);
    sem_close(filled_slot);
    munmap(shared_buffer,sizeof(SharedBuffer));
    close(fd);

}