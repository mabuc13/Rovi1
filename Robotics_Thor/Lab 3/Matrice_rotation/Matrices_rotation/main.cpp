#include <iostream>

#include <math.h>
#include <vector>

#include "rotation.h"

#define MY_PI 3.14159

using namespace std;

int main()
{

    Rotation myRotation;

    vector<float>rotation;

    // z,y,x = 1.5708, 0.261799, 3.14159
    rotation = {MY_PI,15*(MY_PI/180),MY_PI/2};
    //rotation = {MY_PI/2, 15*(MY_PI/180), MY_PI};
    //rotation = {MY_PI/2,MY_PI/3,MY_PI/2};
    myRotation.myRot(rotation);

    myRotation.printRotation();

    myRotation.eularAngles(rotation);

    return 0;
}


/*
 *
 * Rotation3D(5.91459e-17, 1, 1.06617e-16,
 *              0.965926, -2.95361e-17, -0.258819,
 *              -0.258819, 1.18292e-16, -0.965926)
 * */
