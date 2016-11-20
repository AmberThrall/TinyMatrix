/*
    TinyMatrix.h - zlib - Amber Thrall <amber.rose.thrall@gmail.com>

ABOUT:
    All-in-one C++11 MxN matrix header file. Includes several matrix operations such as: determinant, cofactor, inverse,
    adjugate, Gauss-Jordan elimination, etc. Also includes all basic operators such as addition, subtraction, scalars and multiplication.
    All matrices are stored in a single templated struct passing in a type for the data, and the dimension. Also includes an optional
    implementation of vectors, treating them as Nx1 matrices.

LICENSE: (zlib)
    Copyright (c) 2016 Amber Thrall

    This software is provided 'as-is', without any express or implied warranty. In no
    event will the authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose, including commercial applications,
    and to alter it and redistribute it freely, subject to the following restrictions:

    1.  The origin of this software must not be misrepresented; you must not claim that you wrote the original
        software. If you use this software in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2.  Altered source versions must be plainly marked as such, and must not be misrepresented as being the
        original software.
    3.  This notice may not be removed or altered from any source distribution.

DEFINES:
    TINYMATRIX_NO_CPP11
        Disables all features which require C++11 compilers and attempts to use an alternate representation. This includes things
        such as: using std::initializer_list to initialize matrices and vectors, use of std::enable_if to only define functions
        which require a square matrix on square matrices (without C++11 it will throw an exception), and defining an alias for
        NxN matrices (SquareMatrix).

    TINYMATRIX_NO_VECTORS
        Disables the vector class. The vector struct is represented as an Nx1 matrix and includes typical vector operations such as:
        dot product, cross product, and magnitude. As well as all other matrix operations for non-square matrices.

    TINYMATRIX_NO_COUT
        Disables the std::ostream operator for use with std::cout/std::cerr, also removes the inclusion of the header <iostream>.

    TINYMATRIX_NO_UTF8
      Disables UTF-8 output for std::cout. Requires TINYMATRIX_NO_COUT to not be defined.
*/

#pragma once

#include <cstring>
#include <cmath>
#include <cstdarg>

#ifndef TINYMATRIX_NO_CPP11
#include <type_traits>
#include <initializer_list>
#endif

#ifndef TINYMATRIX_NO_COUT
#include <iostream>
#endif

namespace TinyMatrix {
    template<std::size_t> struct _int2type{};

    template<typename T, size_t M, size_t N>
    struct Matrix {
    public:
        Matrix(T val = 0) {
            for (size_t i = 0; i < M; ++i) {
                for (size_t j = 0; j < N; ++j)
                    this->data[i][j] = val;
            }
        }
        Matrix(const Matrix<T,M,N>& other) {
            memcpy(this->data, other.data, sizeof(data));
        }
        Matrix(T data[M][N]) {
            for (size_t i = 0; i < M; ++i) {
                for (size_t j = 0; j < N; ++j)
                    this->data[i][j] = data[i][j];
            }
        }
        #ifndef TINYMATRIX_NO_CPP11
        Matrix(std::initializer_list<T> list) {
            size_t r = 0, c = 0;
            for (auto it = std::begin(list); it != std::end(list) && r < M; ++it) {
                this->data[r][c] = *it;
                c++;
                if (c == N) {
                    c = 0;
                    r++;
                    if (r == M)
                        break;
                }
            }
        }
        #endif

        template<size_t P, size_t R>
        Matrix<T,P,R> Resize() {
            Matrix<T,P,R> ret;
            for (size_t r = 0; r < M && r < P; ++r) {
                for (size_t c = 0; c < N && c < R; ++c)
                    ret(r,c) = this->data[r][c];
            }
            return ret;
        }

        template<size_t P, size_t R>
        Matrix<T,P,R> SubMatrix(size_t i, size_t j) {
            Matrix<T,P,R> ret;
            for (size_t r = 0; r < P; ++r) {
                for (size_t c = 0; c < R; ++c)
                    ret(r,c) = this->data[r + i][c + j];
            }
            return ret;
        }

        Matrix<T,1,N> GetRow(size_t r) {
            Matrix<T,1,N> ret;
            for (size_t i = 0; i < N; ++i)
                ret(0,i) = this->data[r][i];
            return ret;
        }

        Matrix<T,M,1> GetColumn(size_t c) {
            Matrix<T,M,1> ret;
            for (size_t i = 0; i < M; ++i)
                ret(i,0) = this->data[i][c];
            return ret;
        }

        void SetRow(size_t r, T row[N]) {
            for (size_t i = 0; i < N; ++i)
                this->data[r][i] = row[i];
        }

        void SetRow(size_t r, Matrix<T,1,N> row) {
            for (size_t i = 0; i < N; ++i)
                this->data[r][i] = row(0,i);
        }

        void SetColumn(size_t c, T column[M]) {
            for (size_t i = 0; i < M; ++i)
                this->data[i][c] = column[i];
        }

        void SetColumn(size_t c, Matrix<T,M,1> column) {
            for (size_t i = 0; i < M; ++i)
                this->data[i][c] = column(i,0);
        }

        Matrix<T,M,N> SwapRows(size_t i1, size_t i2) {
            Matrix<T,M,N> ret(*this);
            for (size_t j = 0; j < N; ++j) {
                ret(i1,j) = this->data[i2][j];
                ret(i2,j) = this->data[i1][j];
            }
            return ret;
        }

        Matrix<T,M,N> SwapColumns(size_t j1, size_t j2) {
            Matrix<T,M,N> ret(*this);
            for (size_t i = 0; i < M; ++i) {
                ret(i,j1) = this->data[i][j2];
                ret(i,j2) = this->data[i][j1];
            }
            return ret;
        }

        Matrix<T,M,N> MultiplyRow(T value, size_t i) {
            Matrix<T,M,N> ret(*this);
            for (size_t j = 0; j < N; ++j) {
                ret(i,j) *= value;
            }
            return ret;
        }

        Matrix<T,M,N> MultiplyColumn(T value, size_t j) {
            Matrix<T,M,N> ret(*this);
            for (size_t i = 0; i < M; ++i) {
                ret(i,j) *= value;
            }
            return ret;
        }

        // i2 -> v * i1 + i2
        Matrix<T,M,N> AddRowToRow(T v, size_t i1, size_t i2) {
            Matrix<T,M,N> ret(*this);
            for (size_t j = 0; j < N; ++j) {
                ret(i2,j) = v * this->data[i1][j] + this->data[i2][j];
            }
            return ret;
        }

        // j2 -> v * j1 + j2
        Matrix<T,M,N> AddColumnToColumn(T v, size_t j1, size_t j2) {
            Matrix<T,M,N> ret(*this);
            for (size_t i = 0; i < M; ++i) {
                ret(i,j2) = v * this->data[i][j1] + this->data[i][j2];
            }
            return ret;
        }

        Matrix<T,M-1,N> RemoveRow(size_t i) {
            Matrix<T,M-1,N> ret;

            size_t indexR = 0;
            for (size_t r = 0; r < M; ++r) {
                if (r == i)
                    continue;

                for (size_t c = 0; c < N; ++c)
                    ret(indexR, c) = this->data[r][c];
                indexR++;
            }

            return ret;
        }

        Matrix<T,M,N-1> RemoveColumn(size_t j) {
            Matrix<T,M,N-1> ret;

            for (size_t r = 0; r < M; ++r) {
                size_t indexC = 0;
                for (size_t c = 0; c < N; ++c) {
                    if (c == j)
                        continue;
                    ret(r, indexC++) = this->data[r][c];
                }
            }

            return ret;
        }

        Matrix<T,M-1,N-1> RemoveRowAndColumn(size_t i, size_t j) {
            Matrix<T,M-1,N-1> ret;

            size_t indexR = 0;
            for (size_t r = 0; r < M; ++r) {
                if (r == i)
                    continue;

                size_t indexC = 0;
                for (size_t c = 0; c < N; ++c) {
                    if (c == j)
                        continue;
                    ret(indexR, indexC++) = this->data[r][c];
                }
                indexR++;
            }

            return ret;
        }

        Matrix<T,M,N> RowReduce() {
            Matrix<T,M,N> ret(*this);

            size_t lead = 0;
            for (size_t r = 0; r < M; r++) {
                if (lead >= N)
                    break;

                size_t i = r;
                while (ret(i, lead) == 0) {
                    i++;
                    if (i >= M) {
                        i = r;
                        lead++;
                        if (lead >= N)
                            return ret;
                    }
                }

                ret = ret.SwapRows(i, r);
                if (ret(r, lead) != 0) {
                    ret = ret.MultiplyRow(1/ret(r,lead), r);
                }
                for (size_t i = 0; i < M; ++i) {
                    if (i != r)
                        ret = ret.AddRowToRow(-ret(i,lead), r, i);
                }

                lead++;
            }

            return ret;
        }

        T * Raw() {
            return (T*)this->data;
        }

        bool IsSquare() {
            return (M == N);
        }

        size_t NumRows() {
            return M;
        }

        size_t NumColumns() {
            return N;
        }

        Matrix<T,N,M> Transpose() const {
            Matrix<T,N,M> ret;
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c) {
                    ret(c,r) = this->data[r][c];
                }
            }
            return ret;
        }

        // Square Matrix functions
        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, bool>::type IsSingular()
        #else
        bool IsSingular()
        #endif
        {
            if (!IsSquare())
                return false;
            return (Determinant() == 0);
        }

        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, T>::type Determinant()
        #else
        T Determinant()
        #endif
        {
            if (!IsSquare())
                throw "Determinant of non-square matrix";
            return _DeterminantHelper(*this, _int2type<M>());
        }

        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, T>::type Minor(size_t i, size_t j)
        #else
        T Minor(size_t i, size_t j)
        #endif
        {
            if (!IsSquare())
                throw "Minor of non-square matrix";
            return _DeterminantHelper(RemoveRowAndColumn(i, j), _int2type<M-1>());
        }

        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, T>::type Cofactor(size_t i, size_t j)
        #else
        T Cofactor(size_t i, size_t j)
        #endif
        {
            if (!IsSquare())
                throw "Cofactor of non-square matrix";
            return T(pow(-1.0f, i+j)) * Minor(i, j);
        }

        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, Matrix<T,M,M>>::type Adjugate()
        #else
        Matrix<T,M,M> Adjugate()
        #endif
        {
            if (!IsSquare())
                throw "Adjugate of non-square matrix";

            Matrix<T,M,M> ret;
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < M; ++c)
                    ret(r,c) = Cofactor(r,c);
            }
            return ret.Transpose();
        }

        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, Matrix<T,M,M>>::type Inverse()
        #else
        Matrix<T,M,M> Inverse()
        #endif
        {
            if (!IsSquare())
                throw "Inverse of non-square matrix";

            T detA = Determinant();
            if (detA == 0)
                throw "Inverse of singular matrix";

            return (1 / detA) * Adjugate();
        }

        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, T>::type Trace()
        #else
        T Trace()
        #endif
        {
            if (!IsSquare())
                throw "Trace of non-square matrix";

            T sum = 0;
            for (size_t i = 0; i < M; ++i)
                sum += this->data[i][i];
            return sum;
        }

        // Static functions
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

        template<size_t P, size_t Q>
        static Matrix<T,M,P+Q> Augmented(Matrix<T,M,P> a, Matrix<T,M,Q> b) {
            Matrix<T,M,P+Q> ret;
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < P; ++c)
                    ret(r,c) = a(r,c);
                for (size_t c = 0; c < Q; ++c)
                    ret(r,P+c) = b(r,c);
            }
            return ret;
        }

        // Operators
        Matrix<T,M,N>& operator=(const Matrix<T,M,N>& other) { // copy
            if (this != &other) {
                for (size_t r = 0; r < M; ++r) {
                    for (size_t c = 0; c < N; ++c)
                        this->data[r][c] = other(r,c);
                }
            }
            return *this;
        }

        friend bool operator==(const Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) {
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c) {
                    if (lhs(r,c) != rhs(r,c))
                        return false;
                }
            }
            return true;
        }
        friend bool operator!=(const Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) { return !(lhs == rhs); }

        T& operator() (size_t r, size_t c) { return this->data[r][c]; }
        const T& operator() (size_t r, size_t c) const { return this->data[r][c]; }

        Matrix<T,M,N>& operator+=(const Matrix<T,M,N>& rhs) {
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c)
                    this->data[r][c] += rhs(r,c);
            }
            return *this;
        }
        friend Matrix<T,M,N> operator+(Matrix<T,M,N> lhs, const Matrix<T,M,N>& rhs) {
            return (lhs += rhs);
        }

        Matrix<T,M,N>& operator-=(const Matrix<T,M,N>& rhs) {
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c)
                    this->data[r][c] -= rhs(r,c);
            }
            return *this;
        }
        friend Matrix<T,M,N> operator-(Matrix<T,M,N> lhs, const Matrix<T,M,N>& rhs) {
            return (lhs -= rhs);
        }

        Matrix<T,M,N>& operator*=(const T& scalar) {
            for (size_t r = 0; r < M; ++r) {
                for (size_t c = 0; c < N; ++c)
                    this->data[r][c] *= scalar;
            }
            return *this;
        }
        friend Matrix<T,M,N> operator*(Matrix<T,M,N> lhs, const T& scalar) {
            return (lhs *= scalar);
        }
        friend Matrix<T,M,N> operator*(const T& scalar, Matrix<T,M,N> rhs) {
            return (rhs *= scalar);
        }

        template<size_t P>
        friend Matrix<T,M,P> operator*(Matrix<T,M,N> lhs, const Matrix<T,N,P>& rhs) {
            Matrix<T,M,P> ret;

            for (size_t r = 0; r < M; ++r) {
                for (size_t c1 = 0; c1 < P; ++c1) {
                    for (size_t c2 = 0; c2 < N; ++c2)
                        ret(r, c1) += lhs(r,c2) * rhs(c2,c1);
                }
            }

            return ret;
        }

        #ifndef TINYMATRIX_NO_COUT
        friend std::ostream& operator<<(std::ostream& os, const Matrix<T,M,N> mat) {
            for (size_t r = 0; r < M; ++r) {
              #ifdef TINYMATRIX_NO_UTF8
                os << "[ ";
              #else
                if (M == 1)
                  os << "[ ";
                else if (r == 0)
                  os << "⎡ ";
                else if (r == M-1)
                  os << "⎣ ";
                else if (M > 1)
                  os << "⎢ ";
              #endif
                for (size_t c = 0; c < N; ++c)
                    os << mat(r,c) << " ";
                #ifdef TINYMATRIX_NO_UTF8
                  os << "] " << std::endl;
                #else
                  if (M == 1)
                    os << "] " << std::endl;
                  else if (r == 0)
                    os << "⎤ " << std::endl;
                  else if (r == M-1)
                    os << "⎦ " << std::endl;
                  else
                    os << "⎥ " << std::endl;
                #endif
            }
            return os;
        }
        #endif
    protected:
        T data[M][N];
    };

    //HACK? for template recursive functions
    template<typename T, size_t M, size_t I>
    T _DeterminantHelper(Matrix<T,M,M> matrix, _int2type<I>) {
        // TODO: Elementary operations to reduce?
        if (M == 0)
            return T(1);
        if (M == 1)
            return matrix(0,0);
        if (M == 2)
            return (matrix(0,0) * matrix(1,1) - matrix(0,1) * matrix(1,0));

        T det;
        float sign = 1;
        for (size_t i = 0; i < M; ++i) {
            det += sign * matrix(0,i) * _DeterminantHelper(matrix.RemoveRowAndColumn(0, i), _int2type<I-1>());
            sign *= -1;
        }

        return det;
    }

    template<typename T, size_t M>
    T _DeterminantHelper(Matrix<T,M,M> matrix, _int2type<0>) {
        return T(1);
    }

#ifndef TINYMATRIX_NO_CPP11
    template<typename T, size_t N>
    using SquareMatrix = Matrix<T, N, N>;
#endif

#ifndef TINYMATRIX_NO_VECTORS
    template <typename T, size_t N>
    struct Vector : public Matrix<T, N, 1> {
    public:
        Vector(T v = 0) : Matrix<T,N,1>(v) {
        }
        Vector(const Matrix<T,N,1>& other) : Matrix<T,N,1>(other) {
        }
        Vector(const T data[N]) {
            for (size_t i = 0; i < N; ++i)
                this->data[i][0] = data[i];
        }

        #ifndef TINYMATRIX_NO_CPP11
        Vector(std::initializer_list<T> list) {
            size_t index = 0;
            for (auto it = std::begin(list); it != std::end(list) && index < N; ++it)
                this->data[index++][0] = T(*it);
        }
        #endif

        T Dot(const Vector<T, N>& b) const {
            const Matrix<T,N,1> &a(*this);
            const Matrix<T,N,1> b_mat = b;
            return (a.Transpose() * b_mat)(0,0);
        }

        T Length() const { return Magnitude(); }
        T Magnitude() const {
            return T(sqrt(Dot(*this)));
        }

        void Normalize() {
            (*this) * (T(1.0)/Magnitude());
        }

        Vector<T,N> Unit() const {
            return (*this) * (T(1.0)/Magnitude());
        }

        static Vector<T,N> Basis(size_t i) {
            Vector<T,N> ret;
            ret(i) = 1;
            return ret;
        }

        T operator()(size_t i) const { return this->data[i][0]; }
        T& operator()(size_t i) { return this->data[i][0]; }

        template<size_t M>
        friend Vector<T,M> operator*(const Matrix<T,M,N>& lhs, const Vector<T,N>& rhs) {
            Matrix<T,M,1> product = lhs * (const Matrix<T,N,1>)rhs;
            return Vector<T,M>(product);
        }
    };

    template<class T>
    Vector<T,3> CrossProduct(Vector<T,3> a, Vector<T,3> b) {
        T res[3] = {
            (a(1) * b(2) - a(2)*b(1)),
            -(a(0) * b(2) - a(2)*b(0)),
            (a(0) * b(1) - a(1)*b(0))
        };
        return Vector<T,3>(res);
    }
#endif
}
