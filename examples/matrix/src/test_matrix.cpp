#include <cstdlib>
#include <iostream>
#include "Matrices.h"
#include "Timer.h"

using namespace std;

int main(int argc, char *argv[])
{
    Matrix4 m1, m2;
    m1.set(2,-1,0,0, 1,2,5,0, 1,0,4,0, 3,1,2,1);
    m2 = m1;

    cout << "ORIGINAL\n";
    cout << m1;
    cout << "\nDET: " << m1.getDeterminant() << endl;

    Timer t;
    t.start();
    for(int i = 0; i < 10000000; ++i)
    {
        m2 = m1;
        m2.invertGeneral();
    }
    t.stop();
    cout << "\nINVERSE (General)\n";
    cout << m2;
    cout << "ELAPSED TIME (General): " << t.getElapsedTime() << endl;

    t.start();
    for(int i = 0; i < 10000000; ++i)
    {
        m2 = m1;
        m2.invertAffine();
    }
    t.stop();
    cout << "ELAPSED TIME (Affine) : " << t.getElapsedTime() << "\n\n";
    cout << m2;

    system("PAUSE");
    return EXIT_SUCCESS;
}
