#include "logger.hpp"

#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>

std::string getTime(){
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm localTime = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%H:%M:%S");

    return oss.str(); // retorna como string
}

void Logger::log_erro(std::string erromsg){
    std::cerr << "ERRO: " << " | "<< getTime() << "|" << erromsg << std::endl;
}

void Logger::log_info(std::string infomsg){
    std::cout << " | "<< getTime() << "|" << infomsg << std::endl;
}

void Logger::log_dbug(std::string dbugmsg){
    std::cout << "DEBUG: " << " | "<< getTime() << "|" << dbugmsg << std::endl;
}