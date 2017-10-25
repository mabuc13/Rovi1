#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <visionmethods.h>
using namespace std;

int main()
{


      VisionMethods picture2("pictures/Images/ImagesForStudents/Image2.png");
      picture2.histogramOfRegion(1, 1035, 1437, 280, 267);
      picture2.adaptiveMedianFilter(1, 11);
      picture2.dftFunc(1);
      picture2.writeImages();
      picture2.showAllImages();
}


