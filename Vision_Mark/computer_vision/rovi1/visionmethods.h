#ifndef VISIONMETHODS_H
#define VISIONMETHODS_H
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
using namespace::std;
class VisionMethods
{
public: //General idea is to create an instance of this class for each image
    VisionMethods(std::string pathToImage);
    ~VisionMethods();
    void greyIntensityTransform(int intensityC, int whichImage);
    void calcHistogram(int whichImage);
    //General purpose methods
    void showImg(int index);
    void showAllImages();
private:
    std::vector<cv::Mat> imageVersions; //Using index to this vector to acces images stored in the class
    int imgCounter = 0;
    vector<string> imageDescription;
};

#endif // VISIONMETHODS_H
