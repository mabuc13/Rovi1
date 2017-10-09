#ifndef VISIONMETHODS_H
#define VISIONMETHODS_H
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
using namespace::std;
class VisionMethods
{
public: //General idea is to create an instance of this class for each image
    VisionMethods(std::string pathToImage);
    ~VisionMethods();
    void greyIntensityTransform(int intensityC, int whichImage);
    void calcHistogram(int whichImage);
    void medianFilter(int whichImage, int maskSize);
    void adaptiveMedianFilter(int whichImage, int maxMaskSize);
    void dftFunc(int index);
    void butterWorth(int whichImage, int distance, bool high, int order);
    void histogramOfRegion(int whichImage, int topLeftX, int topLeftY, int width, int heigth);
    void maxFilter(int whichImage, int maskSize);
    //General purpose methods
    void showImg(int index);
    void showAllImages();
    //void addBorder(int bWidth, int whichImage);
private:
    std::vector<cv::Mat> imageVersions; //Using index to this vector to acces images stored in the class
    int imgCounter = 0;
    vector<string> imageDescription;
};

#endif // VISIONMETHODS_H
