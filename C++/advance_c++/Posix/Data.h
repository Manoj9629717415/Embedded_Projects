#ifndef DATA_H
#define DATA_H
#include <pthread.h>

const char* shm_name = "/vehicle_data";
const char* sem_name = "/vehicle_semaphore";
const char* producer_sem = "/empty_slots";
const char* consumer_sem = "/filled_slots";

constexpr int BUFFER_SIZE = 4;

struct VehicleData
{
    int speed;
    int rpm;
};

struct SharedBuffer{
    VehicleData buffer[BUFFER_SIZE];
    int write_index;
    int read_index;
};

struct SharedMemory_mutex{
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

    VehicleData buffer[BUFFER_SIZE];
    int write_index;
    int read_index;
    int count;
    bool shutdown;

    bool update_inprogress;
    int prev_write_index;
    int prev_count;
};


#endif