#include "main.h"

pthread_mutex_t stack[STACK_SIZE];
int top = -1;

void push(pthread_mutex_t mutex, pthread_mutex_t stack[],int *top)
{
    if (*top==-1)
    {
        stack[STACK_SIZE-1] = mutex;
        *top = STACK_SIZE - 1;
    }
    else if (*top == 0)
    {
        printf("The stack is full \n");
    }
    else
    {
        stack[(*top) - 1] = mutex;
        (*top)--;
    }
}

void pop(pthread_mutex_t stack[],int *top)
{
    if(*top == -1)
    {
        printf("The stack is empty.\n");
    }
    else 
    {
        // Uncomment for debuggings
        // printf("The shovel popped\n");
        if((*top)== STACK_SIZE-1)
        {
            (*top)= -1;
        }
        else
        {
            (*top)++;
        }
    }
}
void delete_lower_task(int id)
{
    pthread_cancel(thread_lower_handles[id]);
}
void delete_upper_task(int id)
{
    pthread_cancel(thread_upper_handles[id]);
}
void delete_all_tasks()
{
    for (int i =0;i<MAX_NUMBER_OF_WORKERS;i++)
    {
        pthread_cancel(thread_lower_handles[i]);
        pthread_cancel(thread_upper_handles[i]);
    }
}
void* digger_lower(void* p)
{
    while(1)
    {
        pthread_mutex_t shovel = stack[top];
        pop(stack,&top);
        pthread_mutex_lock(&shovel);
        printf("lower digger %d: working\n",*((int*)p));
        sleep(WORK_TIME/1000);
        pthread_mutex_unlock(&shovel);
        push(shovel,stack,&top);
        pthread_mutex_unlock(&semSoilHeap);
        printf("lower digger %d: resting\n", *((int*)p));
        sleep(BREAK_TIME/1000);
    }
}
void* digger_upper(void* p)
{
    while(1)
    {
        if (pthread_mutex_trylock(&semSoilHeap))
        {
            pthread_mutex_t shovel = stack[top];
            pop(stack,&top);
            pthread_mutex_lock(&shovel);
            printf("upper digger %d: working\n",*((int*)p));
            sleep(WORK_TIME/1000);
            pthread_mutex_unlock(&shovel);
            push(shovel,stack,&top);
            printf("upper digger %d: resting\n", *((int*)p));
            sleep(BREAK_TIME/1000);
        } else
        {
            printf("upper digger %d: resting\n", *((int*)p));
            sleep(BREAK_TIME/1000);
        }
        
    }
}

void create_lower_task(int id)
{
    int *arg = malloc(sizeof(*arg));
    if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
    *arg = id;
    pthread_t temp;
    pthread_create(&temp,NULL,digger_lower,arg);
    thread_lower_handles[id] = temp;
    pthread_detach(temp);
}

void create_upper_task(int id)
{
    int *arg = malloc(sizeof(*arg));
    if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
    *arg = id;
    pthread_t temp;
    pthread_create(&temp,NULL,digger_upper,arg);
    thread_upper_handles[id] = temp;
    pthread_detach(temp);
}

void* take_input()
{
    create_lower_task(0);
    create_upper_task(0);
    
    int id_lower = 1;
    int id_upper = 1;
    bool deleted = false;

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

    do
    {
        c= getchar();
        printf("%c \n",c);
        sleep(0.1);

        if (c == 'i') 
            {
                if (id_lower < MAX_NUMBER_OF_WORKERS)
                {
                    create_lower_task(id_lower);
                    id_lower++;
                }
                else {
                    fprintf(stderr, "Too many workers\n");
                    id_lower = MAX_NUMBER_OF_WORKERS-1;
                }
            }
        if (c == 'o')
            { 
                    if(id_lower>0){
                            if (id_lower !=0) id_lower--;
                            delete_lower_task(id_lower);
                    } else {
                            fprintf(stderr, "All workers are stopped\n");
                    }
            }
            
         if (c == 'I') 
            {
                if (id_upper<MAX_NUMBER_OF_WORKERS){
                        create_upper_task(id_upper);
                        id_upper++;
                } else {
                    fprintf(stderr, "Too many workers\n");
                    id_upper = MAX_NUMBER_OF_WORKERS-1;
                }
                    
            }
        if (c == 'O')
            { 
                    if(id_upper>0){
                            if (id_upper !=0) id_upper--;
                            delete_upper_task(id_upper);
                    } else {
                            fprintf(stderr, "All workers are stopped\n");
                    }
            }
         if (c == 'E')
                        {       
                                if ((id_lower+id_upper)>0) {
                                        delete_all_tasks();
                                        id_lower = 0;
                                        id_upper = 0;
                                }
                        }

    } while(c!='q');

    /* restore the former settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
}
void CreateTasks()
{
    
    push(shovel1,stack,&top);
    push(shovel2,stack,&top);
    push(shovel3,stack,&top);

    pthread_mutex_lock(&semSoilHeap);

    pthread_t id1;
    pthread_create(&id1,NULL,take_input,NULL);

    int* ptr;  
    pthread_join(id1, (void**)&ptr);
}

int main()
{
    CreateTasks();
    return 0;
}