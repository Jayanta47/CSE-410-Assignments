#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>

#define pi (2.0 * acos(0.0))

int addition_scale = 1;

double deg2Rad(double angle)
{
    return ((pi * angle) / 180.0);
}

class Point
{
public:
    double x, y, z, w;
    Point() { this->w = 1.0; }
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1.0;
    }

    Point(double x, double y, double z, double w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void addVector(Point vect, int sign = 1)
    {
        this->x = this->x + vect.x * sign * addition_scale;
        this->y = this->y + vect.y * sign * addition_scale;
        this->z = this->z + vect.z * sign * addition_scale;
    }

    void scalarMult(double scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
    }

    Point crossProduct(Point vect)
    {
        Point temp;
        temp.x = this->y * vect.z - this->z * vect.y;
        temp.y = this->z * vect.x - this->x * vect.z;
        temp.z = this->x * vect.y - this->y * vect.x;

        return temp;
    }

    double dotProduct(Point vect)
    {
        
        double sum = (this->x * vect.x) + (this->y * vect.y) + (this->z * vect.z);
        return sum;
    }

    void normalize()
    {
        double magnitude = sqrt(x * x + y * y + z * z);
        this->x /= magnitude;
        this->y /= magnitude;
        this->z /= magnitude;
    }

    Point returnCopy()
    {
        Point temp(this->x, this->y, this->z, this->w);
        return temp;
    }
};

Point RodriguesFormula(Point x, Point a, double angle)
{
    /**
     *  R(x) = cos(Theta).x + (1-cos(Theta))(a.x)a + sin(Theta)(a*x)
     */

    // a is sent as a normalized vector

    double cosTheta = cos(deg2Rad(angle));
    double sinTheta = sin(deg2Rad(angle));


    double scalarMlt_a = (1.0 - cosTheta) * a.dotProduct(x);
    Point a_cross_x = a.crossProduct(x);

    x.scalarMult(cosTheta);
    a_cross_x.scalarMult(sinTheta);
    a.scalarMult(scalarMlt_a);

    Point temp;
    temp.addVector(x);
    temp.addVector(a);
    temp.addVector(a_cross_x);

    return temp;
}

#endif
