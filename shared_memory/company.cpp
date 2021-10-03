#include "main.h"

key_t key = ftok("/company",65);
int shmid = shmget(key,1024,0644|IPC_CREAT);
struct company_registry* ptr = (company_registry *) shmat(shmid,(void*)0,0);
int counter =0;


namespace diggers
{
        void digger_lower(int id)
        {       
                while(1)
                {       
                        if(!semShovels.empty())
                        {
                                std::mutex* shovel = semShovels.top();
                                semShovels.pop();
                                shovel->lock();
                                std::cout<<"lower digger "<<id<<": working \n";
                                std::this_thread::sleep_for(std::chrono::milliseconds(WORK_TIME));
                                shovel->unlock();
                                semShovels.push(shovel);
                                semSoilHeap.unlock();

                                ptr->companies[counter].work_done++;
                                
                        }
                        std::cout<<"lower digger "<<id<<": resting\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(BREAK_TIME));
                }
        }
        void digger_upper(int id)
        {
                while(1)
                {
                        // std::cout<<semSoilHeap.try_lock(); // 0 if locked ; 1 if unlocked ;
                        if(semSoilHeap.try_lock()){
                                if(!semShovels.empty())
                                {
                                        std::mutex* shovel = semShovels.top();
                                        semShovels.pop();
                                        shovel->lock();
                                        std::cout<<"upper digger "<<id<<": working \n";
                                        std::this_thread::sleep_for(std::chrono::milliseconds(WORK_TIME));
                                        shovel->unlock();
                                        semShovels.push(shovel);

                                        semSoilHeap.unlock();
                                        ptr->companies[counter].work_done++;
                                        
                                } 
                                else 
                                {
                                        std::cout<<"upper digger "<<id<<": resting\n";
                                        std::this_thread::sleep_for(std::chrono::milliseconds(BREAK_TIME));   
                                }
                        } else { 
                                std::cout<<"upper digger "<<id<<": resting\n";
                                std::this_thread::sleep_for(std::chrono::milliseconds(BREAK_TIME));  
                        }
                }
        }
        void create_lower_task(int id)
        {
                std::thread temp(digger_lower,id);
                thread_lower_handles[id] = temp.native_handle();
                temp.detach();
        }
        void create_upper_task(int id)
        {
                std::thread temp(digger_upper,id);
                thread_upper_handles[id] = temp.native_handle();
                temp.detach();
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
                for (auto& handle : thread_lower_handles)
                {
                        pthread_cancel(handle);
                }
                for (auto& handle : thread_upper_handles)
                {
                        pthread_cancel(handle);
                }
        }

        void take_input()
        {
                create_lower_task(0);
                create_upper_task(0);

                int id_lower = 1;
                int id_upper = 1;
                bool deleted = false;

                shovel1 = new std::mutex();
                shovel2 = new std::mutex();
                shovel3 = new std::mutex();
                
                semShovels.push(shovel1);
                semShovels.push(shovel2);
                semShovels.push(shovel3);

                semSoilHeap.lock();

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
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        
                        if (c == 'i') 
                        {
                                if (id_lower<MAX_NUMBER_OF_WORKERS){
                                        create_lower_task(id_lower);
                                        id_lower++;
                                } else {std::cerr<<"too many workers\n";
                                        id_lower = MAX_NUMBER_OF_WORKERS-1;
                                }
                                
                        }
                        if (c == 'I') 
                        {
                                if (id_upper<MAX_NUMBER_OF_WORKERS){
                                        create_upper_task(id_upper);
                                        id_upper++;
                                } else {std::cerr<<"too many workers\n";
                                        id_upper = MAX_NUMBER_OF_WORKERS-1;
                                }
                                
                        }

                        if (c == 'o')
                        { 
                                if(id_lower>0){
                                        if (id_lower !=0) id_lower--;
                                        delete_lower_task(id_lower);
                                } else {
                                        std::cerr<<"all workers are stopped\n";
                                }
                        }
                        if (c == 'O')
                        { 
                                if(id_upper>0){
                                        if (id_upper !=0) id_upper--;
                                        delete_upper_task(id_upper);
                                } else {
                                        std::cerr<<"all workers are stopped\n";
                                }
                        }
                        if (c == 'E')
                        {       
                                if ((id_lower+id_upper)>0) {
                                        delete_all_tasks();
                                        id_lower = 0;
                                        id_upper = 0;
                                }
                                strcpy(ptr->companies[counter].name,"");
                                ptr->companies[counter].work_done = 0;
                        }
                        

                } while((c!='q'));
                
                // 
                /* restore the former settings */
                tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
        }
}

void CreateTasks()
{
        std::thread id1(diggers::take_input);
        id1.join();
}

int main(int argc, char* argv[])
{

    
    for (auto & company : ptr->companies)
    {   
        if (company.work_done == 0)
        {
           strcpy(company.name,argv[1]);
           break;
        }
        counter ++;
    }

    CreateTasks();   
}