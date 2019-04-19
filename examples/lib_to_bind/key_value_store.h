#pragma once

#include <string>
#include <unordered_map>

#  ifndef KET_VALUE_EXPORT
#    ifdef key_value_storage_EXPORTS
        /* We are building this library */
#      define KEY_VALUE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define KEY_VALUE_EXPORT __declspec(dllimport)
#    endif
#  endif

namespace key_value_store {

class KEY_VALUE_EXPORT Storage {
public:
    void add(const std::string& key, const std::string& value);
    // return value or throw
    std::string get(std::string& key);
private:
    std::unordered_map<std::string, std::string> _map;
};

}