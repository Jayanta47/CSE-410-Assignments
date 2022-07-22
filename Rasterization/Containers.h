#ifndef CONTAINERS_H
#define CONTAINERS_H

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

struct configParams
{
    int Screen_Width, Screen_Height;
    double X_leftLimit, X_rightLimit, Y_bottomLimit, Y_topLimit;
    double Z_frontLimit, Z_rearLimit;
};

#endif 