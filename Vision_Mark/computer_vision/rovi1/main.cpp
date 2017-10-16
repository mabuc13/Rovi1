#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <visionmethods.h>
using namespace std;

int main()
{

   // VisionMethods ex2("pictures/lena.bmp");
    //ex2.showImg(1);
    VisionMethods miniProject("pictures/Images/ImagesForStudents/Image1.png");
    //ex2.greyIntensityTransform(-100, 1);
    //ex2.showImg(2);
    //ex2.calcHistogram(1);
    //ex2.calcHistogram(2);
    //ex2.medianFilter(1, 3);
    //ex2.showAllImages();
    miniProject.calcHistogram(1);
    miniProject.medianFilter(1, 3);
    miniProject.showAllImages();
    return 0;
}


