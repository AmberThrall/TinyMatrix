#define TINYMATRIX_NO_UTF8
#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    float a_data[3][3] = {
        {1, 2, -1},
        {2, 3, -1},
        {-2, 0,-3}
    };
    float v_data[3][1] = {-4, -11, 22};

    auto a = Matrix<float,3,3>(a_data);
    auto v = Vector<float,3>(v_data);
    auto av = Matrix<float,3,4>::Augmented<3,1>(a, v);

    std::cout << "a = \n" << a;
    std::cout << "v = \n" << v;
    std::cout << "[a v] = \n" << av;

    auto a_reduced = av.RowReduce();
    std::cout << "a_reduced = \n" << a_reduced;

    float b_data[2][2] = {
        {2, 3},
        {4, 7},
    };
    auto b = Matrix<float,2,2>(b_data);
    auto bi = Matrix<float,2,4>::Augmented<2,2>(b, Matrix<float,2,2>::Identity());
    std::cout << "[b I] = \n" << bi;
    std::cout << "RowReduce([b I]) = \n" << bi.RowReduce();
}
