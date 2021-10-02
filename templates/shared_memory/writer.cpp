#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

struct company {
    char name[20];
    int work_done;
};

struct company_registry {
    struct company companies[50];
};
struct company_registry * ptr;


void init_shm()
{
    key_t key = ftok("/company",65);

    int shmid = shmget(key,1024,0644|IPC_CREAT);

    char * str = (char*) shmat(shmid,(void*)0,0);

    std::cout<<"Write Date : ";
    std::cin.getline(str,20);

    printf("Data written in memory: %s\n",str);

    shmdt(str);
    // shmctl(shmid,IPC_RMID,NULL);
}
int main()
{   
    init_shm();
    return 0;
}