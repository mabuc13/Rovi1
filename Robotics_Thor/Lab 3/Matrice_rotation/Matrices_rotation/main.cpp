#include <iostream>

#include <math.h>
#include <vector>

#include "rotation.h"

using namespace std;

int main()
{
    cout<<"enter main"<<endl;
    Rotation mytest;
    vector<vector<float>>vec1;
    vector<vector<float>>vec2;
    cout<<"after vectors"<<endl;
    vec1[0][0]=1;
    cout<<"insert number"<<endl;
    mytest.myRot(vec1,vec2);
    cout<<"exit main"<<endl;
    return 0;
}


