#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    float a_data[3][4] = {
        {1, 2, -1, -4},
        {2, 3, -1, -11},
        {-2, 0, -3, 22}
    };

    auto a = Matrix<float,3,4>(a_data);
    std::cout << "a = \n" << a;

    auto a_reduced = a.RowReduce();
    std::cout << "a_reduced = \n" << a_reduced;
}
