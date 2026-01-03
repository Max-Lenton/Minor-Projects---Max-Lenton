#include <cmath>
#include <iostream>
#include <string>

int main(int argc, char* argv[]){
     // outputs if the inncorrect number of arguments are sent 
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <number1> <operator> <number2>" << std::endl;
        std::cerr << "Example: " << argv[0] << " 5 + 3" << std::endl;
        std::cerr << "Supported operators: +, -, *, /, %" << std::endl;
        return 1;
    }
    double num1 = atof(argv[1]);
    std::string op = argv[2];
    double num2 = atof(argv[3]);

    double result = 0;

    bool isValid = true; 

    if (op == "+") {
        result = num1 + num2;
    }
    else if (op == "-"){
        result = num1 - num2;
    }
    else if (op == "*"||op == "x"){
        result = num1 * num2;
    }
    else if (op == "/"){
        result = num1 / num2;
    }
    else if (op == "%"){
        result = static_cast<int>(num1) % static_cast<int>(num2);
    }
    if (isValid) {
        std::cout << num1 << " " << op << " " << num2 << " = " << result << std::endl;
    }
    return 0;
}