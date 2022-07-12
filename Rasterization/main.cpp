#include <bits/stdc++.h>
#include <cstdio>
#include "Vector.h"

using namespace std;

struct gluLookParams {
    double eyeX, eyeY, eyeZ;
    double lookX, lookY, lookZ;
    double upX, upY, upZ;

    void print() {
        printf("glulookAt Parameters:\n");
        printf("eyeX: %.4lf, eyeY: %.4lf, eyeZ: %.4lf\n", eyeX, eyeY, eyeZ);
        printf("lookX: %.4lf, lookY: %.4lf, lookZ: %.4lf\n", lookX, lookY, lookZ);
        printf("upX: %.4lf, upY: %.4lf, upZ: %.4lf\n", upX, upY, upZ);
    }
};

struct perspectiveParams {
    double fovY, aspectRatio, near, far;

    void print() {
        printf("gluPerspective Parameters:\n");
        printf("fovY: %.4lf, aspectRatio: %.4lf, near: %.4lf, far: %.4lf\n", 
            fovY, aspectRatio, near, far);
    }
};

gluLookParams glP;
perspectiveParams prsP;


int main(int argc, char **argv) 
{
    ifstream in;
    ofstream out;

    in.open("./test_cases/1/scene.txt");

    if (!in.is_open()) {
        cout<<"Input File Reading Failed"<<endl;
        exit(1);
    }

    in>>glP.eyeX>>glP.eyeY>>glP.eyeZ;
    in>>glP.lookX>>glP.lookY>>glP.lookZ;
    in>>glP.upX>>glP.upY>>glP.upZ;

    in>>prsP.fovY>>prsP.aspectRatio>>prsP.near>>prsP.far;

    glP.print();
    prsP.print();

    string command;

    while(true) {
        in>>command;

        if (command == "triangle") {

        }
        else if (command == "translate") {

        }
        else if (command == "scale") {

        }
        else if (command == "rotate") {

        }
        else if (command == "push") {

        }
        else if (command == "pop") {

        }
        else if (command == "end") {
            break;
        }
        else {
            // invalid
        }
    }

    in.close();
}

