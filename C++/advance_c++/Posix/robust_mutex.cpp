#include <iostream>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

struct Shared_data{
    pthread_mutex_t mutex;
    int value;
};


int main()
{
   
    Shared_data* data = static_cast<Shared_data*>(mmap(nullptr,sizeof(Shared_data),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0));

    if(data == MAP_FAILED)
    {
        perror("mmap failed");
        return 1;
    }

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_setrobust(&attr,PTHREAD_MUTEX_ROBUST);

    pthread_mutex_init(&data->mutex,&attr);

    pthread_mutexattr_destroy(&attr);

    data->value = 0;

    pid_t pid = fork();

    if(pid == -1)
    {
        perror("fork failed");
    }

    if(pid == 0)
    {
         // Child
        std::cout << "Child: locking mutex\n";
        int ret = pthread_mutex_lock(&data->mutex);

        if(ret != 0)
        {
             // Child
            std::cout << "Child: locking mutex\n";
            return 1;
        }

        data->value = 100;

        std::cout << "Child: value = "
                  << data->value << '\n';

        std::cout << "Child: crashing now...\n";

        _exit(1);
       // Intentionally terminate without unlock

    }

    waitpid(pid,nullptr,0);
    std::cout << "Parent: child terminated\n";
    std::cout << "Parent: trying to lock mutex\n";

    int ret = pthread_mutex_lock(&data->mutex);

    if(ret == EOWNERDEAD)
    {
        std::cout << "Parent: owner died!\n";

        std::cout << "Parent: value = "
                  << data->value << '\n';

        // We must decide whether shared state is
        // still consistent.
        data->value = 0;

        pthread_mutex_consistent(&data->mutex);

        std::cout << "Parent: state recovered\n";
    }
    else if(ret != 0)
    {
        std::cerr << "Parent lock failed: "
                  << std::strerror(ret) << '\n';
        return 1;
    }

    pthread_mutex_unlock(&data->mutex);

    pthread_mutex_destroy(&data->mutex);

    munmap(data,sizeof(Shared_data));

    return 0;

}