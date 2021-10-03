#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#include "main.h"
#include <thread>


void monitorData()
{
    
    key_t key = ftok("/company",65);

    int shmid = shmget(key,1024,0644|IPC_CREAT); 
    printf("--------------------------------------------------\n");
    for(;;)
    {
        struct company_registry * ptr = (company_registry *) shmat(shmid,(void*)0,0);
        for (auto&company :ptr->companies)
        {
            if (company.work_done != 0)
            {
            printf("Data read from memory: %s %d\n",company.name,company.work_done);
            printf("--------------------------------------------------\n");
            }
        }
        shmdt(ptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(MONITOR_WAIT_TIME));

    }
   
    // shmctl(shmid,IPC_RMID,NULL);
}

int main()
{
    std::thread monitor(monitorData);
    monitor.join();
    return 0;

}