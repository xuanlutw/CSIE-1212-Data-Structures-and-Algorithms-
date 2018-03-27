#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <new>
#include "matrix.h"

Matrix::Matrix(const int& r, const int& c) //constructor
{
    this->row = r;
    this->col = c;
    this->array = (double**)malloc(this->row * sizeof(double*));
    for (int i = 0;i < this->row;++i)
        this->array[i] = (double*)malloc(this->col * sizeof(double));
}

Matrix::Matrix(const Matrix& rhs) //copy constructor
{
    this->row = rhs.row;
    this->col = rhs.col;
    this->array = (double**)malloc(this->row * sizeof(double*));
    for (int i = 0;i < this->row;++i) {
        this->array[i] = (double*)malloc(this->col * sizeof(double));
        for (int j = 0;j < this->col;++j)
            this->array[i][j] = rhs.array[i][j];
    }
}

Matrix::~Matrix() //destructor
{
    for (int i = 0;i < this->row;++i)
        free(this->array[i]);
    free(this->array);
}

double* & Matrix::operator [](const int& idx) const
{
    return this->array[idx];
}

Matrix Matrix::operator =(const Matrix& rhs) // assignment constructor
{
    for (int i = 0;i < this->row;++i)
        free(this->array[i]);
    free(this->array);

    this->row = rhs.row;
    this->col = rhs.col;
    this->array = (double**)malloc(this->row * sizeof(double*));
    for (int i = 0;i < this->row;++i) {
        this->array[i] = (double*)malloc(this->col * sizeof(double));
        for (int j = 0;j < this->col;++j)
            this->array[i][j] = rhs.array[i][j];
    }

    for (int i = 0;i < this->row;++i)
        for (int j = 0;j < this->col;++j)
            this->array[i][j] = rhs.array[i][j];
    return rhs;
}

Matrix Matrix::operator -() const
{
    Matrix ans = *this;
    for (int i = 0;i < ans.row;++i) 
        for (int j = 0;j < ans.col;++j)
            ans.array[i][j] = -ans.array[i][j];
    return ans;
}

Matrix Matrix::operator +() const
{
    return *this;
}

Matrix Matrix::operator -(const Matrix& rhs) const
{
    if (this->row != rhs.row || this->col != rhs.col) {
        fprintf(stderr, "In file: %s, line: %d, class matrix dim NOT equal!\n", __FILE__, __LINE__);
        return rhs;
    }
    Matrix ans(this->row, this->col);
    for (int i = 0;i < this->row;++i)
        for (int j = 0;j < this->col;++j)
            ans.array[i][j] = this->array[i][j] - rhs.array[i][j];
    return ans;
}

Matrix Matrix::operator +(const Matrix& rhs) const
{
    if (this->row != rhs.row || this->col != rhs.col) {
        fprintf(stderr, "In file: %s, line: %d, class matrix dim NOT equal!\n", __FILE__, __LINE__);
        return rhs;
    }
    Matrix ans(this->row, this->col);
    for (int i = 0;i < this->row;++i)
        for (int j = 0;j < this->col;++j)
            ans.array[i][j] = this->array[i][j] + rhs.array[i][j];
    return ans;
}

Matrix Matrix::operator *(const Matrix& rhs) const
{
    if (this->col != rhs.row) {
        fprintf(stderr, "In file: %s, line: %d, class matrix dim NOT match!\n", __FILE__, __LINE__);
        return rhs;
    }
    Matrix ans(this->row, rhs.col);
    for (int i = 0;i < ans.row;++i) {
        for (int j = 0;j < ans.col;++j) {
            ans.array[i][j] = 0;
            for (int k = 0;k < this->col;++k)
                ans.array[i][j] += this->array[i][k] * rhs.array[k][j];
        }
    }
    return ans;
}

Matrix Matrix::operator /(const Matrix& rhs) const
{
    return *this * rhs.inverse();
}

Matrix Matrix::inverse() const
{
    if (this->row != this->col) {
        fprintf(stderr, "In file: %s, line: %d, class matrix NOT square!\n", __FILE__, __LINE__);
        return *this;
    }
    Matrix ans(this->row, this->col);
    Matrix matrix = *this;
    int n = ans.row;
    for (int i = 0;i < n;++i) 
        for (int j = 0;j < n;++j)
            ans[i][j] = (i == j);
    // up triangle
    for (int i = 0;i < n;++i) {
        for (int j = i + 1;j < n;++j) {
            if (matrix[i][i] == 0) {
                for (int k = 0;k < n;++k) {
                    double tmp = matrix[j][k];
                    matrix[j][k] = matrix[i][k];
                    matrix[i][k] = tmp;
                    tmp = ans[j][k];
                    ans[j][k] = ans[i][k];
                    ans[i][k] = tmp;
                }
            }
            else if (matrix[j][i] == 0) {
                continue;
            }
            else {
                double mul = matrix[j][i] / matrix[i][i];
                for (int k = 0;k < n;++k) {
                    matrix[j][k] -= mul * matrix[i][k];
                    ans[j][k] -= mul * ans[i][k];
                }
            }
        }
    }
    // low triangle
    for (int i = n - 1;i >= 0;--i) {
        for (int j = 0;j < i;++j) {
            double mul = matrix[j][i] / matrix[i][i];
            for (int k = 0;k < n;++k) {
                matrix[j][k] -= mul * matrix[i][k];
                ans[j][k] -= mul * ans[i][k];
            }
        }
    }
    // normalize
    for (int i = 0;i < n;++i) {
        double dev = matrix[i][i];
        for (int k = 0;k < n;++k) {
            matrix[i][k] /= dev;
            ans[i][k] /= dev;
        }
    }
    return ans;
}

void Matrix::read(const char* fn)
{
    int r, c;
    FILE *fp = fopen(fn, "r");
    if(fp == NULL){
        printf("read file [%s] error\n", fn);
        exit(0);
    }
    fscanf(fp, "%d%d", &r, &c);
    Matrix tmp(r, c);
    for(int i = 0 ; i < r ; i++)
        for(int j = 0 ; j < c ; j++)
            fscanf(fp, "%lf", &tmp.array[i][j]);
    fclose(fp);
    *this = tmp;
}

void Matrix::write(const char* fn)
{
    FILE *fp = fopen(fn, "w");
    if(fp == NULL){
        printf("write file [%s] error\n", fn);
        exit(0);
    }
    fprintf(fp, "%d %d\n", row, col);
    for(int i = 0 ; i < row ; i++)
        for(int j = 0 ; j < col ; j++)
            fprintf(fp, "%lf%c", array[i][j], " \n"[j==col-1]);
    fclose(fp);
}

void Matrix::print() const
{
    for(int i = 0 ; i < row ; i++)
        for(int j = 0 ; j < col ; j++)
            printf("%lf%c", array[i][j], " \n"[j==col-1]);
}
