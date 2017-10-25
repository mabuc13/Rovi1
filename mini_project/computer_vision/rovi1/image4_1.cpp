#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <visionmethods.h>
using namespace std;

int main()
{

VisionMethods picture4A("pictures/Images/ImagesForStudents/Image4_1.png"); //A notch filter or butterworth would be able to filter out the frequency based noise.
    picture4A.dftFunc(1);
    picture4A.histogramOfRegion(1, 1035, 1437, 280, 267);
    picture4A.butterUp(1, 60, 2);
    picture4A.writeImages();
    picture4A.showAllImages();
}

