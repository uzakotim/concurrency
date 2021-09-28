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

#define MAX_NUMBER_OF_WORKERS 50


namespace diggers
{   
    std::thread::native_handle_type thread_handles[MAX_NUMBER_OF_WORKERS];

    void digger(int counter);
    void create_task(int id);
    void delete_task(int id);
    void take_input();
} // namespace diggers
#endif //MAIN_H