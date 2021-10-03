#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#include "main.h"

void init_shm()
{
    key_t key = ftok("/company",65);

    int shmid = shmget(key,1024,0644|IPC_CREAT);

    struct company_registry* ptr = (company_registry *) shmat(shmid,(void*)0,0);

    std::cout<<"Write Name : ";
    std::cin.getline(ptr->companies[0].name,20);
    std::cout<<"Write work done: ";
    std::cin>>(ptr->companies[0].work_done);

    printf("Data written in memory: %s\n",ptr->companies[0].name);
    shmdt(ptr);
}
int main()
{   
    init_shm();
    return 0;
}