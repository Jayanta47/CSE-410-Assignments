#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include<utility>
#include <assert.h>

#include "Vector.h"

using namespace std; 

class Triangle
{
private:
    /* data */
    Point points[3];
    int colors[3];

    void assignRandomColors();
public:
    Triangle(Point A, Point B, Point C);
    std::string getVerticesStr();
    Point getVertices (int sl);
    int getValofColor(std::string color);
    std::pair<double, double> MinMaxPoint(char axis);
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
    colors[0] = rand()%256;
    colors[1] = rand()%256;
    colors[2] = rand()%256;
}

std::string Triangle::getVerticesStr()
{
    std::ostringstream oss;
    oss<<"Vertices:\n";
    oss<<points[0].x<<" "<<points[0].y<<" "<<points[0].z<<"\n";
    oss<<points[1].x<<" "<<points[1].y<<" "<<points[1].z<<"\n";
    oss<<points[2].x<<" "<<points[2].y<<" "<<points[2].z<<"\n";

    return oss.str();

}

Point Triangle::getVertices(int sl)
{
    return points[sl];
}

int Triangle::getValofColor(std::string color)
{
    if (color=="red") return colors[0];
    else if (color=="green") return colors[1];
    else if (color=="blue") return colors[2];
    else return -1;
}

std::pair<double, double> Triangle::MinMaxPoint(char axis)
{
    assert(axis=='X' || axis == 'Y'||axis == 'Z');
    pair<double, double> p;
    if (axis == 'X') 
    {
        p.first = min(points[0].x, min(points[1].x, points[2].x));
        p.second = max(points[0].x, max(points[1].x, points[2].x));
    }
    else if (axis == 'Y')
    {
        p.first = min(points[0].y, min(points[1].y, points[2].y));
        p.second = max(points[0].y, max(points[1].y, points[2].y));
    }
    else {
        p.first = min(points[0].z, min(points[1].z, points[2].z));
        p.second = max(points[0].z, max(points[1].z, points[2].z));
    }
    return p;
}

Triangle::~Triangle()
{
}


#endif