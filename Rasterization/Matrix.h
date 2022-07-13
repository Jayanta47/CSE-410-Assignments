#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include <assert.h>

#define forn(i, n) for (i = 0; i < n; i++)

struct Triplet
{
    double x, y, z;
    Triplet()
    {
        x = y = z = 0.0;
    }

    Triplet(double x_, double y_, double z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
};

struct gluLookParams
{
    double eyeX, eyeY, eyeZ;
    double lookX, lookY, lookZ;
    double upX, upY, upZ;

    void print()
    {
        printf("glulookAt Parameters:\n");
        printf("eyeX: %.4lf, eyeY: %.4lf, eyeZ: %.4lf\n", eyeX, eyeY, eyeZ);
        printf("lookX: %.4lf, lookY: %.4lf, lookZ: %.4lf\n", lookX, lookY, lookZ);
        printf("upX: %.4lf, upY: %.4lf, upZ: %.4lf\n", upX, upY, upZ);
    }
};

struct perspectiveParams
{
    double fovY, aspectRatio, near, far;

    void print()
    {
        printf("gluPerspective Parameters:\n");
        printf("fovY: %.4lf, aspectRatio: %.4lf, near: %.4lf, far: %.4lf\n",
               fovY, aspectRatio, near, far);
    }
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
    void multiplyMatrix(TransformMatrix mat);
    Point multiplyPoint(Point A);
    TransformMatrix getCopy();
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

void TransformMatrix::multiplyMatrix(TransformMatrix mat)
{
    double temp[size][size];
    double sum = 0.0;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            sum = 0.0;
            for (int k = 0; k < size; k++)
            {
                sum += matrix[i][k] * mat.getVal(k, j);
            }
            temp[i][j] = sum;
        }
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = temp[i][j];
        }
    }
}

Point TransformMatrix::multiplyPoint(Point A)
{
    Point temp;
    double arr[4];
    arr[0] = A.x;
    arr[1] = A.y;
    arr[2] = A.z;
    arr[3] = A.w;

    double pointArr[4];

    int i, k;
    forn(i, size)
    {
        double sum = 0.0;
        forn(k, size)
        {
            sum += matrix[i][k] * arr[k];
        }
        pointArr[i] = sum;
    }

    temp.x = pointArr[0] / pointArr[3];
    temp.y = pointArr[1] / pointArr[3];
    temp.z = pointArr[2] / pointArr[3];
    temp.w = 1.0;

    return temp;
}

TransformMatrix TransformMatrix::getCopy()
{
    TransformMatrix temp;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            temp.setVal(i, j, matrix[i][j]);
        }
    }

    return temp;
}

TransformMatrix::~TransformMatrix()
{
}

void createIdentityMtx(TransformMatrix &mat)
{
    int n = mat.getSize();
    int i, j;
    forn(i, n)
    {
        forn(j, n)
        {
            if (i == j)
                mat.setVal(i, j, 1.0);
            else
                mat.setVal(i, j, 0.0);
        }
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

    // std::cout<<a.x<<" "<<a.y<<" "<<a.z<<std::endl;

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

void createViewMatrix(TransformMatrix &mat, gluLookParams glP)
{
    Point up(glP.upX, glP.upY, glP.upZ);
    Point look(glP.lookX, glP.lookY, glP.lookZ);
    Point eye(glP.eyeX, glP.eyeY, glP.eyeZ);

    Point l = look.returnCopy();
    l.addVector(eye, -1);
    l.normalize();

    Point r = l.crossProduct(up);
    r.normalize();

    Point u = r.crossProduct(l);
    u.normalize();

    TransformMatrix translateM;
    createIdentityMtx(translateM);
    Triplet T(-glP.eyeX, -glP.eyeY, -glP.eyeZ);

    createTranslationMtx(translateM, T);

    // creating rotation matrix

    mat.setVal(0, 0, r.x);
    mat.setVal(0, 1, r.y);
    mat.setVal(0, 2, r.z);

    mat.setVal(1, 0, u.x);
    mat.setVal(1, 1, u.y);
    mat.setVal(1, 2, u.z);

    mat.setVal(2, 0, -l.x);
    mat.setVal(2, 1, -l.y);
    mat.setVal(2, 2, -l.z);

    mat.multiplyMatrix(translateM);
}

void createProjectionMatrix(TransformMatrix &mat, perspectiveParams prsP)
{
    double fovX = prsP.fovY * prsP.aspectRatio;
    double t = prsP.near * tan(deg2Rad(prsP.fovY / 2.0));
    double r = prsP.near * tan(deg2Rad(fovX / 2.0));

    mat.setVal(0, 0, prsP.near / r);
    mat.setVal(1, 1, prsP.near / t);
    mat.setVal(2, 2, -(prsP.far + prsP.near) / (prsP.far - prsP.near));
    mat.setVal(2, 3, -(2.0 * prsP.far * prsP.near) / (prsP.far - prsP.near));
    mat.setVal(3, 2, -1.0);
    mat.setVal(3, 3, 0.0);
}

Point multiplyPoint(TransformMatrix mat, Point A)
{
    Point temp;
    double arr[4];
    arr[0] = A.x;
    arr[1] = A.y;
    arr[2] = A.z;
    arr[3] = A.w;

    double pointArr[4];

    int i, k, size = mat.getSize();
    forn(i, size)
    {
        double sum = 0.0;
        forn(k, size)
        {
            sum += mat.getVal(i, k) * arr[k];
        }
        pointArr[i] = sum;
    }

    temp.x = pointArr[0] / pointArr[3];
    temp.y = pointArr[1] / pointArr[3];
    temp.z = pointArr[2] / pointArr[3];
    temp.w = 1.0;

    return temp;
}

#endif