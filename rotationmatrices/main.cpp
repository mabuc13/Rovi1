#include <iostream>
#include <vector>
#include <cmath>
#include "rotation.h"
using namespace std;
#define M_PI 3.14159265358979323846
int main()
{
    rotation object;
    object.createRotationMatrix(M_PI/2, M_PI/3, M_PI/2);
    /*
     * object.createRotationMatrix(M_PI/2, 15*(M_PI/180), M_PI);
     * 5.91459e-17, 1, 1.06617e-16,
     * 0.965926, -2.95361e-17, -0.258819,
     * -0.258819, 1.18292e-16, -0.965926
     */
    //RPY<> R1 = RPY<>(Pi/2, 15*Deg2Rad, Pi);
    object.printMatrix();
    object.printEulerAngles();
    return 0;
}

