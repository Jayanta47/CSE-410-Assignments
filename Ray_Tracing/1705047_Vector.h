#ifndef VECTOR_H
#define VECTOR_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cmath>
#include<cstdlib>
#include<vector>
#include<limits>


// #ifdef _WIN32 
// #include <windows.h>
// #endif 

// #include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))
#define ANTICLOCKWISE 1
#define CLOCKWISE -1
#define INF numeric_limits<double>::infinity()

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int addition_scale = 1;
double rotation_unit = 3; // in degrees


double deg2Rad(double angle)
{
    return (pi * angle)/180.0;
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

    Point addVectorCopy(Point vect, int sign = 1)
    {
        Point temp;
        temp.x = this->x + vect.x * sign * addition_scale;
        temp.y = this->y + vect.y * sign * addition_scale;
        temp.z = this->z + vect.z * sign * addition_scale;

        return temp;
    }

    void scalarMult(double scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
    }

    Point scalarMultCopy(double scalar)
    {
        Point new_point;
        new_point.x = this->x * scalar;
        new_point.y = this->y * scalar;
        new_point.z = this->z * scalar;

        return new_point;
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


void rotate_vector(Point &vec, Point axis, int angle_dir) // + for anticlockwise, - for clockwise
{
    Point vec_cross_axis = axis.crossProduct(vec.returnCopy());

    double ang = angle_dir * deg2Rad(rotation_unit);

    vec.x = vec.x * cos(ang) + vec_cross_axis.x * sin(ang);
    vec.y = vec.y * cos(ang) + vec_cross_axis.y * sin(ang);
    vec.z = vec.z * cos(ang) + vec_cross_axis.z * sin(ang);
}

double distanceBetweenTwoPoints(Point a, Point b)
{
    double x_ = pow(a.x-b.x, 2.0);
    double y_ = pow(a.y-b.y, 2.0);
    double z_ = pow(a.z-b.z, 2.0);

    return sqrt(x_+y_+z_);
}

#endif 


