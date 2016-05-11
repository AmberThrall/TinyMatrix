#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    float a_data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    auto a = Matrix<float,3,3>(a_data);
    std::cout << "a = \n" << a;
    std::cout << "det(a) = " << a.Determinant() << std::endl;

    float b_data[3][3] = {
        { 3, 5, 2 },
        { 7, 2, 9 },
        { 1, 0, 11 }
    };

    auto b = Matrix<float,3,3>(b_data);
    std::cout << "b = \n" << b;
    std::cout << "det(b) = " << b.Determinant() << std::endl;
}
