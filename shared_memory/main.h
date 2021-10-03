#if !defined(MAIN_H)
#define MAIN_H

#define INPUT_WAIT_TIME 100
#define MAX_NUMBER_OF_COMPANIES 50
#define MAX_LEMGTH_OF_COMPANY_NAME 20
#define MONITOR_WAIT_TIME 1000
#define MAX_NUMBER_OF_WORKERS 50
#define WORK_TIME 50
#define BREAK_TIME 50


#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <linux/input.h>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <stdlib.h>
#include <stack>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <vector>




std::mutex * shovel1;
std::mutex * shovel2;
std::mutex * shovel3;
std::mutex semSoilHeap;
std::stack<std::mutex*> semShovels;

struct company {
    char name[MAX_LEMGTH_OF_COMPANY_NAME];
    int work_done;
};

struct company_registry {
    struct company companies[MAX_NUMBER_OF_COMPANIES];
};

namespace diggers
{   

    std::thread::native_handle_type thread_lower_handles[MAX_NUMBER_OF_WORKERS];
    std::thread::native_handle_type thread_upper_handles[MAX_NUMBER_OF_WORKERS];
    
    
    void digger_lower(int counter);
    void digger_upper(int counter);
    void create_lower_task(int id);
    void create_upper_task(int id);
    void delete_lower_task(int id);
    void delete_upper_task(int id);
    void delete_all_tasks();

    void take_input();
} // namespace diggers

#endif //MAIN_H