#include <iostream>
#include <thread>
#include <linux/sched.h>
#include <stdio.h>

long int task_run[]={100, 450, 200};
int task_stop[]={18, 25, 30};

void task(int n)
{
        long int x;

        printf("Task %i has been started\n", n);

        

        while (1)
        {
                printf("task %d: running\n", n);
                x = 1000000 * task_run[n];
                while (x > 0) x--;
                printf("task %d: stopped\n", n);
                std::this_thread::sleep_for(std::chrono::milliseconds(task_stop[n]));
        }
}

void kernelTimeSlicing()
{
        struct sched_param sp = {
        .sched_priority = 90
        };
        pid_t pid = 0;
        sched_setscheduler(pid, SCHED_RR, &sp);
        // sched_setscheduler(pid, SCHED_OTHER, &sp);
}


void CreateTasks(void)
{
        
        kernelTimeSlicing();
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

