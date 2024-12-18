#include <iostream>

bool is_odd(int number) {
    return number % 2 != 0;
}

int main() {
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;

    if (is_odd(number)) {
        std::cout << number << " is odd." << std::endl;
    } else {
        std::cout << number << " is even." << std::endl;
    }

    return 0;
}





"test"