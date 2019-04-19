#include <thread>
#include <iostream>
#include <vector>

#include "key_value_store.h"

int main() {
    key_value_store::Storage x;
    x.add("hello", "world");
    std::cout << x.get(std::string("hello")) << std::endl;
    return 0;
}