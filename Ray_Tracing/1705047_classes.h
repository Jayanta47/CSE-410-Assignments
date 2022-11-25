#ifndef CLASSES_H
#define CLASSES_H

#include "1705047_Vector.h"

int recursionLevel = 0;
Point pos;

int f=0;
using namespace std;

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
    Point getLightPosition()
    {
        return this->light_pos;
    }
    Color getPointColor()
    {
        return this->color;
    }

    void draw();
    ~PointLight();
};

PointLight::PointLight(/* args */)
{
}

PointLight::~PointLight()
{
}

void PointLight::draw()
{
    int stacks = 4, segments = 10;
    Point points[stacks + 1][segments + 1];
    double h, r;

    for (int i = 0; i <= stacks; i++)
    {
        h = sin(((double)i / (double)stacks) * (pi / 2));
        r = cos(((double)i / (double)stacks) * (pi / 2));

        for (int j = 0; j <= segments; j++)
        {
            points[i][j] = Point(r * cos(((double)j / (double)segments) * 2 * pi), r * sin(((double)j / (double)segments) * 2 * pi), h);
        }
    }

    glColor3f(color.red, color.green, color.blue);

    for (int i = 0; i < stacks; i++)
    {
        for (int j = 0; j < segments; j++)
        {
            glBegin(GL_QUADS);
            {
                /* upper hemisphere */
                glVertex3f(light_pos.addVectorCopy(points[i][j]).x, light_pos.addVectorCopy(points[i][j]).y, light_pos.addVectorCopy(points[i][j]).z);
                glVertex3f(light_pos.addVectorCopy(points[i][j+1]).x, light_pos.addVectorCopy(points[i][j+1]).y, light_pos.addVectorCopy(points[i][j+1]).z);
                glVertex3f(light_pos.addVectorCopy(points[i+1][j+1]).x, light_pos.addVectorCopy(points[i+1][j+1]).y, light_pos.addVectorCopy(points[i+1][j+1]).z);
                glVertex3f(light_pos.addVectorCopy(points[i+1][j]).x, light_pos.addVectorCopy(points[i+1][j]).y, light_pos.addVectorCopy(points[i+1][j]).z);

                /* lower hemisphere */
                glVertex3f(light_pos.addVectorCopy(points[i][j]).x, light_pos.addVectorCopy(points[i][j]).y, light_pos.addVectorCopy(points[i][j], -1).z);
                glVertex3f(light_pos.addVectorCopy(points[i][j+1]).x, light_pos.addVectorCopy(points[i][j+1]).y, light_pos.addVectorCopy(points[i][j+1], -1).z);
                glVertex3f(light_pos.addVectorCopy(points[i+1][j+1]).x, light_pos.addVectorCopy(points[i+1][j+1]).y, light_pos.addVectorCopy(points[i+1][j+1], -1).z);
                glVertex3f(light_pos.addVectorCopy(points[i+1][j]).x, light_pos.addVectorCopy(points[i+1][j]).y, light_pos.addVectorCopy(points[i+1][j], -1).z);
            }
            glEnd();
        }
    }
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
    SpotLight(PointLight point_light, Point light_direction, double cutoff_angle)
    {
        this->point_light = point_light;
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
    }
    Point getLightPosition()
    {
        return this->point_light.getLightPosition();
    }
    Color getPointColor()
    {
        return this->point_light.getPointColor();
    }
    double getCutOffAngle() {
        return this->cutoff_angle;
    }
    Point getLightDir()
    {
        return this->light_direction;
    }
    ~SpotLight();
};

SpotLight::SpotLight(/* args */)
{
}

SpotLight::~SpotLight()
{
}

void computeAmbientComponent(Color &color, Color pointColor, ReflectionCoeff rfcoeff)
{
    color.red = pointColor.red * rfcoeff.ambientCoeff;
    color.green = pointColor.green * rfcoeff.ambientCoeff;
    color.blue = pointColor.blue * rfcoeff.ambientCoeff;
}

void calculateReflectionComponents(
    Ray ray,
    Color &color,
    Point surfaceNormal,
    PointLight light,
    Ray incidentRay,
    Point intersectionPoint,
    Color pointColor, // intersection point color
    ReflectionCoeff rfcoeff,
    int shine)
{
    Point Rd = incidentRay.getRD();
    double lambertVal = Rd.scalarMultCopy(-1.0).dotProduct(surfaceNormal);

    Point reflectedRayDir = incidentRay.getRD();
    double normal_scale = 2.0 * incidentRay.getRD().dotProduct(surfaceNormal);
    reflectedRayDir.addVector(surfaceNormal.scalarMultCopy(normal_scale), -1);

    // Ray reflectedRay(intersectionPoint, reflectedRayDir);

    double phongValue = ray.getRD().scalarMultCopy(-1.0).dotProduct(reflectedRayDir);

    Color lightColor = light.getPointColor();
    double s = std::max(lambertVal, 0.0);
    color.red += lightColor.red * pointColor.red * rfcoeff.diffuseCoeff * s;
    color.green += lightColor.green * pointColor.green * rfcoeff.diffuseCoeff * s;
    color.blue += lightColor.blue * pointColor.blue * rfcoeff.diffuseCoeff * s;

    s = std::max(phongValue, 0.0);
    s = pow(s, shine);

    color.red += lightColor.red * pointColor.red * rfcoeff.specularCoeff * s;
    color.green += lightColor.green * pointColor.green * rfcoeff.specularCoeff * s;
    color.blue += lightColor.blue * pointColor.blue * rfcoeff.specularCoeff * s;
}

void calculateReflectionComponentsSpot(
    Ray ray,
    Color &color,
    Point surfaceNormal,
    SpotLight light,
    Ray incidentRay,
    Point intersectionPoint,
    Color pointColor, // intersection point color
    ReflectionCoeff rfcoeff,
    int shine)
{
    Point Rd = incidentRay.getRD();
    double lambertVal = Rd.scalarMultCopy(-1.0).dotProduct(surfaceNormal);

    Point reflectedRayDir = incidentRay.getRD();
    double normal_scale = 2.0 * incidentRay.getRD().dotProduct(surfaceNormal);
    reflectedRayDir.addVector(surfaceNormal.scalarMultCopy(normal_scale), -1);

    // Ray reflectedRay(intersectionPoint, reflectedRayDir);

    double phongValue = ray.getRD().scalarMultCopy(-1.0).dotProduct(reflectedRayDir);

    Color lightColor = light.getPointColor();
    double s = std::max(lambertVal, 0.0);
    color.red += lightColor.red * pointColor.red * rfcoeff.diffuseCoeff * s;
    color.green += lightColor.green * pointColor.green * rfcoeff.diffuseCoeff * s;
    color.blue += lightColor.blue * pointColor.blue * rfcoeff.diffuseCoeff * s;

    s = std::max(phongValue, 0.0);
    s = pow(s, shine);

    color.red += lightColor.red * pointColor.red * rfcoeff.specularCoeff * s;
    color.green += lightColor.green * pointColor.green * rfcoeff.specularCoeff * s;
    color.blue += lightColor.blue * pointColor.blue * rfcoeff.specularCoeff * s;
}

void calculateRecursiveReflectionComponents(Color &color, Color reflectedColor, ReflectionCoeff rfcoeff)
{
    color.red += reflectedColor.red * rfcoeff.recursiveCoeff;
    color.green += reflectedColor.green * rfcoeff.recursiveCoeff;
    color.blue += reflectedColor.blue * rfcoeff.recursiveCoeff;
}

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
    Object(/* args */)
    {
        shine = 0;
    }

    Color getColor() const
    {
        return color;
    }

    void setColor(Color color)
    {
        this->color = color;
    }

    ReflectionCoeff getReflectionCoefficient()
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

    virtual void draw() = 0; // pure virtual function
    virtual double intersect(Ray, Color &, int) = 0;
    ~Object()
    {
    }
};

std::vector<Object *> objects;
std::vector<PointLight> pointLights;
std::vector<SpotLight> spotLights;

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
    Point points[stacks + 1][segments + 1];
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
                glVertex3f(reference_point.addVectorCopy(points[i][j]).x, reference_point.addVectorCopy(points[i][j]).y, reference_point.addVectorCopy(points[i][j]).z);
                glVertex3f(reference_point.addVectorCopy(points[i][j+1]).x, reference_point.addVectorCopy(points[i][j+1]).y, reference_point.addVectorCopy(points[i][j+1]).z);
                glVertex3f(reference_point.addVectorCopy(points[i+1][j+1]).x, reference_point.addVectorCopy(points[i+1][j+1]).y, reference_point.addVectorCopy(points[i+1][j+1]).z);
                glVertex3f(reference_point.addVectorCopy(points[i+1][j]).x, reference_point.addVectorCopy(points[i+1][j]).y, reference_point.addVectorCopy(points[i+1][j]).z);
                // lower hemisphere

                glVertex3f(reference_point.addVectorCopy(points[i][j]).x, reference_point.addVectorCopy(points[i][j]).y, reference_point.addVectorCopy(points[i][j], -1).z);
                glVertex3f(reference_point.addVectorCopy(points[i][j+1]).x, reference_point.addVectorCopy(points[i][j+1]).y, reference_point.addVectorCopy(points[i][j+1], -1).z);
                glVertex3f(reference_point.addVectorCopy(points[i+1][j+1]).x, reference_point.addVectorCopy(points[i+1][j+1]).y, reference_point.addVectorCopy(points[i+1][j+1], -1).z);
                glVertex3f(reference_point.addVectorCopy(points[i+1][j]).x, reference_point.addVectorCopy(points[i+1][j]).y, reference_point.addVectorCopy(points[i+1][j], -1).z);
            }
            glEnd();
        }
    }
}

double Sphere::intersect(Ray ray, Color &color, int level)
{
    double a, b, c, tMin;
    Point Ro = ray.getRO();
    Point Rd = ray.getRD();
    Point C = this->reference_point.returnCopy();

    a = 1.0;
    b = (Rd.dotProduct(Ro) - Rd.dotProduct(C)) * 2.0;
    c = Ro.dotProduct(Ro) + C.dotProduct(C) - 2.0 * (C.dotProduct(Ro)) - this->length * this->length;

    double d = b * b - 4.0 * c;

    if (d < 0.0)
    {
        tMin = INF; // NOTICE: how to mitigate this corner case?
    }
    else if (d > 0.0)
    {
        tMin = -b / (2.0 * a) - sqrt(d) / (2.0 * a);

        if (tMin < 0.0)
        {
            tMin = -b / (2.0 * a) + sqrt(d) / (2.0 * a);
        }
    }
    else
    {
        tMin = -b / (2.0 * a);
    }

    if (level == 0)
    {
        return tMin;
    }

    Rd.scalarMult(tMin);
    Ro.addVector(Rd);
    Point intersectionPoint = Ro.returnCopy();

    computeAmbientComponent(color, this->color, this->rfco);

    /* determining unit normal vector at intersection point on object's surface */
    Point surfaceNormal = intersectionPoint.returnCopy();
    surfaceNormal.addVector(C, -1);
    surfaceNormal.normalize();

    if (distanceBetweenTwoPoints(pos, C) > this->length)
    {
        surfaceNormal.scalarMult(-1.0);
    }

    /* computing diffuse & specular reflection components of reflected ray */
    for (int i = 0; i < pointLights.size(); i++)
    {
        Point light_position = pointLights[i].getLightPosition();
        Point light_dir = intersectionPoint.returnCopy();
        light_dir.addVector(light_position, -1);

        Ray incidentRay(light_position, light_dir);

        /* checking if intersection point is in shadow */
        double t, tMin_ = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            Color dummyColor; // color = black
            t = objects[j]->intersect(incidentRay, dummyColor, 0);

            if (t > 0.0 && t < tMin_)
            {
                tMin_ = t;
            }
        }

        // shadowIntersectionPoint = incidentRay[Ro]+incidentRay[RD]*tMin_;
        Point shadowIntersectionPoint = light_position.returnCopy();
        Point shadowIP_Rd = light_dir.returnCopy();
        shadowIP_Rd.scalarMult(tMin_);
        shadowIntersectionPoint.addVector(shadowIP_Rd);

        double epsilon = 0.0000001; // for tuning light effect

        // distance between incident ray origin and intersection point
        double d_IR_IP = distanceBetweenTwoPoints(intersectionPoint, incidentRay.getRO());

        // distance between incident ray origin and shadow intersection point
        double d_IR_SIP = distanceBetweenTwoPoints(shadowIntersectionPoint, incidentRay.getRO());

        if (d_IR_IP - epsilon <= d_IR_SIP)
        {
            calculateReflectionComponents(
                ray,
                color,
                surfaceNormal,
                pointLights[i],
                incidentRay,
                intersectionPoint,
                this->color, // intersection point color
                this->rfco,
                this->shine);
        }

    }

    for (int i = 0; i < spotLights.size(); i++)
    {
        Point spotLightPos = spotLights[i].getLightPosition();
        Point lightDir = intersectionPoint.addVectorCopy(spotLightPos, -1);
        
        Point spotLightDir = spotLights[i].getLightDir();

        double angle = acos(spotLightDir.dotProduct(spotLightDir));

        if (angle>spotLights[i].getCutOffAngle()) continue;
        
        Point light_position = spotLights[i].getLightPosition();
        // Point light_dir = intersectionPoint.returnCopy();
        // light_dir.addVector(light_position, -1);
        Point light_dir = spotLightDir;

        Ray incidentRay(light_position, light_dir);

        double t, tMinimum = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            Color dummyColor; // color = black
            t = objects[j]->intersect(incidentRay, dummyColor, 0);

            if (t > 0.0 && t < tMinimum)
            {
                tMinimum = t;
            }
        }


        // shadowIntersectionPoint = incidentRay[Ro]+incidentRay[RD]*tMin_;
        Point shadowIntersectionPoint = incidentRay.getRO().addVectorCopy(incidentRay.getRD().scalarMultCopy(tMinimum));
        double epsilon = 0.0000001; // for tuning light effect

        // distance between incident ray origin and intersection point
        double d_IR_IP = distanceBetweenTwoPoints(intersectionPoint, incidentRay.getRO());

        // distance between incident ray origin and shadow intersection point
        double d_IR_SIP = distanceBetweenTwoPoints(shadowIntersectionPoint, incidentRay.getRO());

        if (d_IR_IP - epsilon <= d_IR_SIP)
        {
            calculateReflectionComponentsSpot(
                ray,
                color,
                surfaceNormal,
                spotLights[i],
                incidentRay,
                intersectionPoint,
                spotLights[i].getPointColor(), // intersection point color
                this->rfco,
                this->shine);
        }

    }



    /* handling recursive reflection */
    if (level >= recursionLevel)
    {
        return tMin;
    }

    /* incorporating concept of evil epsilon to recursive reflection computation */
    Point reflectedDir = ray.getRD();
    reflectedDir.addVector(surfaceNormal.scalarMultCopy(ray.getRD().dotProduct(surfaceNormal) * 2.0), -1);

    reflectedDir.normalize();
    Point reflectedOrigin = intersectionPoint.returnCopy();
    reflectedOrigin.addVector(reflectedDir);
    Ray reflectedRay(reflectedOrigin, reflectedDir);

    /* finding nearest intersecting object (if available) */
    int nearest = INT32_MAX;
    double t, tMinimum = INF;

    for (int i = 0; i < objects.size(); i++)
    {
        Color dummyColor; // color = black
        t = objects[i]->intersect(reflectedRay, dummyColor, 0);

        if (t > 0.0 && t < tMinimum)
        {
            tMinimum = t;
            nearest = i;
        }
    }

    /* finding color component for reflected ray */
    Color reflectedColor; // color = black

    if (nearest != INT32_MAX)
    {
        tMinimum = objects[nearest]->intersect(reflectedRay, reflectedColor, level + 1);
    }

    /* computing recursive reflection component of reflected ray */
    calculateRecursiveReflectionComponents(color, reflectedColor, this->rfco);

    /* clipping the color values (if necessary) */
    color.red = (color.red > 1.0) ? 1.0 : ((color.red < 0.0) ? 0.0 : color.red);
    color.green = (color.green > 1.0) ? 1.0 : ((color.green < 0.0) ? 0.0 : color.green);
    color.blue = (color.blue > 1.0) ? 1.0 : ((color.blue < 0.0) ? 0.0 : color.blue);

    return tMin;
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
    double detA, beta, gamma, T, tMin;

    Point a = this->reference_point;
    Point b = this->reference_point2;
    Point c = this->reference_point3;

    double ax = a.x;
    double ay = a.y;
    double az = a.z;

    double bx = b.x;
    double by = b.y;
    double bz = b.z;

    double cx = c.x;
    double cy = c.y;
    double cz = c.z;

    double Ro_x = ray.getRO().x;
    double Ro_y = ray.getRO().y;
    double Ro_z = ray.getRO().z;

    double Rd_x = ray.getRD().x;
    double Rd_y = ray.getRD().y;
    double Rd_z = ray.getRD().z;

    beta = (ax - Ro_x) * ((ay - cy) * Rd_z - (az - cz) * Rd_y) +
           (ax - cx) * ((az - Ro_z) * Rd_y - (ay - Ro_y) * Rd_z) +
           Rd_x * ((ay - Ro_y) * (az - cz) - (az - Ro_z) * (ay - cy));

    gamma = (ax - bx) * ((ay - Ro_y) * Rd_z - (az - Ro_z) * Rd_y) +
            (ax - Ro_x) * ((az - bz) * Rd_y - (ay - by) * Rd_z) +
            Rd_x * ((ay - by) * (az - Ro_z) - (az - bz) * (ay - Ro_y));

    T = (ax - bx) * ((ay - cy) * (az - Ro_z) - (az - cz) * (ay - Ro_y)) +
        (ax - cx) * ((ay - Ro_y) * (az - bz) - (ay - by) * (az - Ro_z)) +
        (ax - Ro_x) * ((ay - by) * (az - cz) - (ay - cy) * (az - bz));

    detA = (ax - bx) * ((ay - cy) * Rd_z - (az - cz) * Rd_y) +
           (ax - cx) * ((az - bz) * Rd_y - (ay - by) * Rd_z) +
           Rd_x * ((ay - by) * (az - cz) - (az - bz) * (ay - cy));

    // if(f<1000)
    // {
    //     cout<<beta<<" "<<gamma<<" "<<T<<" "<<detA<<endl;
    //     f++;
    // }
 
    if (detA == 0.0)
    {
        /* ray will not intersect the triangle plane */
        tMin = INF;
    }
    else
    {
        /* ray will intersect the triangle plane */
        if (beta / detA > 0.0 && gamma / detA > 0.0 && (beta / detA + gamma / detA) < 1.0)
        {
            /* intersection point lies within the boundary of the triangle */
            tMin = T / detA;
        }
        else
        {
            /* intersection point does not lie within the boundary of the triangle */
            tMin = INF;
        }
    }

    if (level == 0)
    {
        return tMin;
    }

    /* illuminating with Phong Lighting Model */
    Point intersectionPoint = ray.getRO();
    intersectionPoint.addVector(ray.getRD().scalarMultCopy(tMin));

    /* determining unit normal vector on appropriate side of triangle */
    
    b.addVector(a, -1);
    c.addVector(a, -1);
    Point surfaceNormal = b.crossProduct(c);
    surfaceNormal.normalize();

    if (ray.getRD().scalarMultCopy(-1.0).dotProduct(surfaceNormal) < 0.0)
    {
        surfaceNormal.scalarMult(-1.0);
    }

    /* computing ambient light component of reflected ray */
    computeAmbientComponent(color, this->color, this->rfco);

    /* computing diffuse & specular reflection components of reflected ray */
    for (int i = 0; i < pointLights.size(); i++)
    {
        Point lightPos = pointLights[i].getLightPosition();
        Point lightDir = intersectionPoint.returnCopy();
        lightDir.addVector(lightPos, -1);

        Ray incidentRay(lightPos, lightDir);

        double t, tMinimum = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            Color dummyColor; // color = black
            t = objects[j]->intersect(incidentRay, dummyColor, 0);

            if (t > 0.0 && t < tMinimum)
            {
                tMinimum = t;
            }
        }

        Point shadowIntersectionPoint = incidentRay.getRO().addVectorCopy(incidentRay.getRD().scalarMultCopy(tMinimum));
        double epsilon = 0.0000001; // for tuning light effect

        // distance between incident ray origin and intersection point
        double d_IR_IP = distanceBetweenTwoPoints(intersectionPoint, incidentRay.getRO());

        // distance between incident ray origin and shadow intersection point
        double d_IR_SIP = distanceBetweenTwoPoints(shadowIntersectionPoint, incidentRay.getRO());

        if (d_IR_IP - epsilon <= d_IR_SIP)
        {
            calculateReflectionComponents(
                ray,
                color,
                surfaceNormal,
                pointLights[i],
                incidentRay,
                intersectionPoint,
                this->color, // intersection point color
                this->rfco,
                this->shine);
        }
    }

    for (int i = 0; i < spotLights.size(); i++)
    {
        Point spotLightPos = spotLights[i].getLightPosition();
        Point lightDir = intersectionPoint.addVectorCopy(spotLightPos, -1);
        
        Point spotLightDir = spotLights[i].getLightDir();

        double angle = acos(spotLightDir.dotProduct(spotLightDir));

        if (angle>spotLights[i].getCutOffAngle()) continue;
        
        Point light_position = spotLights[i].getLightPosition();
        // Point light_dir = intersectionPoint.returnCopy();
        // light_dir.addVector(light_position, -1);
        Point light_dir = spotLightDir;

        Ray incidentRay(light_position, light_dir);

        double t, tMinimum = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            Color dummyColor; // color = black
            t = objects[j]->intersect(incidentRay, dummyColor, 0);

            if (t > 0.0 && t < tMinimum)
            {
                tMinimum = t;
            }
        }


        // shadowIntersectionPoint = incidentRay[Ro]+incidentRay[RD]*tMin_;
        Point shadowIntersectionPoint = incidentRay.getRO().addVectorCopy(incidentRay.getRD().scalarMultCopy(tMinimum));
        double epsilon = 0.0000001; // for tuning light effect

        // distance between incident ray origin and intersection point
        double d_IR_IP = distanceBetweenTwoPoints(intersectionPoint, incidentRay.getRO());

        // distance between incident ray origin and shadow intersection point
        double d_IR_SIP = distanceBetweenTwoPoints(shadowIntersectionPoint, incidentRay.getRO());

        if (d_IR_IP - epsilon <= d_IR_SIP)
        {
            calculateReflectionComponentsSpot(
                ray,
                color,
                surfaceNormal,
                spotLights[i],
                incidentRay,
                intersectionPoint,
                spotLights[i].getPointColor(), // intersection point color
                this->rfco,
                this->shine);
        }

    }

    /* handling recursive reflection */
    if (level >= recursionLevel)
    {
        return tMin;
    }

    /* incorporating concept of evil epsilon to recursive reflection computation */
    Point reflectedDir = ray.getRD();
    reflectedDir.addVector(surfaceNormal.scalarMultCopy(ray.getRD().dotProduct(surfaceNormal) * 2.0), -1);

    reflectedDir.normalize();
    Point reflectedOrigin = intersectionPoint.returnCopy();
    reflectedOrigin.addVector(reflectedDir);
    Ray reflectedRay(reflectedOrigin, reflectedDir);

    /* finding nearest intersecting object (if available) */
    int nearest = INT32_MAX;
    double t, tMinimum = INF;

    for (int i = 0; i < objects.size(); i++)
    {
        Color tempColor; // color = black
        t = objects[i]->intersect(reflectedRay, tempColor, 0);

        if (t > 0.0 && t < tMinimum)
        {
            tMinimum = t;
            nearest = i;
        }
    }

    /* finding color component for reflected ray */
    Color reflectedColor; // color = black

    if (nearest != INT32_MAX)
    {
        tMinimum = objects[nearest]->intersect(reflectedRay, reflectedColor, level + 1);
    }

    /* computing recursive reflection component of reflected ray */
    calculateRecursiveReflectionComponents(color, reflectedColor, this->rfco);

    /* clipping the color values (if necessary) */
    color.red = (color.red > 1.0) ? 1.0 : ((color.red < 0.0) ? 0.0 : color.red);
    color.green = (color.green > 1.0) ? 1.0 : ((color.green < 0.0) ? 0.0 : color.green);
    color.blue = (color.blue > 1.0) ? 1.0 : ((color.blue < 0.0) ? 0.0 : color.blue);

    return tMin;
}

Triangle::~Triangle()
{
}

struct gQuadCoeff
{
    double a, b, c, d, e, f, g, h, i, j;
};

class GeneralQuadSurface : public Object
{
private:
    /* data */
    struct gQuadCoeff gQCoeff;

public:
    GeneralQuadSurface(/* args */);
    GeneralQuadSurface(gQuadCoeff gQCoeff, double length, double width, double height, Point referencePoint);
    void draw() {}
    double intersect(Ray, Color &, int);
    ~GeneralQuadSurface();
};

GeneralQuadSurface::GeneralQuadSurface(/* args */)
{
    gQCoeff.a = gQCoeff.b = gQCoeff.c = gQCoeff.d = gQCoeff.e = gQCoeff.f = gQCoeff.g = gQCoeff.h = gQCoeff.i = gQCoeff.j = 0.0;
    this->length = this->width = this->height = 0;
}

GeneralQuadSurface::GeneralQuadSurface(gQuadCoeff gQCoeff, double length, double width, double height, Point referencePoint)
{
    this->gQCoeff = gQCoeff;
    this->length = length;
    this->height = height;
    this->width = width;
    this->reference_point = reference_point;
}

double GeneralQuadSurface::intersect(Ray ray, Color &color, int level)
{
    // reference: http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html

    double Aq, Bq, Cq, tMin, tMax;
    double A, B, C, D, E, F, G, H, I, J;
    A = this->gQCoeff.a;
    B = this->gQCoeff.b;
    C = this->gQCoeff.c;
    D = this->gQCoeff.d;
    E = this->gQCoeff.e;
    F = this->gQCoeff.f;
    G = this->gQCoeff.g;
    H = this->gQCoeff.h;
    I = this->gQCoeff.i;
    J = this->gQCoeff.j;

    double xd, yd, zd, xo, yo, zo;
    Point Ro = ray.getRO();
    Point Rd = ray.getRD();
    xd = Rd.x;
    yd = Rd.y;
    zd = Rd.z;
    xo = Ro.x;
    yo = Ro.y;
    zo = Ro.z;

    Aq = A * pow(xd, 2) + B * pow(yd, 2) + C * pow(zd, 2) + D * xd * yd + E * xd * zd + F * yd * zd;

    Bq = 2 * A * xo * xd + 2 * B * yo * yd + 2 * C * zo * zd +
         D * (xo * yd + yo * xd) + E * (xo * zd + zo * xd) +
         F * (yo * zd + yd * zo) + G * xd + H * yd + I * zd;

    Cq = A * pow(xo, 2) + B * pow(yo, 2) + C * pow(zo, 2) +
         D * xo * yo + E * xo * zo + F * yo * zo +
         G * xo + H * yo + I * zo + J;

    // if (f<200)
    // {
    //     cout<<Aq<<" "<<Bq<<" "<<Cq<<endl;
    //     f++;
    // }

    if (Aq == 0.0)
    {
        tMin = INF;
        if (Bq != 0.0)
        {
            tMin = -Cq / Bq;
        }
        tMax = INF;
    }
    else
    {
        double discriminant = (Bq * Bq) - (4.0 * Aq * Cq);

        if (discriminant < 0.0)
        {
            tMin = tMax = INF;
        }
        else if (discriminant > 0.0)
        {
            tMax = -Bq / (2.0 * Aq) + sqrt(discriminant) / (2.0 * Aq);
            tMin = -Bq / (2.0 * Aq) - sqrt(discriminant) / (2.0 * Aq);
        }
        else
        {
            tMin = -Bq / (2.0 * Aq);
            tMax = INF;
        }
    }

    /* clipping general quadric surface along the dimensions (if necessary) */
    if (tMin < INF)
    {
        if (tMax < INF)
        {
            if (tMin > 0.0)
            {
                Point intersectionPoint = ray.getRO().addVectorCopy(ray.getRD().scalarMultCopy(tMin));

                if ((length != 0.0 && (intersectionPoint.x < reference_point.x ||
                                       intersectionPoint.x > reference_point.x + length)) ||
                    (width != 0.0 && (intersectionPoint.y < reference_point.y ||
                                      intersectionPoint.y > reference_point.y + width)) ||
                    (height != 0.0 && (intersectionPoint.z < reference_point.z ||
                                       intersectionPoint.z > reference_point.z + height)))
                {
                    tMin = INF;
                }
            }
            if (tMax > 0.0)
            {
                Point intersectionPoint = ray.getRO().addVectorCopy(ray.getRD().scalarMultCopy(tMax));

                if ((length != 0.0 && (intersectionPoint.x < reference_point.x ||
                                       intersectionPoint.x > reference_point.x + length)) ||
                    (width != 0.0 && (intersectionPoint.y < reference_point.y ||
                                      intersectionPoint.y > reference_point.y + width)) ||
                    (height != 0.0 && (intersectionPoint.z < reference_point.z ||
                                       intersectionPoint.z > reference_point.z + height)))
                {
                    tMax = INF;
                }
            }
            tMin = (tMin > 0.0 && tMin < tMax) ? tMin : tMax;
        }
        else
        {
            if (tMin > 0.0)
            {
                Point intersectionPoint = ray.getRO().addVectorCopy(ray.getRD().scalarMultCopy(tMin));

                if ((length != 0.0 && (intersectionPoint.x < reference_point.x ||
                                       intersectionPoint.x > reference_point.x + length)) ||
                    (width != 0.0 && (intersectionPoint.y < reference_point.y ||
                                      intersectionPoint.y > reference_point.y + width)) ||
                    (height != 0.0 && (intersectionPoint.z < reference_point.z ||
                                       intersectionPoint.z > reference_point.z + height)))
                {
                    tMin = INF;
                }
            }
        }
    }

    // if (f<200)
    // {
    //     cout<<tMin<<endl;
    //     f++;
    // }

    if (level == 0)
    {
        return tMin;
    }

    // if (tMin != INF)
    // {
    //     cout<<tMin<<endl;
    // }

    Point intersectionPoint = ray.getRO().addVectorCopy(ray.getRD().scalarMultCopy(tMin));

    double x_ip = intersectionPoint.x;
    double y_ip = intersectionPoint.y;
    double z_ip = intersectionPoint.z;

    double x_norm, y_norm, z_norm;

    x_norm = 2.0 * A * x_ip + D * y_ip + E * z_ip + G;
    y_norm = 2.0 * B * y_ip + D * x_ip + F * z_ip + H;
    z_norm = 2.0 * C * z_ip + E * x_ip + F * y_ip + I;

    // if (f<200)
    // {
    //     // cout<<x_norm<<" "<<y_norm<<" "<<z_norm<<endl;
    //     cout<<x_ip<<" "<<y_ip<<" "<<z_ip<<" "<<tMin<<endl;
    //     f++;
    // }

    Point surfaceNormal(x_norm, y_norm, z_norm);
    surfaceNormal.normalize();

    if (ray.getRD().scalarMultCopy(-1.0).dotProduct(surfaceNormal) < 0.0)
    {
        surfaceNormal.scalarMult(-1.0);
    }

    /* computing ambient light component of reflected ray */
    computeAmbientComponent(color, this->color, this->rfco);

    /* computing diffuse & specular reflection components of reflected ray */
    for (int i = 0; i < pointLights.size(); i++)
    {
        Point lightPos = pointLights[i].getLightPosition();
        Point lightDir = intersectionPoint.returnCopy();
        lightDir.addVector(lightPos, -1);

        Ray incidentRay(lightPos, lightDir);

        /* checking if intersection point is in shadow */
        double t, tMinimum = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            Color dummyColor; // color = black
            t = objects[j]->intersect(incidentRay, dummyColor, 0);

            if (t > 0.0 && t < tMinimum)
            {
                tMinimum = t;
            }
        }

        Point shadowIntersectionPoint = incidentRay.getRO().addVectorCopy(incidentRay.getRD().scalarMultCopy(tMinimum));
        double epsilon = 0.0000001; // for tuning light effect

        // distance between incident ray origin and intersection point
        double d_IR_IP = distanceBetweenTwoPoints(intersectionPoint, incidentRay.getRO());

        // distance between incident ray origin and shadow intersection point
        double d_IR_SIP = distanceBetweenTwoPoints(shadowIntersectionPoint, incidentRay.getRO());

        if (d_IR_IP - epsilon <= d_IR_SIP)
        {
            calculateReflectionComponents(
                ray,
                color,
                surfaceNormal,
                pointLights[i],
                incidentRay,
                intersectionPoint,
                this->color, // intersection point color
                this->rfco,
                this->shine);
        }
    }

    for (int i = 0; i < spotLights.size(); i++)
    {
        Point spotLightPos = spotLights[i].getLightPosition();
        Point lightDir = intersectionPoint.addVectorCopy(spotLightPos, -1);
        
        Point spotLightDir = spotLights[i].getLightDir();

        double angle = acos(spotLightDir.dotProduct(spotLightDir));

        if (angle>spotLights[i].getCutOffAngle()) continue;
        
        Point light_position = spotLights[i].getLightPosition();
        // Point light_dir = intersectionPoint.returnCopy();
        // light_dir.addVector(light_position, -1);
        Point light_dir = spotLightDir;

        Ray incidentRay(light_position, light_dir);

        double t, tMinimum = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            Color dummyColor; // color = black
            t = objects[j]->intersect(incidentRay, dummyColor, 0);

            if (t > 0.0 && t < tMinimum)
            {
                tMinimum = t;
            }
        }


        // shadowIntersectionPoint = incidentRay[Ro]+incidentRay[RD]*tMin_;
        Point shadowIntersectionPoint = incidentRay.getRO().addVectorCopy(incidentRay.getRD().scalarMultCopy(tMinimum));
        double epsilon = 0.0000001; // for tuning light effect

        // distance between incident ray origin and intersection point
        double d_IR_IP = distanceBetweenTwoPoints(intersectionPoint, incidentRay.getRO());

        // distance between incident ray origin and shadow intersection point
        double d_IR_SIP = distanceBetweenTwoPoints(shadowIntersectionPoint, incidentRay.getRO());

        if (d_IR_IP - epsilon <= d_IR_SIP)
        {
            calculateReflectionComponentsSpot(
                ray,
                color,
                surfaceNormal,
                spotLights[i],
                incidentRay,
                intersectionPoint,
                spotLights[i].getPointColor(), // intersection point color
                this->rfco,
                this->shine);
        }

    }

    /* handling recursive reflection */
    if (level >= recursionLevel)
    {
        return tMin;
    }

    /* incorporating concept of evil epsilon to recursive reflection computation */
    Point reflectedDir = ray.getRD();
    reflectedDir.addVector(surfaceNormal.scalarMultCopy(ray.getRD().dotProduct(surfaceNormal) * 2.0), -1);

    reflectedDir.normalize();
    Point reflectedOrigin = intersectionPoint.returnCopy();
    reflectedOrigin.addVector(reflectedDir);
    Ray reflectedRay(reflectedOrigin, reflectedDir);

    /* finding nearest intersecting object (if available) */
    int nearest = INT32_MAX;
    double t, tMinimum = INF;

    for (int i = 0; i < objects.size(); i++)
    {
        Color tempColor; // color = black
        t = objects[i]->intersect(reflectedRay, tempColor, 0);

        if (t > 0.0 && t < tMinimum)
        {
            tMinimum = t;
            nearest = i;
        }
    }

    /* finding color component for reflected ray */
    Color reflectedColor; // color = black

    if (nearest != INT32_MAX)
    {
        tMinimum = objects[nearest]->intersect(reflectedRay, reflectedColor, level + 1);
    }

    /* computing recursive reflection component of reflected ray */
    calculateRecursiveReflectionComponents(color, reflectedColor, this->rfco);

    /* clipping the color values (if necessary) */
    color.red = (color.red > 1.0) ? 1.0 : ((color.red < 0.0) ? 0.0 : color.red);
    color.green = (color.green > 1.0) ? 1.0 : ((color.green < 0.0) ? 0.0 : color.green);
    color.blue = (color.blue > 1.0) ? 1.0 : ((color.blue < 0.0) ? 0.0 : color.blue);

    return tMin;
}

GeneralQuadSurface::~GeneralQuadSurface()
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
    this->type = "Floor";
}

Floor::Floor(double floorwidth, double tilewidth)
{
    // reference_point.x = -floorwidth / 2;
    // reference_point.y = -floorwidth / 2;
    // reference_point.z = 0.0;
    this->width = tilewidth;
    this->length = floorwidth;
    this->type = "Floor";
}

void Floor::draw()
{
    int num_row = (int)length / width; // floorwidth/tilewidth
    int num_col = num_row;
    for (int i = 0; i < num_row; i++)
    {
        for (int j = 0; j < num_col; j++)
        {
            /* drawing square on a plane parallel to x-y plane */
            Color c;
            if ((i + j) % 2 == 0)
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
            leftBottomCorner.x = -length / 2.0 + width * j;
            leftBottomCorner.y = -length / 2.0 + width * i;
            leftBottomCorner.z = 0.0;

            glBegin(GL_QUADS);
            {
                glVertex3f(leftBottomCorner.x, leftBottomCorner.y, leftBottomCorner.z);
                glVertex3f(leftBottomCorner.x + width, leftBottomCorner.y, leftBottomCorner.z);
                glVertex3f(leftBottomCorner.x + width, leftBottomCorner.y + width, leftBottomCorner.z);
                glVertex3f(leftBottomCorner.x, leftBottomCorner.y + width, leftBottomCorner.z);
            }
            glEnd();
        }
    }
}

double Floor::intersect(Ray ray, Color &color, int level)
{
    double floorWidth = this->length;
    double tileWidth = this->width;
    Point surfaceNormal(0.0, 0.0, 1.0);
    if (pos.dotProduct(surfaceNormal) < 0.0)
    {
        surfaceNormal.scalarMult(-1.0);
    }
    double tMin = INF;

    if (ray.getRD().dotProduct(surfaceNormal) != 0.0)
    {
        tMin = (-1.0 * ray.getRO().dotProduct(surfaceNormal)) / ray.getRD().dotProduct(surfaceNormal);
    }

    

    if (tMin > 0.0 && tMin < INF)
    {
        // make sure intersection point is on the floor
        Point intersectionPoint = ray.getRO().addVectorCopy(ray.getRD().scalarMultCopy(tMin));

        if (!((intersectionPoint.x > -floorWidth / 2.0 && intersectionPoint.x < floorWidth / 2.0) &&
              (intersectionPoint.y > -floorWidth / 2.0 && intersectionPoint.y < floorWidth / 2.0)))
        {
            /* intersection point is not on the floor */
            tMin = INF;
        }
    }

    if (level == 0)
    {
        return tMin;
    }

    Point d(-floorWidth / 2.0, -floorWidth / 2.0, 0.0);
    Point intersectionPoint = ray.getRO().addVectorCopy(ray.getRD().scalarMultCopy(tMin));
    Point referencePos = intersectionPoint.addVectorCopy(d, -1);
    // NOTICE HERE
    Color iPointColor;
    int i = floor(referencePos.x / tileWidth);
    int j = floor(referencePos.y / tileWidth);
    if ((i + j) % 2 == 0)
    {
        // iPointColor = this->color;
        iPointColor.red = 0;
        iPointColor.green = 0;
        iPointColor.blue = 0;
    }
    else
    {
        // iPointColor.red = 1.0 - this->color.red;
        // iPointColor.green = 1.0 - this->color.green;
        // iPointColor.blue = 1.0 - this->color.blue;
        // iPointColor = this->color;

        iPointColor.red = 1.0;
        iPointColor.green = 1.0;
        iPointColor.blue = 1.0;
    }

    /* computing ambient light component of reflected ray */
    computeAmbientComponent(color, iPointColor, this->rfco);

    /* computing diffuse & specular reflection components of reflected ray */
    for (int i = 0; i < pointLights.size(); i++)
    {
        Point lightPos = pointLights[i].getLightPosition();
        Point lightDir = intersectionPoint.addVectorCopy(lightPos, -1);

        Ray incidentRay(lightPos, lightDir);

        /* checking if intersection point is in shadow */
        double t, tMinimum = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            Color tempColor; // color = black
            t = objects[j]->intersect(incidentRay, tempColor, 0);

            if (t > 0.0 && t < tMinimum)
            {
                tMinimum = t;
            }
        }

        Point shadowIntersectionPoint = incidentRay.getRO().addVectorCopy(incidentRay.getRD().scalarMultCopy(tMinimum));
        double epsilon = 0.0000001; // for tuning light effect

        // distance between incident ray origin and intersection point
        double d_IR_IP = distanceBetweenTwoPoints(intersectionPoint, incidentRay.getRO());

        // distance between incident ray origin and shadow intersection point
        double d_IR_SIP = distanceBetweenTwoPoints(shadowIntersectionPoint, incidentRay.getRO());

        if (d_IR_IP - epsilon <= d_IR_SIP)
        {
            calculateReflectionComponents(
                ray,
                color,
                surfaceNormal,
                pointLights[i],
                incidentRay,
                intersectionPoint,
                iPointColor, // intersection point color
                this->rfco,
                this->shine);
        }
    }

    for (int i = 0; i < spotLights.size(); i++)
    {
        Point spotLightPos = spotLights[i].getLightPosition();
        Point lightDir = intersectionPoint.addVectorCopy(spotLightPos, -1);
        
        Point spotLightDir = spotLights[i].getLightDir();

        double angle = acos(spotLightDir.dotProduct(spotLightDir));

        if (angle>spotLights[i].getCutOffAngle()) continue;
        
        Point light_position = spotLights[i].getLightPosition();
        // Point light_dir = intersectionPoint.returnCopy();
        // light_dir.addVector(light_position, -1);
        Point light_dir = spotLightDir;

        Ray incidentRay(light_position, light_dir);

        double t, tMinimum = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            Color dummyColor; // color = black
            t = objects[j]->intersect(incidentRay, dummyColor, 0);

            if (t > 0.0 && t < tMinimum)
            {
                tMinimum = t;
            }
        }


        // shadowIntersectionPoint = incidentRay[Ro]+incidentRay[RD]*tMin_;
        Point shadowIntersectionPoint = incidentRay.getRO().addVectorCopy(incidentRay.getRD().scalarMultCopy(tMinimum));
        double epsilon = 0.0000001; // for tuning light effect

        // distance between incident ray origin and intersection point
        double d_IR_IP = distanceBetweenTwoPoints(intersectionPoint, incidentRay.getRO());

        // distance between incident ray origin and shadow intersection point
        double d_IR_SIP = distanceBetweenTwoPoints(shadowIntersectionPoint, incidentRay.getRO());

        if (d_IR_IP - epsilon <= d_IR_SIP)
        {
            calculateReflectionComponentsSpot(
                ray,
                color,
                surfaceNormal,
                spotLights[i],
                incidentRay,
                intersectionPoint,
                spotLights[i].getPointColor(), // intersection point color
                this->rfco,
                this->shine);
        }

    }

    /* handling recursive reflection */
    if (level >= recursionLevel)
    {
        return tMin;
    }

    /* incorporating concept of evil epsilon to recursive reflection computation */
    Point reflectedDir = ray.getRD();
    reflectedDir.addVector(surfaceNormal.scalarMultCopy(ray.getRD().dotProduct(surfaceNormal) * 2.0), -1);

    reflectedDir.normalize();
    Point reflectedOrigin = intersectionPoint.addVectorCopy(reflectedDir);
    Ray reflectedRay(reflectedOrigin, reflectedDir);

    /* finding nearest intersecting object (if available) */
    int nearest = INT32_MAX;
    double t, tMinimum = INF;

    for (int i = 0; i < objects.size(); i++)
    {
        Color tempColor; // color = black
        t = objects[i]->intersect(reflectedRay, tempColor, 0);

        if (t > 0.0 && t < tMinimum)
        {
            tMinimum = t;
            nearest = i;
        }
    }

    /* finding color component for reflected ray */
    Color reflectedColor; // color = black

    if (nearest != INT32_MAX)
    {
        tMinimum = objects[nearest]->intersect(reflectedRay, reflectedColor, level + 1);
    }

    /* computing recursive reflection component of reflected ray */
    calculateRecursiveReflectionComponents(color, reflectedColor, this->rfco);

    /* clipping the color values (if necessary) */
    color.red = (color.red > 1.0) ? 1.0 : ((color.red < 0.0) ? 0.0 : color.red);
    color.green = (color.green > 1.0) ? 1.0 : ((color.green < 0.0) ? 0.0 : color.green);
    color.blue = (color.blue > 1.0) ? 1.0 : ((color.blue < 0.0) ? 0.0 : color.blue);

    // color.red = 1.0;
    // color.green = 1.0;
    // color.blue = 1.0;

    return tMin;
}

Floor::~Floor()
{
}

#endif