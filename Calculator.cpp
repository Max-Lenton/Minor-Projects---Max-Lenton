#include <cmath>
#include <iostream>
#include <string>

int Calculator(){
    std::cout << "Enter the Number of Operands: ";
    int x;
    std::cin >> x;
    int operands[x];

}
int main(){
    // Print User Instructions 
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Please select the function you wish to use:" << std::endl;
    std::cout << "1. Calculator" << std::endl;
    std::cout << "2. Bits & Bytes" << std::endl;
    std::cout << "3. Statistics" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

