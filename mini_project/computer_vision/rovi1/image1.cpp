#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <visionmethods.h>
using namespace std;

int main()
{


VisionMethods picture1("pictures/Images/ImagesForStudents/Image1.png");
    picture1.histogramOfRegion(1, 1035, 1437, 280, 267);
    picture1.dftFunc(1);
    picture1.maxFilter(1, 5);
    picture1.dftFunc(4);
    picture1.histogramOfRegion(4, 1035, 1437, 280, 267);
    picture1.dftFunc(5);
    picture1.writeImages();
    picture1.showAllImages();

}
