#include "rotation.h"

Rotation::Rotation()
{

}
vector<vector<float>> Rotation::initialize(){
    vector<vector<float>>matrix;
    for(int i = 0; i <=3 ; i++){
        for(int j = 0 ; j<=3; j++){
            matrix[i][j]=0;
        }

    }
    return matrix;
}

vector<vector<float>> Rotation::myRot(vector<float> rot_vec){

    float x = rot_vec[0];
    float y = rot_vec[1];
    float z = rot_vec[2];
    tmp = A;
    /* First coloum */

    A[0][0] = cos(y)*cos(z);
    A[1][0] = sin(x)*sin(y)*cos(z) - cos(x)*sin(z) ;
    A[2][0] = cos(x)*sin(y)*cos(z) + sin(x)*sin(z);
    /* Second coloum */
    A[0][1] = cos(y)*sin(z);
    A[1][1] = sin(x)*sin(y)*sin(z) + cos(x)*cos(z) ;
    A[2][1] = cos(x)*sin(y)*sin(z) - sin(x)*cos(z);
    /* Third coloum */
    A[0][2] = -sin(y);
    A[1][2] = sin(x)*cos(y);
    A[2][2] = cos(x)*cos(y);



    return A;
}
void Rotation::printRotation(){
    cout<<"Matrix contains"<<endl;

    for(int i = 0; i <=2 ; i++){
        for(int j = 0 ; j<=2; j++){
            cout<<A[i][j]<<" ";
        }
        cout<<endl;
    }
}
void Rotation::eularAngles(vector<float>rot){
    //float x=rot[0];
    float y=rot[1];
    //float z=rot[2];
    float thethaX;
    float thethaY;
    float thethaZ;

    thethaY =asin(-A[0][2]);
    thethaX=atan2(cos(y)*A[1][2],cos(y)*A[2][2]);
    thethaZ=atan2(cos(y)*A[0][1],cos(y)*A[0][0]);
    cout<<"The eular angles are(x,y,z) : "<<thethaX<<","<<thethaY<<","<<thethaZ<<endl;
}


Rotation::~Rotation(){

}

