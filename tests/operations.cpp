#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    float a_data[2][3] = {
        {1, 2, 3},
        {4, 5, 6},
    };

    Matrix<float, 2,3> a = Matrix<float,2,3>(a_data);
    std::cout << "a = \n" << a;
    std::cout << "a + a = \n" << a + a;
    std::cout << "a - a = \n" << a - a;
    std::cout << "3a = \n" << 3*a;

    Matrix<float, 2,3> z = a;
    z -= a;
    std::cout << "z = \n" << z;
    std::cout << "z == a: " << (z == a) << std::endl;
    std::cout << "z == z: " << (z == z) << std::endl;

    Matrix<float, 3,2> unity(1);
    std::cout << "unity = \n" << unity;
    std::cout << "a * unity = \n" << a * unity;
    std::cout << "a * aT = \n" << a * a.Transpose();
}
