#ifndef ROTATION_H
#define ROTATION_H

#include <iostream>
#include <math.h>
#include <vector>


using namespace std;

class Rotation
{
public:
    Rotation();
    vector<vector<float>> initialize();
    vector<vector<float>> myRot(vector<float>rotation);
    void eularAngles(vector<float>rot);


    void printRotation();
    ~Rotation();

private:
    vector<vector<float>> tmp = {{0,0,0},{0,0,0},{0,0,0}};
    vector<vector<float>> A = {{0,0,0},{0,0,0},{0,0,0}};



};

#endif // ROTATION_H
