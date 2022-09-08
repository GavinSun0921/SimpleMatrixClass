/**
 * @author  Gavin
 * @date    2022/9/1
 * @Email   gavinsun0921@foxmail.com
 */

#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>

template<typename T = double>
class Matrix {
public:
    Matrix() = delete;

    explicit Matrix(size_t _dimx, size_t _dimy) {
        this->dimX = _dimx;
        this->dimY = _dimy;
        buffer.resize(_dimx * _dimy * 2);
    }

    Matrix(const Matrix<T> &mat) noexcept {
        this->dimX = mat.dimX;
        this->dimY = mat.dimY;
        this->buffer.assign(mat.buffer.begin(), mat.buffer.end());
    }

    ~Matrix() = default;

    T &at(size_t x, size_t y) {
        typename std::vector<T>::iterator it = this->buffer.begin() + (x * this->dimY + y);
        if (it >= this->buffer.begin() && it < this->buffer.end()) {
            return *it;
        } else {
            throw std::out_of_range("matrix out of range");
        }
    }

    T &fastAt(size_t x, size_t y) {
        typename std::vector<T>::iterator it = this->buffer.begin() + (x * this->dimY + y);
        return *it;
    }

    const T &cat(size_t x, size_t y) const {
        typename std::vector<T>::const_iterator it = this->buffer.begin() + (x * this->dimY + y);
        if (it >= this->buffer.begin() && it < this->buffer.end()) {
            return *it;
        } else {
            throw std::out_of_range("matrix out of range");
        }
    }

    const T &fastCat(size_t x, size_t y) const {
        typename std::vector<T>::const_iterator it = (this->buffer.begin() + (x * this->dimY + y));
        return *it;
    }

    Matrix<T> &operator=(const Matrix<T> &src) {
        this->dimX = src.dimX;
        this->dimY = src.dimY;
        this->buffer.assign(src.buffer.begin(), src.buffer.end());
    }

    Matrix<T> operator+() const {
        return *this;
    }

    Matrix<T> operator-() const {
        Matrix<T> tmp(*this);
        for (auto val: tmp) {
            val = -val;
        }
        return tmp;
    }

    Matrix<T> operator~() const {
        Matrix<T> ret = Matrix<T>(this->dimY, this->dimX);
        for (size_t i = 0; i < this->dimX; i++) {
            for (size_t j = 0; j < this->dimY; j++) {
                ret.fastAt(i, j) = this->fastCat(j, i);
            }
        }
        return ret;
    }

    Matrix<T> &operator+=(const Matrix<T> src) {
        return *this = *this + src;
    }

    Matrix<T> &operator-=(const Matrix<T> src) {
        return *this = *this - src;
    }

    Matrix<T> &operator*=(const T c) {
        return *this = *this * c;
    }

    Matrix<T> &operator*=(const Matrix<T> src) {
        return *this = *this * src;
    }

    T &operator[](size_t y) {
        // TODO not implement
    }

    Matrix<T> pow(const int y) {
        Matrix<T> ret = Matrix<T>(this->dimX, this->dimY);
        for (size_t i = 0; i < this->dimX; i++) {
            for (size_t j = 0; j < this->dimY; j++) {
                ret.fastAt(i, j) = std::pow(this->fastCat(i, j), y);
            }
        }
        return ret;
    }

public:
    size_t dimX{}, dimY{};

private:
    std::vector<T> buffer;

};

template<typename T>
Matrix<T> transposition(const Matrix<T> &x) {
    return ~x;
}

template<typename T>
Matrix<T> operator+(const Matrix<T> &a, const Matrix<T> &b) {
    if (a.dimX != b.dimX or a.dimY != b.dimY) {
        throw std::logic_error("the matrix's size doesn't match in matrix addition!");
    }
    Matrix<T> res = Matrix<T>(a.dimX, a.dimY);
    for (size_t i = 0; i < res.dimX; i++) {
        for (size_t j = 0; j < res.dimY; j++) {
            res.fastAt(i, j) = a.fastCat(i, j) + b.fastCat(i, j);
        }
    }
    return res;
}

template<typename T>
Matrix<T> operator-(const Matrix<T> &a, const Matrix<T> &b) {
    if (a.dimX != b.dimX or a.dimY != b.dimY) {
        throw std::logic_error("the matrix's size doesn't match in matrix subtraction!");
    }
    Matrix<T> res = Matrix<T>(a.dimX, a.dimY);
    for (size_t i = 0; i < res.dimX; i++) {
        for (size_t j = 0; j < res.dimY; j++) {
            res.fastAt(i, j) = a.fastCat(i, j) - b.fastCat(i, j);
        }
    }
    return res;
}

template<typename T>
Matrix<T> operator*(const Matrix<T> &a, const T &b) {
    Matrix<T> res = Matrix<T>(a.dimX, a.dimY);
    for (size_t i = 0; i < a.dimX; i++) {
        for (size_t j = 0; j < a.dimY; j++) {
            res.fastAt(i, j) = a.fastAt(i, j) * b;
        }
    }
    return res;
}

template<typename T>
Matrix<T> operator*(const T &b, const Matrix<T> &a) {
    return operator*(a, b);
}

template<typename T>
Matrix<T> operator*(const Matrix<T> &a, const Matrix<T> &b) {
    if (a.dimY != b.dimX) {
        throw std::logic_error(
                "first matrix's columns doesn't equal to second matrix's rows in matrix multiplication!");
    }
    Matrix<T> res = Matrix<T>(a.dimX, b.dimY);
    for (size_t i = 0; i < a.dimX; i++) {
        for (size_t j = 0; j < b.dimY; j++) {
            res.fastAt(i, j) = 0;
            for (size_t k = 0; k < a.dimY; k++) {
                res.fastAt(i, j) += a.fastCat(i, k) * b.fastCat(k, j);
            }
        }
    }
    return res;
}

template<typename T>
std::ostream &print(std::ostream &ostr, const Matrix<T> &a) {
    for (size_t i = 0; i < a.dimX; i++) {
        if (i == 0) ostr << "[";
        else ostr << " ";
        ostr << "[\t";
        for (size_t j = 0; j < a.dimY; j++) {
            ostr << a.fastCat(i, j) << "\t";
        }
        if (i < a.dimX - 1) ostr << "]\n";
        else ostr << "]]\n";
    }
    return ostr;
}

template<typename T>
std::ostream &operator<<(std::ostream &ostr, const Matrix<T> &a) {
    return print(ostr, a);
}
