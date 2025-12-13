#include <iostream>
#include <lamport.hpp>

int main(){
    Lamport lamport;

    std::cout << "I am a compressor" << lamport.curTimestamp() << std::endl;

    return 0;
}