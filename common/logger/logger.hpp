#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>

#define LOG_ERRO(...) Logger::log_erro(__func__, __VA_ARGS__)
#define LOG_INFO(...) Logger::log_info(__func__, __VA_ARGS__)
#define LOG_DBUG(...) Logger::log_dbug(__func__, __VA_ARGS__)

class Logger {
public:
    /*
        ERRO
    */
    template<typename... Args>
    static void log_erro(std::string function, Args... args){
        std::ostringstream oss;
        (oss << ... << args);
        std::cout << "[ERRO][" << function << "] " << " | "<< getTime() << " | " << oss.str() << std::endl;
    }


    /*
        INFO
    */
    template<typename... Args>
    static void log_info(std::string function, Args... args){
        std::ostringstream oss;
        (oss << ... << args);
        std::cout << "[INFO][" << function << "] " << " | "<< getTime() << " | " << oss.str() << std::endl;
    }

    /*
        DBUG
    */
    template<typename... Args>
    static void log_dbug(std::string function, Args... args){
        std::ostringstream oss;
        (oss << ... << args);
        std::cout << "[DBUG][" << function << "] " << " | "<< getTime() << " | " << oss.str() << std::endl;
    }

private: 
    static std::string getTime(){
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);

        std::tm localTime = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&localTime, "%H:%M:%S");

        return oss.str(); // retorna como string
    }

};

#endif