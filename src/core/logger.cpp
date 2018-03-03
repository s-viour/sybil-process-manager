#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "logger.h"

using namespace sybil;

logger* logger::_logger_instance = nullptr;

logger* logger::get() {
    if (!_logger_instance) {
        _logger_instance = new logger;
    }
    return _logger_instance;
}

void logger::write(std::string message, logger_level level) {
    if (level > 3 || level < 0) {
        write("log level out of range", DEBUG);
        throw std::out_of_range("log level out of range");
    }
    //timestamp creation
    auto current_time = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(current_time);
    auto printable_time = ctime(&tt);
    std::string timestamp = "[";
    timestamp += printable_time;
    if (!timestamp.empty() && timestamp[timestamp.length()-1] == '\n') {
        timestamp.erase(timestamp.length()-1);
    }
    timestamp += "] ";
    //end timestamp creation
    if (level >= _log_level) {
        std::cout << timestamp << _prefixes.at(level) << message << std::endl;
    }
    std::ofstream file("sybil.log", std::ios::app);
    if (file.is_open()) {
        file << timestamp << _prefixes.at(level) << message << std::endl;
    }
    file.close();
}

void logger::set_level(logger_level level) {
    if (level > 3 || level < 0) {
        write("log level out of range", DEBUG);
        throw std::out_of_range("log level out of range");
    }
    else {
        _log_level = level;
        write("set level to " + _prefixes.at(level), DEBUG);
    }
}

std::vector<std::string> logger::latest(int count) {
    std::ifstream file("sybil.log");
    std::string line_buffer;
    std::vector<std::string> lines;
    if (file.is_open()) {
        while (std::getline(file, line_buffer)) {
            lines.push_back(line_buffer);
        };
    }
    file.close();
    std::vector<std::string> latest;
    int current_count = 0;
    for (auto iterator : lines) {
        if (current_count >= count) {
            break;
        }
        latest.push_back(iterator);
        current_count++;
    }
    std::reverse(latest.begin(), latest.end());
    return latest;
}

std::string logger::print_latest(int count) {
    auto v_latest = latest(count);
    for (auto iterator : v_latest) {
        std::cout << iterator << std::endl;
    }
}