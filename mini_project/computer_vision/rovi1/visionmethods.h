#ifndef VISIONMETHODS_H
#define VISIONMETHODS_H
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <sstream>
using namespace::std;
using namespace::cv;
class VisionMethods
{
public: //General idea is to create an instance of this class for each image
    VisionMethods(std::string pathToImage);
    ~VisionMethods();
    void greyIntensityTransform(int intensityC, int whichImage);
    void calcHistogram(int whichImage);
    void medianFilter(int whichImage, int maskSize);
    void medianFilterV2(int whichImage, int maskSize);
    void adaptiveMedianFilter(int whichImage, int maxMaskSize);
    void dftFunc(int index);
    void butterWorth(int whichImage, int distance, bool high, int order);
    void histogramOfRegion(int whichImage, int topLeftX, int topLeftY, int width, int heigth); //Saving region image first, then histogram
    void maxFilter(int whichImage, int maskSize);
    void dftInverse(int whichImage);
    void shiftDFT(Mat& fImage );
    Mat create_spectrum_magnitude_display(Mat& complexImg, bool rearrange);
    void create_butterworth_lowpass_filter(Mat &dft_Filter, int D, int n);
    void butterUp(int whichImage, int radiu, int orde);
    //General purpose methods
    void showImg(int index);
    void showAllImages();
    void showSpecificImages(int img1, int img2, int img3, int img4);
    void writeImages();
    //void addBorder(int bWidth, int whichImage);
private:
    std::vector<cv::Mat> imageVersions; //Using index to this vector to acces images stored in the class
    int imgCounter = 0;
    vector<string> imageDescription;
    cv::Mat lastComplexI;
};


#endif // VISIONMETHODS_H
