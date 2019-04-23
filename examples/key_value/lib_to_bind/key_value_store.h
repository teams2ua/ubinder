#pragma once

#include <string>
#include <unordered_map>

namespace key_value_store {

class Storage {
public:
    void add(const std::string& key, const std::string& value);
    // return value or throw
    std::string get(std::string& key);
private:
    std::unordered_map<std::string, std::string> _map;
};

}