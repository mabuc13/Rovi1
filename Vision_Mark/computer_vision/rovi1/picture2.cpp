#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <visionmethods.h>
using namespace std;

int main()
{
      /*//PICTURE 1
    VisionMethods picture1("pictures/Images/ImagesForStudents/Image1.png");
    picture1.histogramOfRegion(1, 1035, 1437, 280, 267);
    picture1.maxFilter(1, 5);
    picture1.adaptiveMedianFilter(4, 9);



    picture1.showAllImages();
  */



        // *  ---- PICTURE 2 ------
      VisionMethods picture2("pictures/Images/ImagesForStudents/Image2.png");
      picture2.histogramOfRegion(1, 1176, 1350, 280, 267);
      picture2.adaptiveMedianFilter(1, 9);
      picture2.medianFilter(1, 3);
      picture2.showAllImages();
          /**/
      //miniProject.calcHistogram(1);
     //miniProject.medianFilter(1, 3);
     //miniProject.showAllImages();
    /*//Picture 3
    VisionMethods picture3("pictures/Images/ImagesForStudents/Image3.png");
    picture3.histogramOfRegion(1, 1035, 1437, 280, 267);
    picture3.showAllImages();
    */
}


