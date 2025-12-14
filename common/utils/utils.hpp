#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>

class Utils {
public:
    static std::string removeIPv4Prefix(const std::string& addr) {
        const std::string prefix = "ipv4:";
        if (addr.rfind(prefix, 0) == 0) { 
            return addr.substr(prefix.size());
        }
        return addr;
    }
};

#endif