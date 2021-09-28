#include <iostream>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>


void take_input()
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
                printf("%c ",c);
        } while(c!='q');

        /* restore the former settings */
        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
}

int main()
{
        std::thread id1(take_input);
        id1.join();
}