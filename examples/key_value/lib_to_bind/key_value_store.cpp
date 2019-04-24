#include <chrono>
#include <thread>

#include "key_value_store.h"

namespace key_value_store {

using namespace std::chrono_literals;

void Storage::add(const std::string& key, const std::string& value) {
    std::this_thread::sleep_for(1s);
    std::lock_guard<std::mutex> guard(_lock);
    _map[key] = value;
}

std::string Storage::get(const std::string& key) {
    std::this_thread::sleep_for(1s);
    std::lock_guard<std::mutex> guard(_lock);
    return _map[key];
}


}