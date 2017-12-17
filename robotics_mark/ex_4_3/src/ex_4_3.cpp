#include <rw/rw.hpp>
#include <rw/math/EAA.hpp>
#include <rw/math/Rotation3D.hpp>
#include <rw/math.hpp>
#include <cmath>

using namespace rw::common;
using namespace rw::math;
using namespace rw::kinematics;
//using rw::math::Transform3D;
//using rw::math::rotation3D;

//double calcTheta(rw::math::Rotation3D<> R1)
//{
//    double theta = acos((R1[1, 2], R1[1, 2], R1[1, 2] - 1)/2);
//    return theta;
//}

//Vector3D<> calcV(Rotation3D<> R1)
//{
//     vector3D<> v1(R1[7]-R1[5], R1[2]-R1[6], R1[4]-R1[1]);
//     double scalar = norm(v1);
//     return v1*scalar;
//}

int main()
{

   Rotation3D<double> R1(1/4, sqrt(3)/4, sqrt(3)/2, sqrt(3)/4, 3/4,-1/2, -sqrt(3)/2, 1/2, 0); //Next compute (v, theta) given a rotation R

    double theta = acos((R1(1, 1), R1(2, 2), R1(3, 3) - 1)/2);                                         //also compute R_EAA given (v, theta)


}
