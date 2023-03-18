#include <iostream>
#include <CircularList.hpp>

int main() {
    CircularList<int> list;

    list.push(1);
    list.push(2);

    std::cout << list << std::endl;

    list.push(-5);

    std::cout << list << std::endl;

    std::cout << list.find(-5)->value << std::endl;

    return 0;
}
