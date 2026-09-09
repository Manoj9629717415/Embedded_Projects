#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include "Data.h"

int main()
{
    int fd = shm_open(shm_name,O_RDWR,0666);

    if(fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    SharedBuffer* shared_buffer;
    shared_buffer = static_cast<SharedBuffer*>(mmap(nullptr,sizeof(SharedBuffer),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0));

    if(shared_buffer == MAP_FAILED)
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    sem_t* empty_slot = sem_open(producer_sem,0);

    if(empty_slot == SEM_FAILED)
    {
        perror("sem_open");
        munmap(shared_buffer,sizeof(SharedBuffer));
        close(fd);
        return 1;
    }

    sem_t* filled_slot = sem_open(consumer_sem,0);

    if(filled_slot == SEM_FAILED)
    {
        perror("sem_open");
        sem_close(empty_slot);
        munmap(shared_buffer,sizeof(SharedBuffer));
        close(fd);
        return 1;
    }

    int count = 0;
    while(count <10)
    {
        sem_wait(filled_slot);
        std::cout<<" rpm "<<shared_buffer->buffer[shared_buffer->read_index].rpm<<" speed "<<shared_buffer->buffer[shared_buffer->read_index].speed<<"\n";
        shared_buffer->read_index = (shared_buffer->read_index + 1) % BUFFER_SIZE;
        sem_post(empty_slot);
        sleep(5);
        count++;
    }

    sem_close(empty_slot);
    sem_close(filled_slot);
    munmap(shared_buffer,sizeof(SharedBuffer));
    close(fd);

}