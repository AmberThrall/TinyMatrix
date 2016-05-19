#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    float v1[3] = {1,2,3};
    auto V1 = Vector<float, 3>(v1);
    auto V2 = Vector<float, 3>(2);
    auto j = Vector<float, 3>::Basis(1);
    std::cout << "V1 = \n" << V1;
    std::cout << "V2 = \n" << V2;
    std::cout << "V1 dot V2 = " << V1.Dot(V2) << std::endl;
    std::cout << "V1 cross V2 = \n" << CrossProduct<float>(V1, V2);
    std::cout << "||V1|| = " << V1.Magnitude() << std::endl;
    std::cout << "v1/||v1|| = \n" << V1.Unit();
    std::cout << "V1 + V2 = \n" << V1 + V2;
    std::cout << "j = \n" << j;

    Matrix<float, 3, 3> matrix = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    std::cout << "matrix * v1 = \n" << matrix * V1;

    // C++11's initializer_list:
    Vector<float, 4> V3 = {1, 2, 3, 4};
    std::cout << "V3 = \n" << V3;
}
