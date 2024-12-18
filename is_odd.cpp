#include <iostream>

bool is_even(int number) {
    return number % 2 == 0;
}

int main() {
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;

    if (is_even(number)) {
        std::cout << number << " is even." << std::endl;
    } else {
        std::cout << number << " is odd." << std::endl;
    }

    return 0;
}
