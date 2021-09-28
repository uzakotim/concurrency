#include "main.h"



namespace diggers
{

        void digger(int id)
        {
                while(1)
                {
                        std::cout<<"digger "<<id<<'\n';
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
        }
        void create_task(int id)
        {
                std::thread temp(digger,id);
                thread_handles[id] = temp.native_handle();
                temp.detach();
        }


        void delete_task(int id)
        {
                pthread_cancel(thread_handles[id]);
        }
        void delete_all_tasks()
        {
                for (auto& handle : thread_handles)
                {
                        pthread_cancel(handle);
                }
        }

        void take_input()
        {

                int id = 0;



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
                        printf("%c ",c);
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        if (c == 'i') 
                        {
                                if (id<MAX_NUMBER_OF_WORKERS){
                                        create_task(id);
                                        id++;
                                } else {std::cerr<<"too many workers\n";
                                        id = MAX_NUMBER_OF_WORKERS-1;
                                }
                                
                        }
                        if (c == 'o')
                        { 
                                if(id>0){
                                        if (id !=0) id--;
                                        delete_task(id);
                                } else {
                                        std::cerr<<"all workers are stopped\n";
                                }
                        }
                        if (c == 'E')
                        {
                                delete_all_tasks();
                                id = 0;
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