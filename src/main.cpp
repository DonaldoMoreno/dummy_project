#include <iostream>
#include <string>
#include <vector>

// Simple calculator class
class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }
    
    int subtract(int a, int b) {
        return a - b;
    }
    
    int multiply(int a, int b) {
        return a * b;
    }
    
    double divide(int a, int b) {
        if (b == 0) {
            std::cerr << "Error: Division by zero!" << std::endl;
            return 0.0;
        }
        return static_cast<double>(a) / b;
    }
};

int main() {
    Calculator calc;
    
    std::cout << "Simple Calculator Demo" << std::endl;
    std::cout << "======================" << std::endl;
    
    // Test addition
    std::cout << "10 + 5 = " << calc.add(10, 5) << std::endl;
    
    // Test subtraction
    std::cout << "10 - 5 = " << calc.subtract(10, 5) << std::endl;
    
    // Test multiplication
    std::cout << "10 * 5 = " << calc.multiply(10, 5) << std::endl;
    
    // Test division
    std::cout << "10 / 5 = " << calc.divide(10, 5) << std::endl;
    
    // Test division by zero
    std::cout << "10 / 0 = " << calc.divide(10, 0) << std::endl;
    
    return 0;
}
