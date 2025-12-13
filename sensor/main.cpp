#include <iostream>
#include <lamport.hpp>

int main(){
    Lamport lamport;

    std::cout << "I am a sensor" << lamport.curTimestamp() << std::endl;

    return 0;
}