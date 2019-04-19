#include "key_value_store.h"

namespace key_value_store {

void Storage::add(const std::string& key, const std::string& value) {
    _map[key] = value;
}

std::string Storage::get(std::string& key) {
    return _map[key];
}


}