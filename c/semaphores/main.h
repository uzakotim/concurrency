#if !defined(MAIN_H)
#define MAIN_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <termios.h>
#include <stdbool.h>

#define MAX_NUMBER_OF_WORKERS 50
#define WORK_TIME 50
#define BREAK_TIME 50
#define STACK_SIZE 3

pthread_mutex_t shovel1;
pthread_mutex_t shovel2;
pthread_mutex_t shovel3;
pthread_mutex_t semSoilHeap;

pthread_t thread_lower_handles[MAX_NUMBER_OF_WORKERS];
pthread_t thread_upper_handles[MAX_NUMBER_OF_WORKERS];

#endif //MAIN_
