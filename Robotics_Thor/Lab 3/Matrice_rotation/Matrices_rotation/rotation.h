#ifndef ROTATION_H
#define ROTATION_H

#include <iostream>
#include <vector>


using namespace std;

class Rotation
{
public:
    Rotation();
    void myRot(vector<vector<float>>A,vector<vector<float>>B);

    ~Rotation();

private:

    vector<vector<float> >myVec;

};

#endif // ROTATION_H
