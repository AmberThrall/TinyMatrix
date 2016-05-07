#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    float a_data[2][3] = {
        {1, 2, 3},
        {4, 5, 6},
    };

    Matrix<float, 2,3> a = Matrix<float,2,3>(a_data);
    std::cout << "a = \n" << a;
    std::cout << "aT = \n" << a.Transpose();
    std::cout << "(aT)T = \n" << a.Transpose().Transpose();

    float b_data[3][3] = {
        { 3, 5, 2 },
        { 7, 2, 9 },
        { 1, 0, 11 }
    };

    Matrix<float, 3,3> b = Matrix<float,3,3>(b_data);
    std::cout << "b = \n" << b;
    std::cout << "bT = \n" << b.Transpose();
}
