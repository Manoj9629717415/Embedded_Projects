#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main()
{
    int value = 10;
    int* shared_value = static_cast<int*>(mmap(nullptr,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0));

   if(shared_value == MAP_FAILED)
   {
    std::cerr<<" Maping failed \n";
   }

   *shared_value = 10;

    std::cout<<" before fork value "<<value<<" \n";

    pid_t pid = fork();

    if(pid < 0)
    {
        std::cout<<" fork failed \n";
    }
    else if(pid == 0)
    {
        value += 10;
        std::cout<<" this is Child process \n";
        std::cout<<" child pid "<<getpid()<<"\n";
        std::cout<<" parent pid "<<getppid()<<"\n";
        std::cout<<" child process value "<<value<<" \n";

        std::cout<<" child sees shared value "<<*shared_value<<"\n";
        *shared_value = 100;

        std::cout<<" child changed shared value "<<*shared_value<<"\n";

    }
    else{
        value += 20;
        std::cout<<" this is Parent process \n";
        std::cout<<" parentpid "<<getpid()<<"\n";
        std::cout<<" child pid "<<pid<<"\n";
        std::cout<<" Parent process value "<<value<<" \n";

        wait(nullptr);

         std::cout << "Parent sees: "
                  << *shared_value << "\n";
    }
    munmap(shared_value,sizeof(int));
return 0;

}