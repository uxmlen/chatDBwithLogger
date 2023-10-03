#pragma once

#include <fstream>
#include <shared_mutex>
#include <string>

class Logger {
public:
    Logger();
    ~Logger();

    void write(const std::string& receiver, const std::string& sender,
        const std::string& content);
    std::string read(int line);

private:
    std::fstream file_;
    std::shared_mutex sh_;
};
