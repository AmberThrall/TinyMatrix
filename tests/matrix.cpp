#include "../TinyMatrix.h"

using namespace TinyMatrix;

int main() {
    Matrix<float,3,3> mat1; 
    std::cout << "mat1 = \n" << mat1;
    Matrix<float,4,4> mat2 = Matrix<float,4,4>::Identity();
    std::cout << "mat2 = \n" << mat2;
    std::cout << "mat2[2] = \n" << mat2.GetColumn(2);
    std::cout << "Row 2 = \n" << mat2.GetRow(2);

    float mat3_data[2][3] = {
        {1, 2, 3},
        {4, 5, 6},
    };

    Matrix<float, 2,3> mat3 = Matrix<float,2,3>(mat3_data);
    std::cout << "mat3 = \n" << mat3;
    std::cout << "mat3(1,0) = " << mat3(1,0) << std::endl;
}
