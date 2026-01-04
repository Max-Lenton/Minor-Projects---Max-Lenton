#include <cmath>
#include <iostream>

int main(){
    double x = 0, y = 1, z; 
    while (x < 900000){ 
        std::cout << x << std::endl;
        z = x + y; 
        x = y; 
        y = z;
    }
}