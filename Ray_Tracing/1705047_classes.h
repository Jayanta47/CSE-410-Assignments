#ifndef CLASSES_H
#define CLASSES_H

#include "Vector.h"

int recursionLevel = 0;
/* Color structure */
struct Color
{
    double red;
    double green;
    double blue;

    Color()
    {
        red = green = blue = 0.0;
    }

    Color(double red, double green, double blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
};

class Ray
{
    Point rO; // origin Point
    Point rD; // unit direction Point

public:
    Ray()
    {
        /* default constructor */
    }

    Ray(Point rO, Point rD)
    {
        this->rO = rO;
        this->rD = rD;
        this->rD.normalize();
    }

    Point getRO() const
    {
        return rO;
    }

    Point getRD() const
    {
        return rD;
    }

    ~Ray()
    {
        /* destructor */
    }
};

struct ReflectionCoeff
{
    double ambientCoeff;
    double diffuseCoeff;
    double specularCoeff;
    double recursiveCoeff;

    ReflectionCoeff()
    {
        ambientCoeff = 0.0;
        diffuseCoeff = 0.0;
        specularCoeff = 0.0;
        recursiveCoeff = 0.0;
    }

    ReflectionCoeff(double ambientCoeff,
                    double diffuseCoeff,
                    double specularCoeff,
                    double recursiveCoeff)
    {
        this->ambientCoeff = ambientCoeff;
        this->diffuseCoeff = diffuseCoeff;
        this->specularCoeff = specularCoeff;
        this->recursiveCoeff = recursiveCoeff;
    }
};

class Object
{
protected:
    Point reference_point;
    double height, width, length;
    Color color;
    ReflectionCoeff rfco;
    int shine;
    std::string type;

public:
    Object(/* args */);
    Color getColor() const
    {
        return color;
    }

    void setColor(Color color)
    {
        this->color = color;
    }

    ReflectionCoeff getReflectionCoefficient() const
    {
        return rfco;
    }

    void setReflectionCoefficient(ReflectionCoeff rfco)
    {
        this->rfco = rfco;
    }

    int getShininess() const
    {
        return shine;
    }

    void setShininess(int shine)
    {
        this->shine = shine;
    }

    std::string get_type()
    {
        return this->type;
    }


    virtual void draw();
    virtual double intersect(Ray, Color &, int);
    ~Object();
};

Object::Object(/* args */)
{
}

Object::~Object()
{
}

class Sphere : public Object
{
private:
    /* data */
    int segments;
    int stacks;

public:
    Sphere(/* args */);
    Sphere(Point center, double radius, int segments, int stacks);
    void draw();
    double intersect(Ray, Color &, int);
    ~Sphere();
};

Sphere::Sphere(/* args */)
{
    length = 0.0;
    segments = stacks = 0;
    this->type = "Sphere";
}

Sphere::Sphere(Point center, double radius, int segments, int stacks)
{
    this->length = radius;
    this->segments = segments;
    this->stacks = stacks;
    this->reference_point = center;
    this->type = "Sphere";
}

Sphere::~Sphere()
{
}

void Sphere::draw()
{
    Point points[stacks+1][segments+1];
    int i, j;
    double h, r;
    double total_angle = (2.0 * pi);
    // generate points
    for (i = 0; i <= this->stacks; i++)
    {
        h = this->length * sin(((double)i / (double)stacks) * (pi / 2));
        r = this->length * cos(((double)i / (double)stacks) * (pi / 2));
        for (j = 0; j <= this->segments; j++)
        {
            points[i][j].x = r * cos(((double)j / (double)this->segments) * total_angle);
            points[i][j].y = r * sin(((double)j / (double)this->segments) * total_angle);
            points[i][j].z = h;
        }
    }
    glColor3f(color.red, color.green, color.blue);
    // draw quads using generated points
    for (i = 0; i < stacks; i++)
    {
        
        for (j = 0; j < segments; j++)
        {
            glBegin(GL_QUADS);
            {
                // upper hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                // lower hemisphere
                
                glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
            
            }
            glEnd();
        }
    }
}

double Sphere::intersect(Ray ray, Color &color, int level)
{
}

class Triangle : public Object
{
private:
    /* data */
    Point reference_point2;
    Point reference_point3;

public:
    Triangle(/* args */);
    Triangle(Point a, Point b, Point c);
    void draw();
    double intersect(Ray, Color &, int);
    ~Triangle();
};

Triangle::Triangle(/* args */)
{
    this->type = "Triangle";
}

Triangle::Triangle(Point a, Point b, Point c)
{
    this->reference_point = a;
    this->reference_point2 = b;
    this->reference_point3 = c;
    this->type = "Triangle";
}

void Triangle::draw()
{
    glColor3f(this->color.red, this->color.green, this->color.blue);

    glBegin(GL_TRIANGLES);
    {
        glVertex3f(this->reference_point.x, this->reference_point.y, this->reference_point.z);
        glVertex3f(this->reference_point2.x, this->reference_point2.y, this->reference_point2.z);
        glVertex3f(this->reference_point3.x, this->reference_point3.y, this->reference_point3.z);
    }
    glEnd();
}

double Triangle::intersect(Ray ray, Color &color, int level)
{
}

Triangle::~Triangle()
{
}

class PointLight
{
private:
    /* data */
    Point light_pos;
    Color color;

public:
    PointLight(/* args */);
    PointLight(Point p, Color c)
    {
        this->light_pos = p;
        this->color = c;
    }
    ~PointLight();
};

PointLight::PointLight(/* args */)
{
}

PointLight::~PointLight()
{
}

class SpotLight
{
private:
    /* data */
    PointLight point_light;
    Point light_direction;
    double cutoff_angle;

public:
    SpotLight(/* args */);
    ~SpotLight();
};

SpotLight::SpotLight(/* args */)
{
}

SpotLight::~SpotLight()
{
}

class Floor : public Object
{
private:
    /* data */

public:
    Floor(/* args */);
    Floor(double floorwidth, double tilewidth);
    void draw();
    double intersect(Ray, Color &, int);
    ~Floor();
};

Floor::Floor(/* args */)
{
    length = 0.0; // floorwidth
    width = 0.0;  // tilewidth
}

Floor::Floor(double floorwidth, double tilewidth)
{
    reference_point.x = -floorwidth / 2;
    reference_point.y = -floorwidth / 2;
    reference_point.z = 0.0;
    width = tilewidth;
    length = floorwidth;
}

void Floor::draw()
{
    int num_row = (int) length/width; // floorwidth/tilewidth
    int num_col = num_row;
    for(int i=0; i<num_row; i++) {
        for(int j=0; j<num_col; j++) {
            /* drawing square on a plane parallel to x-y plane */
            Color c;
            if ((i+j)%2 == 0)
            {
                c = this->color;
            }
            else 
            {
                c.red = 1.0 - this->color.red;
                c.green = 1.0 - this->color.green;
                c.blue = 1.0 - this->color.blue;
            }
            
            glColor3f(c.red, c.green, c.blue);
            Point leftBottomCorner;
            leftBottomCorner.x = -length/2.0+width*j;
            leftBottomCorner.y = -length/2.0+width*i;
            leftBottomCorner.z = 0.0;

            glBegin(GL_QUADS);
            {
                glVertex3f(leftBottomCorner.x, leftBottomCorner.y, leftBottomCorner.z);
                glVertex3f(leftBottomCorner.x+width, leftBottomCorner.y, leftBottomCorner.z);
                glVertex3f(leftBottomCorner.x+width, leftBottomCorner.y+width, leftBottomCorner.z);
                glVertex3f(leftBottomCorner.x, leftBottomCorner.y+width, leftBottomCorner.z);
            }
            glEnd();
        }
    }
}


double Floor::intersect(Ray ray, Color &c, int level)
{
    
}

Floor::~Floor()
{
}

std::vector<Object> objects;
std::vector<PointLight> pointLights;
std::vector<SpotLight> spotLights;

#endif