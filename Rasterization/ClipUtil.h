#ifndef CLIPUTIL_H
#define CLIPUTIL_H

#include <vector>
#include <assert.h>

#include "Vector.h"
#include "Matrix.h"
#include "Containers.h"
#include "Triangle.h"
#include "bitmap_image.hpp"

using namespace std;

double extrapolateXfromY(double X1, double X2, double Y1, double Y2, double Ys)
{
    // assert((Y1-Y2) != 0.0);

    double X = X1 + ((Ys - Y1) * (X1 - X2)) / (Y1 - Y2);

    return X;
}

void generateZBufferVal(vector<Triangle> &tv, configParams &cfP, std::string dirname)
{
    // Initialize Z-buffer and Frame buffer.

    double dx, dy, Top_Y, Bottom_Y, Left_X, Right_X;

    dx = (cfP.X_rightLimit - cfP.X_leftLimit) / cfP.Screen_Width;
    dy = (cfP.Y_topLimit - cfP.Y_bottomLimit) / cfP.Screen_Height;

    Top_Y = cfP.Y_topLimit - dy / 2.0;
    Bottom_Y = cfP.Y_bottomLimit + dy / 2.0;
    Left_X = cfP.X_leftLimit + dx / 2.0;
    Right_X = cfP.X_rightLimit - dx / 2.0;

    double **Z_buffer = new double *[cfP.Screen_Height];

    int i;
    forn(i, cfP.Screen_Height)
    {
        Z_buffer[i] = new double[cfP.Screen_Width];
        int j;
        forn(j, cfP.Screen_Width)
        {
            Z_buffer[i][j] = cfP.Z_rearLimit;
        }
    }

    Triplet **frame_buffer = new Triplet *[cfP.Screen_Height];

    forn(i, cfP.Screen_Height)
    {
        frame_buffer[i] = new Triplet[cfP.Screen_Width];
    }

    for (Triangle triangle : tv)
    {
        pair<double, double> p = triangle.MinMaxPoint('Y');
        double minY = p.first, maxY = p.second;

        int topScanline, bottomScanline;

        topScanline = (maxY >= Top_Y) ? 0 : (int)round((Top_Y - maxY) / dy);

        double Bottom_Y = cfP.Y_bottomLimit + dy / 2.0;

        bottomScanline = cfP.Screen_Height - 1;

        bottomScanline -= (minY <= Bottom_Y) ? 0 : (int)round((minY - Bottom_Y) / dy);

        for (int scanLine = topScanline; scanLine <= bottomScanline; scanLine++)
        {
            double ys = Top_Y - scanLine * dy;
            vector<pair<double, int>> endPoints;
            double X_Max = 0.0, X_Min = 0.0;
            int maxIndex = -1, minIndex = -1;

            for (int pointIdx = 0; pointIdx < 3; pointIdx++)
            {
                Point P1 = triangle.getVertices(pointIdx);
                Point P2 = triangle.getVertices((pointIdx + 1) % 3);

                if (P1.y != P2.y)
                {
                    double X = extrapolateXfromY(P1.x, P2.x, P1.y, P2.y, ys);
                    if (X <= max(P1.x, P2.x) && X >= min(P1.x, P2.x))
                    {
                        endPoints.push_back({X, pointIdx});
                    }
                }
            }

            for (auto points : endPoints)
            {
                if (maxIndex == -1 && minIndex == -1)
                {
                    X_Max = X_Min = points.first;
                    maxIndex = minIndex = points.second;
                }
                else
                {
                    if (points.first < X_Min)
                    {
                        X_Min = points.first;
                        minIndex = points.second;
                    }
                    if (points.first > X_Max)
                    {
                        X_Max = points.first;
                        maxIndex = points.second;
                    }
                }
            }

            int leftIntersectingCol = 0;
            int rightIntersectingCol = cfP.Screen_Width - 1;

            if (X_Min > Left_X)
            {
                leftIntersectingCol = (int)round((X_Min - Left_X) / dx);
            }

            if (X_Max < Right_X)
            {
                rightIntersectingCol = cfP.Screen_Width - 1 - ((int)round((Right_X - X_Max) / dx));
            }

            Point P1 = triangle.getVertices(minIndex);
            Point P2 = triangle.getVertices((minIndex + 1) % 3);

            double za = extrapolateXfromY(P1.z, P2.z, P1.y, P2.y, ys);

            P1 = triangle.getVertices(maxIndex);
            P2 = triangle.getVertices((maxIndex + 1) % 3);

            double zb = extrapolateXfromY(P1.z, P2.z, P1.y, P2.y, ys);
            double const_term = (dx * (zb - za)) / (X_Max - X_Min);

            double zp;

            for (int col = leftIntersectingCol; col <= rightIntersectingCol; col++)
            {

                if (col == leftIntersectingCol)
                {
                    zp = extrapolateXfromY(za, zb, X_Min, X_Max, (Left_X + col * dx));
                }
                else
                {
                    zp += const_term;
                }

                if (zp > cfP.Z_frontLimit && zp < Z_buffer[scanLine][col])
                {
                    Z_buffer[scanLine][col] = zp;

                    frame_buffer[scanLine][col].x = triangle.getValofColor("red");
                    frame_buffer[scanLine][col].y = triangle.getValofColor("green");
                    frame_buffer[scanLine][col].z = triangle.getValofColor("blue");
                }
            }
        }
    }

    bitmap_image bitmapImage(cfP.Screen_Width, cfP.Screen_Height);

    for (int row = 0; row < cfP.Screen_Height; row++)
    {
        for (int col = 0; col < cfP.Screen_Width; col++)
        {
            bitmapImage.set_pixel(col, row, frame_buffer[row][col].x, frame_buffer[row][col].y, frame_buffer[row][col].z);
        }
    }

    bitmapImage.save_image("./output/" + dirname + "/out.bmp");

    FileWriter ZBufferFile("z_buffer.txt", "./output/" + dirname + "/");

    if (!ZBufferFile.openFile())
    {
        // cout << "Could not open z buffer file" << endl;
        exit(EXIT_FAILURE);
    }

    for (int row = 0; row < cfP.Screen_Height; row++)
    {
        for (int col = 0; col < cfP.Screen_Width; col++)
        {
            if (Z_buffer[row][col] < cfP.Z_rearLimit)
            {
                ZBufferFile.writeDoubleToFile(Z_buffer[row][col]);
            }
        }
        ZBufferFile.writeTextToFile("");
    }

    ZBufferFile.closeFile();

    for (int i = 0; i < cfP.Screen_Height; i++)
    {
        delete[] Z_buffer[i];
    }
    delete[] Z_buffer;

    for (int i = 0; i < cfP.Screen_Height; i++)
    {
        delete[] frame_buffer[i];
    }
    delete[] frame_buffer;

    tv.clear();
}

#endif