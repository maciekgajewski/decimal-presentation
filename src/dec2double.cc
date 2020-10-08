#include <iostream>
#include <iomanip>

int main() {

    std::cout << std::setprecision(20) << 0.3 << std::endl;
    std::cout << std::setprecision(20) << 0.1 + 0.2 << std::endl;
    std::cout << std::setprecision(20) << 123.456 << std::endl;
    std::cout << std::setprecision(20) <<std::fixed <<  987654321E10 << std::endl;
    std::cout << std::setprecision(20) << 0.07 << std::endl;

}
