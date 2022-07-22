#include <bits/stdc++.h>
#include <ctime>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include "Vector.h"
#include "Matrix.h"
#include "FileWrite.h"
#include "Triangle.h"
#include "Containers.h"
#include "ClipUtil.h"

using namespace std;

#define println(s) std::cout << s << std::endl;

gluLookParams glP;
perspectiveParams prsP;
configParams cfP;

void printMatrix(TransformMatrix mat)
{
    int n = mat.getSize();
    int i, j;
    forn(i, n)
    {
        forn(j, n)
        {
            // printf("%.3lf ", mat.getVal(i,j));
            cout << std::fixed << setprecision(7) << mat.getVal(i, j) << " ";
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    ifstream in;
    ofstream out;

    srand(time(0));

    std::string dirname = "1";
    std::string path = "./output/" + dirname;
    mkdir(path.c_str(), 0777);

    in.open("./test_cases/" + dirname + "/scene.txt");

    FileWriter stage1("stage1.txt", "./output/" + dirname + "/");
    FileWriter stage2("stage2.txt", "./output/" + dirname + "/");
    FileWriter stage3("stage3.txt", "./output/" + dirname + "/");

    stage1.openFile();
    stage2.openFile();
    stage3.openFile();

    if (!in.is_open())
    {
        cout << "Input File Reading Failed" << endl;
        exit(1);
    }

    in >> glP.eyeX >> glP.eyeY >> glP.eyeZ;
    in >> glP.lookX >> glP.lookY >> glP.lookZ;
    in >> glP.upX >> glP.upY >> glP.upZ;

    in >> prsP.fovY >> prsP.aspectRatio >> prsP.near >> prsP.far;

    // glP.print();
    // prsP.print();

    // generate view matrix
    TransformMatrix viewMatrix;
    createIdentityMtx(viewMatrix);
    createViewMatrix(viewMatrix, glP);

    // generate projection matrix
    TransformMatrix projectionMatrix;
    createIdentityMtx(projectionMatrix);
    createProjectionMatrix(projectionMatrix, prsP);

    stack<TransformMatrix> matrixStack;
    TransformMatrix opMatrix, topMatrix;

    createIdentityMtx(topMatrix);
    matrixStack.push(topMatrix);

    vector<Triangle> triangle_vect;

    string command;

    while (true)
    {
        in >> command;
        // cout<<command<<endl;

        if (command == "triangle")
        {
            Point P1, P2, P3;
            in >> P1.x >> P1.y >> P1.z;
            in >> P2.x >> P2.y >> P2.z;
            in >> P3.x >> P3.y >> P3.z;

            topMatrix = matrixStack.top();
            P1 = topMatrix.multiplyPoint(P1);
            P2 = topMatrix.multiplyPoint(P2);
            P3 = topMatrix.multiplyPoint(P3);

            stage1.writePointToFile(P1);
            stage1.writePointToFile(P2);
            stage1.writePointToFile(P3);
            stage1.writeTextToFile("");

            TransformMatrix V = viewMatrix.getCopy();
            P1 = V.multiplyPoint(P1);
            P2 = V.multiplyPoint(P2);
            P3 = V.multiplyPoint(P3);

            stage2.writePointToFile(P1);
            stage2.writePointToFile(P2);
            stage2.writePointToFile(P3);
            stage2.writeTextToFile("");

            TransformMatrix P = projectionMatrix.getCopy();
            P1 = P.multiplyPoint(P1);
            P2 = P.multiplyPoint(P2);
            P3 = P.multiplyPoint(P3);

            stage3.writePointToFile(P1);
            stage3.writePointToFile(P2);
            stage3.writePointToFile(P3);
            stage3.writeTextToFile("");

            Triangle t(P1, P2, P3);

            triangle_vect.push_back(t);
        }
        else if (command == "translate")
        {
            Triplet T;
            in >> T.x >> T.y >> T.z;

            createIdentityMtx(opMatrix);
            createTranslationMtx(opMatrix, T);

            topMatrix = matrixStack.top();
            topMatrix.multiplyMatrix(opMatrix);
            matrixStack.pop();
            matrixStack.push(topMatrix);
        }
        else if (command == "scale")
        {
            Triplet S;
            in >> S.x >> S.y >> S.z;

            createIdentityMtx(opMatrix);
            createScalingMtx(opMatrix, S);

            topMatrix = matrixStack.top();
            topMatrix.multiplyMatrix(opMatrix);
            matrixStack.pop();
            matrixStack.push(topMatrix);
        }
        else if (command == "rotate")
        {
            double ax, ay, az, angle;
            in >> angle >> ax >> ay >> az;

            Point A(ax, ay, az);

            createIdentityMtx(opMatrix);
            createRotationMtx(opMatrix, A, angle);

            // printMatrix(opMatrix);

            topMatrix = matrixStack.top();
            topMatrix.multiplyMatrix(opMatrix);
            matrixStack.pop();
            matrixStack.push(topMatrix);
        }
        else if (command == "push")
        {
            topMatrix = matrixStack.top();
            matrixStack.push(topMatrix);
        }
        else if (command == "pop")
        {
            if (!matrixStack.empty())
                matrixStack.pop();
        }
        else if (command == "end")
        {
            break;
        }
        else
        {
            // invalid
        }
    }

    in.close();
    stage1.closeFile();
    stage2.closeFile();
    stage3.closeFile();

    // Read data

    in.open("./test_cases/" + dirname + "/config.txt");

    in >> cfP.Screen_Width >> cfP.Screen_Height;
    in >> cfP.X_leftLimit;

    cfP.X_rightLimit = -cfP.X_leftLimit;

    in >> cfP.Y_bottomLimit;

    cfP.Y_topLimit = -cfP.Y_bottomLimit;

    in >> cfP.Z_frontLimit >> cfP.Z_rearLimit;

    // cout<<cfP.Z_frontLimit<<" "<<cfP.Z_rearLimit<<endl;

    in.close();

    generateZBufferVal(triangle_vect, cfP, dirname);
}
