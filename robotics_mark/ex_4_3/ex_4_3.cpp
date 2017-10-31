#include <rw/rw.hpp>
#include <rw/EAA.hpp>
#include <rw/Rotation3D.hpp>

using namespace rw::rotation3d;
using namespace rw::common;
using namespace rw::math;
using namespace rw::kinematics;
using rw::math::Transform3D;
int main()
{

    R1 = rotation3D(1/4, sqrt(3)/4, sqrt(3)/2, sqrt(3)/4, 3/4,-1/2, -sqrt(3)/2, 1/2, 0);
    V = vector3D();
    theta = arctan((R1[1], R1[5], R1[9] - 1)/2);

}
