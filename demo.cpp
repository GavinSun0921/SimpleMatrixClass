#include <iostream>
#include "Matrix.h"

int main() {
    Matrix<double> x(3, 4), y(4, 5);
    for (int i = 0; i < 3; i++) for (int j = 0; j < 4; j++) x.at(i, j) = i+j;
    for (int i = 0; i < 4; i++) for (int j = 0; j < 5; j++) y.at(i, j) = i+j;
    std::cout << x << std::endl;
    std::cout << y << std::endl;

    Matrix<double> z = x * y;
    std::cout << z << std::endl;

    Matrix<double> z2 = z.pow(2);
    std::cout << z2 << std::endl;

    std::cout << transposition(z2) << std::endl;

    return 0;
}
