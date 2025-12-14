#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>

class Logger {
public:
    static void log_erro(std::string erromsg);
    static void log_info(std::string infomsg);
    static void log_dbug(std::string dbugmsg);
};

#endif