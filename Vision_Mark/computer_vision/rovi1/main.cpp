#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <visionmethods.h>
using namespace std;

int main()
{

    VisionMethods ex2("pictures/lena.bmp");
    //ex2.showImg(1);
    ex2.greyIntensityTransform(-100, 1);
    //ex2.showImg(2);
    ex2.showAllImages();
    return 0;
}


