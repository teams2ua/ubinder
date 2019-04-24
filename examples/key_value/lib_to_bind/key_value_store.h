#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

namespace key_value_store {

class Storage {
public:
    void add(const std::string& key, const std::string& value);
    // return value or throw
    std::string get(const std::string& key);
private:
    std::mutex _lock;
    std::unordered_map<std::string, std::string> _map;
};

}