#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <visionmethods.h>
using namespace std;

int main()
{

//Picture 3 From the histogram and fourier magnitude plot it seems that uniform noise is present. The "easiest" way would prolly be to make a butterworth filter

    
    VisionMethods picture3("pictures/Images/ImagesForStudents/Image3.png");
    picture3.histogramOfRegion(1, 1035, 1437, 280, 267);
    picture3.adaptiveMedianFilter(1, 9);
    picture3.dftFunc(1);
    picture3.showAllImages();
    

}
