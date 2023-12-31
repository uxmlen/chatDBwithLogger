#include <iostream>
#include <mutex>
#include <stdexcept>

#include "logger.h"

Logger::Logger()
{
    file_.open("log.txt", std::ios::out | std::ios::in | std::ios::trunc);
    if (!file_.is_open()) {
        throw std::runtime_error("error: open file");
    }
}

Logger::~Logger() { file_.close(); }

void Logger::write(const std::string& receiver, const std::string& sender,
    const std::string& content)
{
    std::unique_lock<std::shared_mutex> l(sh_, std::try_to_lock);
    file_.seekp(0, std::ios::end);
    if (l.owns_lock()) {
        file_ << "to " << receiver << ". by " << sender
              << ". messages: " << content << std::endl;
    }
}

std::string Logger::read(int line)
{
    std::string str;
    std::shared_lock l(sh_);
    std::ifstream file_("log.txt");

    file_.clear();
    file_.seekg(0, std::ios::beg);
    int i;
    for (i = 0; i < line; ++i) {
        std::getline(file_, str);
    }
    return str;
}
