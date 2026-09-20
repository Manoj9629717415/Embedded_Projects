#include <iostream>
#include <mqueue.h>

#include "Data.h"



int main()
{
    if(mq_unlink(queue_name) == -1)
    {
        perror("mq_unlink");
        return 1;
    }

    std::cout << "Message queue removed\n";
}