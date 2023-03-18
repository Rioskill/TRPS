#include <iostream>
#include <CircularList.hpp>

int main() {
    CircularList<int> list;

    std::cout << list << std::endl;

    list.push(10);
    list.push(2);

    std::cout << list << std::endl;

    list.push(-5);

    std::cout << list << std::endl;

    std::cout << list.find(-5)->value << std::endl;

    list.bubble_sort();

    std::cout << list << std::endl;

    std::cout << list.find_prev(-5)->value << std::endl;

    list.remove(-5);
    std::cout << list << std::endl;

    list.remove(2);
    std::cout << list << std::endl;

    list.remove(10);
    std::cout << list << std::endl;

    return 0;
}
