//
// Created by simon on 07/12/2023.
//

#ifndef HPC_STRESS_TESTER_HPC_MATRIX_H
#define HPC_STRESS_TESTER_HPC_MATRIX_H

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

#include "hpc_matrix.h"



template <typename T>
class hpc_matrix {
private:
    int rows;
    int cols;
    T* data;

public:
    hpc_matrix() : rows(0), cols(0), data(nullptr) {}

    hpc_matrix(int rows, int cols, std::function<T(int, int)> init) : rows(rows), cols(cols) {
        data = new T[rows * cols];
        initialize(init);
    }

    hpc_matrix(int rows, int cols) : rows(rows), cols(cols) {
        data = new T[rows * cols];
    }

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }

    bool isSquare() const {
        return rows == cols;
    }

    ~hpc_matrix() {
        delete[] data;
    }

    hpc_matrix(const hpc_matrix& other) : rows(other.rows), cols(other.cols) {
        data = new T[rows * cols];

        for (int i = 0; i < rows * cols; ++i) {
            data[i] = other.data[i];
        }
    }

    hpc_matrix(hpc_matrix&& other) noexcept: rows(other.rows), cols(other.cols), data(other.data) {
        other.rows = 0;
        other.cols = 0;
        other.data = nullptr;
    }

    T operator()(int i, int j) const  {
        return data[i * cols + j];
    }

    T& operator()(int i, int j) {
        return data[i * cols + j];
    }

    void initialize(std::function<T(int, int)> init) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                (*this)(i, j) = init(i,j);
            }
        }
    }

    hpc_matrix& operator=(const hpc_matrix& other) {
        if (this != &other) {
            delete[] data;

            rows = other.rows;
            cols = other.cols;

            data = new T[rows * cols];

            for (int i = 0; i < rows * cols; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    hpc_matrix& operator=(hpc_matrix&& other) noexcept {
        if (this != &other) {
            delete[] data;

            rows = other.rows;
            cols = other.cols;
            data = other.data;

            other.rows = 0;
            other.cols = 0;
            other.data = nullptr;
        }
        return *this;
    }

    hpc_matrix operator+(const hpc_matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            fprintf(stderr, "Error: Matrices must have the same dimensions for addition.\n");

            exit(EXIT_FAILURE);
        }

        hpc_matrix result(rows, cols);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }

        return result;
    }

    hpc_matrix& operator+=(const hpc_matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            fprintf(stderr, "Error: Matrices must have the same dimensions for addition.\n");

            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                (*this)(i, j) += other(i,j);
            }
        }

        return *this;
    }

    hpc_matrix operator-(const hpc_matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            fprintf(stderr, "Error: Matrices must have the same dimensions for addition.\n");

            exit(EXIT_FAILURE);
        }

        hpc_matrix result(rows, cols);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result(i, j) = (*this)(i, j) - other(i, j);
            }
        }

        return result;
    }

    hpc_matrix& operator-=(const hpc_matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            fprintf(stderr, "Error: Matrices must have the same dimensions for addition.\n");

            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                (*this)(i, j) -= other(i,j);
            }
        }

        return *this;
    }

    hpc_matrix operator*(T other) const {
        hpc_matrix<T> result(rows, cols);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result(i, j) = (*this)(i, j) * other;
            }
        }

        return result;
    }

    hpc_matrix &operator*=(T other) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                (*this)(i, j) *= other;
            }
        }

        return *this;
    }

    hpc_matrix operator*(const hpc_matrix& other) const {
        if (cols != other.rows) {
            fprintf(stderr,  "Error: Number of columns in the first matrix must be equal to the number of rows in the second matrix for multiplication.\n");

            exit(EXIT_FAILURE);
        }

        hpc_matrix<T> result(rows, other.cols);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                result(i, j) = 0;
                for (int k = 0; k < cols; ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }

        return result;
    }

    hpc_matrix operator*=(const hpc_matrix &other) {
        (*this) = (*this) * other;
        return (*this);
    }

    hpc_matrix transpose() const {
        hpc_matrix<T> result(cols, rows);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }

        return result;
    }

    hpc_matrix power(int other) const {
        if (rows != cols) {
            fprintf(stderr, "Error: Matrices must be squared for the power.\n");

            exit(EXIT_FAILURE);
        }
        hpc_matrix result = hpc_matrix::identity(rows);

        for (int i = 0; i < other; ++i) {
            result *= (*this);
        }

        return result;
    }

    hpc_matrix inverse() const {
        if (cols != rows) {
            fprintf(stderr,  "Error: Matrices must be squared for the inverse.\n");

            exit(EXIT_FAILURE);
        }

        hpc_matrix result = identity(rows);
        T det = determinant();
        if(det == 0) {
            fprintf(stderr, "Error: Matrices aren't invertible.\n");

            exit(EXIT_FAILURE);
        }
        if(rows == 1) {
            result(0,0) = 1 / (*this)(0, 0);
            return result;
        }
        if(rows == 2) {
            result(0, 0) = (*this)(1, 1) / det;
            result(0, 1) = -(*this)(0, 1) / det;
            result(1, 0) = -(*this)(1, 0) / det;
            result(1, 1) = (*this)(0, 0) / det;
        }
        if(is_idenity()) {
            return identity(cols);
        }
        hpc_matrix inverse(rows, cols);

        hpc_matrix lower, upper, pivot;
        hpc_matrix identity = hpc_matrix::identity(rows);
        luDecomposition(lower,upper,pivot);

        for(int j = 0; j < cols; ++j) {
            // Ly = I
            hpc_matrix y(rows, 1);
            for (int i = 0; i < rows; ++i) {
                T sum = 0;
                for (int k = 0; k < i; ++k) {
                    sum += lower(i, k) * y(k, 0);
                }
                y(i, 0) = (identity(i, j) - sum) / lower(i, i);
            }

            // Ux = y
            for (int i = rows - 1; i >= 0; --i) {
                T sum = 0;
                for (int k = i + 1; k < rows; ++k) {
                    sum += upper(i, k) * inverse(k, j);
                }
                inverse(i, j) = (y(i, 0) - sum) / upper(i, i);
            }
        }

        return inverse;
    }

    bool is_idenity() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (i == j && (*this)(i, j) != 1) {
                    return false;
                }
                if (i != j && (*this)(i, j) != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    T determinant() const {
        if (rows != cols) {
            fprintf(stderr, "Error: Matrices must be squared for the power.\n");

            exit(EXIT_FAILURE);
        }

        if(rows == 1) {
            return data[0];
        }

        if(rows == 2) {
            return data[0] * data[3] - data[1] * data[2];
        }

        if (rows == 3) {
            return data[0] * data[4] * data[8] +
                   data[1] * data[5] * data[6] +
                   data[2] * data[3] * data[7] -
                   data[2] * data[4] * data[6] -
                   data[0] * data[5] * data[7] -
                   data[1] * data[3] * data[8];
        }

        hpc_matrix lu(*this);
        const int swapCount = lu.luDecomposition();

        T det = 1;
        for (int i = 0; i < rows; ++i) {
            det *= lu(i, i);
        }

        return (swapCount % 2 == 0) ? det : -det;

    }

    int luDecomposition() {
        hpc_matrix lower, upper, pivot;
        return luDecomposition(lower,upper,pivot);
    }
    int luDecomposition(hpc_matrix& lower, hpc_matrix& upper, hpc_matrix& pivot) const {
        if (rows != cols) {
            fprintf(stderr, "Error: Matrices must be squared for the power.\n");

            exit(EXIT_FAILURE);
        }

        hpc_matrix lu = (*this);
        lower = hpc_matrix(rows, cols, [](int, int) {  return T(); });
        upper = hpc_matrix(rows, cols, [](int, int) {  return T(); });
        pivot = hpc_matrix(rows, cols, [](int, int) {  return T(); });

        std::vector<int> perm(rows);
        std::iota(perm.begin(), perm.end(), 0);

        int swapCount = 0;

        for (int j = 0; j < rows - 1; ++j) {
            int maxRow = j;
            T maxValue = abs(lu(j, j));

            // Find the row with the maximum value in the current column
            for (int i = j + 1; i < rows; ++i) {
                T absValue = abs(lu(perm[i], j));
                if (absValue > maxValue) {
                    maxRow = i;
                    maxValue = absValue;
                }
            }

            /*if(maxValue <= std::numeric_limits<T>::epsilon()) {
                fprintf(stderr, "Error: Matrix is singular.\n");

                exit(EXIT_FAILURE);
            }*/

            // Swap row
            if(maxRow != j) {
                int temp = perm[j];
                perm[j] = perm[maxRow];
                perm[maxRow] = temp;
                ++swapCount;
            }

            int jj = perm[j];
            // Apply Gaussian elimination to calculate U and L
            for (int i = j + 1; i < rows; ++i) {
                int ii = perm[i];
                lu(ii, j) /= lu(jj, j);

                for (int k = j + 1; k < cols; ++k) {
                    lu(ii, j) -= lu(ii, j) * lu(jj, k);
                }
            }
        }

        for (int j = 0; j < rows; ++j) {
            lower(j, j) = 1;
            for (int i = j + 1; i < cols; ++i) {
                lower(i, j) = lu(perm[i], j);
            }
            for(int i = 0; i<=j;++i) {
                upper(i, j) = lu(perm[i], j);
            }
        }

        for(int i = 0; i< rows; ++i) {
            pivot(i,perm[i]) = 1;
        }

        return swapCount;
    }

    int to_string(char * string, const int n) {
        int k = 0;
        for(int i = 0; i< rows && k < n; ++i) {
            for(int j = 0; j< cols && k < n; ++j) {
                k += sprintf(string + k,"%f ",(*this)(i,j));
            }
            k += sprintf(string + k, "\n");
        }
        return k;
    }

    static hpc_matrix identity(int other) {
        return hpc_matrix(other, other, [](int i, int j){return T() + (i==j);});
    }
};


#endif //HPC_STRESS_TESTER_HPC_MATRIX_H
