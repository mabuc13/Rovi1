#include "rotation.h"

rotation::rotation()
{

}

rotation::~rotation()
{

}

void rotation::createRotationMatrix(double x, double y, double z)
{
    yy = y; //We need this for the euler angles
    //Column 1!! //ROW MAJOR
    rotationMatrix[0][0] = cos(y)*cos(z);
    rotationMatrix[1][0] = cos(y)*sin(z);
    rotationMatrix[2][0] = - sin(y);
    //Column 2!
    rotationMatrix[0][1] = sin(x)*sin(y)*cos(z)
-cos(x)*sin(z);
    rotationMatrix[1][1] = sin(x)*sin(y)*sin(z)+cos(x)*cos(z);
    rotationMatrix[2][1] = sin(x)*cos(y);
    //Column 3!
    rotationMatrix[0][2] = cos(x)*sin(y)*cos(z)+sin(x)*sin(z);
    rotationMatrix[1][2] = cos(x)*sin(y)*sin(z)-sin(x)*cos(z);
    rotationMatrix[2][2] = cos(x)*cos(y);
    /*
     IF IT WAS COLUMN MAJOR:
    //Column 1!!
    rotationMatrix[0][0] = sin(y)*cos(z);
    rotationMatrix[0][1] = cos(y)*sin(z);
    rotationMatrix[0][2] = - sin(y);
    //Column 2!
    rotationMatrix[1][0] = sin(x)*sin(y)*cos(z)
-cos(x)*sin(z);
    rotationMatrix[1][1] = sin(x)*sin(y)*sin(z)+cos(x)*cos(z);
    rotationMatrix[1][2] = sin(x)*cos(y);
    //Column 3!
    rotationMatrix[2][0] = cos(x)*sin(y)*cos(z)+sin(x)*sin(z);
    rotationMatrix[2][1] = cos(x)*sin(y)*sin(z)-sin(x)*cos(z);
    rotationMatrix[2][2] = cos(x)*cos(y);
    */


}

void rotation::printMatrix()
{
    cout << "Printing current rotation matrix:" << endl;
    for (int i = 0; i<3; i++)
        cout << rotationMatrix[i][0] << ", " << rotationMatrix[i][1]  << ", " << rotationMatrix[i][2] << endl;
}

void rotation::printEulerAngles()
{
    double r31 = rotationMatrix[2][0];
    if (abs(r31) > 0.95 && abs(r31) < 1.05)
        cout << "Near singularity encountered, beware!!!" << endl;
    cout << "Roll(z): " << atan2(cos(yy)*rotationMatrix[1][0], cos(yy)*rotationMatrix[0][0]) << endl;
    cout << "Pitch(y): " << asin(-rotationMatrix[2][0]) << endl;
    cout << "Yaw(x): " << atan2(cos(yy)*rotationMatrix[2][1], cos(yy)*rotationMatrix[2][2]) << endl;
}
