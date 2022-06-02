#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))
#define ANTICLOCKWISE 1
#define CLOCKWISE -1
#define MAX_THRESH 40.0

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int translation_unit = 2;
double rotation_unit = 3; // in degrees


double deg2Rad(double angle)
{
    return (pi * angle)/180.0;
}

class Point
{
public:
    double x,y,z;
    Point() {}
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void addVector(Point vect, int sign = 1)
    {
        this->x = this->x + vect.x * sign * translation_unit;
        this->y = this->y + vect.y * sign * translation_unit;
        this->z = this->z + vect.z * sign * translation_unit;
    }

    Point crossProduct(Point vect)
    {
        Point temp;
        temp.x = this->y * vect.z - this->z * vect.y;
        temp.y = this->z * vect.x - this->x * vect.z;
        temp.z = this->x * vect.y - this->y * vect.x;

        return temp;
    }

    Point returnDeepCopy()
    {
        Point temp;
        temp.x = this->x;
        temp.y = this->y;
        temp.z = this->z;
        return temp;
    }
};


