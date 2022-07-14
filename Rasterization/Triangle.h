#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cstdlib>
#include <ctime>

#include "Vector.h"


class Triangle
{
private:
    /* data */
    Point points[3];
    int colors[3];

    void assignRandomColors();
public:
    Triangle(Point A, Point B, Point C);
    
    ~Triangle();
};

Triangle::Triangle(Point A, Point B, Point C)
{
    this->points[0] = A;
    this->points[1] = B;
    this->points[2] = C;

    this->assignRandomColors();
}

void Triangle::assignRandomColors()
{
    srand(time(0));

    colors[0] = rand()%256;
    colors[1] = rand()%256;
    colors[2] = rand()%256;
}

Triangle::~Triangle()
{
}


#endif