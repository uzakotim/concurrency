#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#include "main.h"
#include <thread>

#define INPUT_WAIT_TIME 100

key_t key = ftok("/company",65);
int shmid = shmget(key,1024,0644|IPC_CREAT); 
struct company_registry* ptr = (company_registry *) shmat(shmid,(void*)0,0);


void user_input()
{
    struct termios old_tio, new_tio;
    unsigned char c;

    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO,&old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio=old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &=(~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

    do {
            c=getchar();
            printf("%c \n",c);
            std::this_thread::sleep_for(std::chrono::milliseconds(INPUT_WAIT_TIME));

    } while((c!='q'));
    shmdt(ptr);
    shmctl(shmid,IPC_RMID,NULL);
    /* restore the former settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
}
void monitorData()
{
    printf("--------------------------------------------------\n");
    for (;;){
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
}

int main()
{
    std::thread monitor(monitorData);
    std::thread user_interface(user_input);

    monitor.join();
    user_interface.join();
    return 0;

}