#define TINYMATRIX_CPP11
#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    float v1[3] = {1,2,3};
    auto V1 = Vector<float, 3>(v1);
    auto V2 = Vector<float, 3>(2);
    std::cout << "V1 = \n" << V1;
    std::cout << "V2 = \n" << V2;
    std::cout << "V1 dot V2 = " << V1.Dot(V2) << std::endl;
    std::cout << "V1 cross V2 = \n" << CrossProduct<float>(V1, V2);
    std::cout << "||V1|| = " << V1.Magnitude() << std::endl;
    std::cout << "v1/||v1|| = \n" << V1.Unit();
}
