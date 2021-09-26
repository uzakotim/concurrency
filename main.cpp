#include <iostream>
#include <thread>

void hello()
{
    while(1)
    {
        std::cout<<"Hello,world!\n";
    }
}

int main()
{
    std::thread t(hello);
    t.join();
}