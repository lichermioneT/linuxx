#include <iostream>
#include "operations.h"

int main() {
    double num1, num2;
    char op;

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter operator (+, -, *, /): ";
    std::cin >> op;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    try {
        switch(op) {
            case '+':
                std::cout << "Result: " << add(num1, num2) << std::endl;
                break;
            case '-':
                std::cout << "Result: " << subtract(num1, num2) << std::endl;
                break;
            case '*':
                std::cout << "Result: " << multiply(num1, num2) << std::endl;
                break;
            case '/':
                std::cout << "Result: " << divide(num1, num2) << std::endl;
                break;
            default:
                std::cerr << "Invalid operator!" << std::endl;
                return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}