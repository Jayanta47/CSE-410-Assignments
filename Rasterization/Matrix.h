#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include <assert.h>

struct Triplet
{
    double x, y, z;
};

class TransformMatrix
{
private:
    /* data */
    int size = 4;
    double matrix[4][4];

public:
    TransformMatrix();
    void setVal(int row, int col, double newVal);
    double getVal(int, int);
    int getSize();
    ~TransformMatrix();
};

TransformMatrix::TransformMatrix()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = 0.0;
        }
    }
}

void TransformMatrix::setVal(int row, int col, double newVal)
{
    assert(row < 4 && col < 4);
    this->matrix[row][col] = newVal;
}

double TransformMatrix::getVal(int row, int col)
{
    assert(row < 4 && col < 4);
    return this->matrix[row][col];
}

int TransformMatrix::getSize()
{
    return size;
}

TransformMatrix::~TransformMatrix()
{
}

void createIdentityMtx(TransformMatrix &mat)
{
    int n = mat.getSize();
    for (int i = 0; i < n; i++)
    {
        mat.setVal(i, i, 1.0);
    }
}

void createTranslationMtx(TransformMatrix &mat, Triplet &T)
{
    mat.setVal(0, 3, T.x);
    mat.setVal(1, 3, T.y);
    mat.setVal(2, 3, T.z);
}

void createScalingMtx(TransformMatrix &mat, Triplet &S)
{
    mat.setVal(0, 0, S.x);
    mat.setVal(1, 1, S.y);
    mat.setVal(2, 2, S.z);
}

void createRotationMtx(TransformMatrix &mat, Point a, double angle)
{
    a.normalize();
    Point i_vect(1.0, 0.0, 0.0);
    Point j_vect(0.0, 1.0, 0.0);
    Point k_vect(0.0, 0.0, 1.0);

    Point c1 = RodriguesFormula(i_vect, a, angle);
    Point c2 = RodriguesFormula(j_vect, a, angle);
    Point c3 = RodriguesFormula(k_vect, a, angle);

    mat.setVal(0, 0, c1.x);
    mat.setVal(1, 0, c1.y);
    mat.setVal(2, 0, c1.z);
    
    mat.setVal(0, 1, c2.x);
    mat.setVal(1, 1, c2.y);
    mat.setVal(2, 1, c2.z);
    
    mat.setVal(0, 2, c3.x);
    mat.setVal(1, 2, c3.y);
    mat.setVal(2, 2, c3.z);
}

#endif