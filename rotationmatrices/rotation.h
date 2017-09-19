#ifndef ROTATION_H
#define ROTATION_H
#include <vector>
#include <cmath>
#include <iostream>
using namespace::std;

class rotation
{
public:
    rotation();
    ~rotation();
    void createRotationMatrix(double x, double y, double z);
    void printMatrix();
    void printEulerAngles();
private:
    vector<vector<double>> rotationMatrix = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    double yy;
};


#endif // ROTATION_H
