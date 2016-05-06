// LEGAL SHIT HERE

#pragma once

#include <cstring>
#include <iostream>

namespace TinyMatrix {
    template<class T, size_t M, size_t N>
    class Matrix {
    public:
        Matrix() {
            for (size_t i = 0; i < M; ++i) {
                for (size_t j = 0; j < N; ++j)
                    this->data[i][j] = 0;
            }
        }
        Matrix(T data[M][N]) {
            for (size_t i = 0; i < M; ++i) {
                for (size_t j = 0; j < N; ++j)
                    this->data[i][j] = data[i][j];
            }
        }

        Matrix<T,1,N> GetRow(int r) {
            Matrix<T,1,N> ret;
            for (size_t i = 0; i < N; ++i)
                ret(0,i) = this->data[r][i];
            return ret;
        }

        Matrix<T,M,1> GetColumn(int c) {
            Matrix<T,M,1> ret;
            for (size_t i = 0; i < M; ++i)
                ret(i,0) = this->data[i][c];
            return ret;
        }

        void SetRow(int r, Matrix<T,1,N> row) {
            for (size_t i = 0; i < N; ++i)
                this->data[r][i] = row(0,i);
        }

        void SetColumn(int c, Matrix<T,M,1> column) {
            for (size_t i = 0; i < M; ++i)
                this->data[i][c] = column(i,0);
        }

        T * Raw() {
            return (T*)this->data;
        }

        bool IsSquare() {
            return (M == N);
        }

        static Matrix<T,M,M> Identity() {
            Matrix<T,M,M> ret;
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c) {
                    if (r == c)
                        ret(r,c) = 1;
                    else
                        ret(r,c) = 0;
                }
            }
            return ret;
        }

        T& operator() (size_t r, size_t c) { return this->data[r][c]; }
        const T& operator() (size_t r, size_t c) const { return this->data[r][c]; }

        friend std::ostream& operator<<(std::ostream& os, const Matrix<T,M,N> mat) {
            for (size_t r = 0; r < M; ++r) {
                os << "[ ";
                for (size_t c = 0; c < N; ++c)
                    os << mat(r,c) << " ";
                os << "] " << std::endl;
            }
            return os;
        }
    private:
        T data[M][N];
    };

//    template<typename T, size_t N>
 //   using SquareMatrix = Matrix<T, N, N>;
  //  template <typename T, size_t N>
   // using Vector = Matrix<T, 1, N>;
}
