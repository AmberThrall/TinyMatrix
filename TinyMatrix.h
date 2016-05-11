// LEGAL SHIT HERE

#pragma once

#include <cstring>
#include <cmath>
#include <iostream>

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

        void SetRow(int r, T row[N]) {
            for (size_t i = 0; i < N; ++i)
                this->data[r][i] = row[i];
        }

        void SetRow(int r, Matrix<T,1,N> row) {
            for (size_t i = 0; i < N; ++i)
                this->data[r][i] = row(0,i);
        }

        void SetColumn(int c, T column[M]) {
            for (size_t i = 0; i < M; ++i)
                this->data[i][c] = column[i];
        }

        void SetColumn(int c, Matrix<T,M,1> column) {
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

        Matrix<T,M,N> DivideRow(T value, size_t i) {
            Matrix<T,M,N> ret(*this);
            for (size_t j = 0; j < N; ++j) {
                ret(i,j) /= value;
            }
            return ret;
        }

        Matrix<T,M,N> DivideColumn(T value, size_t j) {
            Matrix<T,M,N> ret(*this);
            for (size_t i = 0; i < M; ++i) {
                ret(i,j) /= value;
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

        Matrix<T,M,N> GaussJordan() {
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
                    ret = ret.DivideRow(ret(r,lead), r);
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
            return _DeterminantHelper(RemoveRowAndColumn(i, j), _int2type<M-1>());
        }

        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, T>::type Cofactor(size_t i, size_t j)
        #else
        T Cofactor(size_t i, size_t j)
        #endif
        {
            return T(pow(-1.0f, i+j)) * Minor(i, j);
        }

        #ifndef TINYMATRIX_NO_CPP11
        template<size_t m = M, size_t n = N>
        typename std::enable_if<m == n, Matrix<T,M,M>>::type Adjugate()
        #else
        Matrix<T,M,M> Adjugate()
        #endif
        {
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

        friend std::ostream& operator<<(std::ostream& os, const Matrix<T,M,N> mat) {
            for (size_t r = 0; r < M; ++r) {
                os << "[ ";
                for (size_t c = 0; c < N; ++c)
                    os << mat(r,c) << " ";
                os << "] " << std::endl;
            }
            return os;
        }
    protected:
        T data[M][N];
    };

#ifndef TINYMATRIX_NO_CPP11
    template<typename T, size_t N>
    using SquareMatrix = Matrix<T, N, N>;
#endif

#ifndef TINYMATRIX_NO_VECTORS
    template <typename T, size_t N>
    class Vector : public Matrix<T, N, 1> {
    public:
        Vector(T v = 0) : Matrix<T,N,1>(v) {
        }
        Vector(const Matrix<T,N,1>& other) : Matrix<T,N,1>(other) {
        }
        Vector(const T data[N]) {
            for (size_t i = 0; i < N; ++i)
                this->data[i][0] = data[i];
        }

        T Dot(const Vector<T, N>& b) const {
            const Vector<T,N> &a(*this);
            return (a.Transpose() * b)(0,0);
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

        T operator()(size_t i) const { return this->data[i][0]; }
        T& operator()(size_t i) { return this->data[i][0]; }
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
}
