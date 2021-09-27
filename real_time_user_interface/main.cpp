#include <iostream>
#include <thread>
#include <linux/sched.h>
#include <stdio.h>

void task(int n)
{
        long int x;

        printf("Task %i has been started\n", n);

        

        while (1)
        {
                printf("task %d: running\n", n);
                x = 1000000;
                while (x > 0) x--;
                printf("task %d: stopped\n", n);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
}

void CreateTasks(void)
{
        std::thread id1(task,0);
        std::thread id2(task,1);
        std::thread id3(task,2);

        id1.join();
        id2.join();
        id3.join();
}

int main()
{
        CreateTasks();    
}

