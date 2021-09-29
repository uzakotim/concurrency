#if !defined(MAIN_H)
#define MAIN_H

#include <iostream>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <termios.h>
#include <vector>
#include <pthread.h>
#include <mutex>
#include <queue>
#include <stack>

#define MAX_NUMBER_OF_WORKERS 50
#define WORK_TIME 50
#define BREAK_TIME 50

std::mutex * shovel1;
std::mutex * shovel2;
std::mutex * shovel3;
std::mutex semSoilHeap;
std::stack<std::mutex*> semShovels;


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