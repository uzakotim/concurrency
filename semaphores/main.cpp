#include "main.h"



namespace diggers
{
        void digger_lower(int id)
        {       
                while(1)
                {       
                        shovel.lock();
                        std::cout<<"lower digger "<<id<<": working \n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(WORK_TIME));
                        
                        semSoilHeap.unlock();
                        shovel.unlock();
                        std::cout<<"lower digger "<<id<<": resting\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(BREAK_TIME));

                }
        }
        void digger_upper(int id)
        {
                while(1)
                {
                        if(semSoilHeap.try_lock())
                        {       
                                shovel.unlock();
                                std::cout<<"upper digger "<<id<<": working \n";
                                std::this_thread::sleep_for(std::chrono::milliseconds(WORK_TIME));
                                semSoilHeap.unlock();
                                shovel.unlock();
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

                int id_lower = 0;
                int id_upper = 0;
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
                        }
                        

                } while(c!='q');

                /* restore the former settings */
                tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
        }
}


int main()
{       
        std::thread id1(diggers::take_input);
        id1.join();
}