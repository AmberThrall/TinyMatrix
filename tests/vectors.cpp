#define TINYMATRIX_CPP11
#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    float v1[3][1] = {{1},{2},{3}};
    auto V1 = Vector<float, 3>(v1);
    auto V2 = Vector<float, 3>(2);
    std::cout << "V1 = \n" << V1;
    std::cout << "V2 = \n" << V2;
    std::cout << "V1 dot V2 = " << DotProduct<float,3>(V1,V2) << std::endl;
    std::cout << "V1 x V2 = \n" << CrossProduct<float>(V1,V2);
}
