#include <bits/stdc++.h>

#include "Matrix.h"


using namespace std;

void printMatrix(TransformMatrix mat) 
{
    int n = mat.getSize();
    int i, j;
    forn(i, n)
    {
        forn(j,n)
        {
            printf("%.3lf ", mat.getVal(i,j));
        }
        printf("\n");
    }
}

int main() {
    int i, j, n;
    bool passed = true;
    cout<<"Identity Matrix Test"<<endl;
    TransformMatrix mat;
    createIdentityMtx(mat);
    n = mat.getSize();
    forn(i,n) {
        forn(j, n) {
          if (i==j)
          {
            if (mat.getVal(i, j)!=1)passed= false;
          }
          else if (mat.getVal(i, j)!= 0.0) passed = false;  
        }
    }

    passed ? cout<<"Passed"<<endl:cout<<"Failed"<<endl;

    stack<TransformMatrix> stk;

    cout<<"Translation Matrix"<<endl;
    Triplet T(2,4,6);
    createTranslationMtx(mat, T);
    printMatrix(mat);

    stk.push(mat);

    cout<<"Scaling Matrix"<<endl;
    createIdentityMtx(mat);
    Triplet S(4,6, 2);
    createScalingMtx(mat, S);
    printMatrix(mat);

    TransformMatrix mat2 = mat;

    mat = stk.top();
    cout<<endl;
    printMatrix(mat);

    mat.multiplyMatrix(mat2);

    cout<<"Multiply matrix test"<<endl;
    printMatrix(mat);
    cout<<endl;
    mat = stk.top();
    printMatrix(mat);

    cout<<"Multiply point Test"<<endl;
    Point A(1,2,3);
    Point B = mat.multiplyPoint(A);
    cout<<B.x<<" "<<B.y<<" "<<B.z<<" "<<B.w<<endl;

}