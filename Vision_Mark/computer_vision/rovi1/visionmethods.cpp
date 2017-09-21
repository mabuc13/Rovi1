#include "visionmethods.h"

VisionMethods::VisionMethods(std::string pathToImage)
{
    cv::Mat originalImage = cv::imread(pathToImage);
    if (originalImage.empty())
        {
           std::cout << "Input image not found at '" << pathToImage << "'\n";
        }
    imageVersions.push_back(originalImage);
    imageDescription.push_back("Image 0. Original image");
    cv::Mat greyImg;
    cvtColor(originalImage, greyImg, CV_BGR2GRAY);
    cout << "Stored original image at 0" << endl;
    imageVersions.push_back(greyImg);
    imageDescription.push_back("Image 1. Original image in greyscale");
    cout << "Stored greyscale version of image at 1" << endl;
    imgCounter = 1;

}

VisionMethods::~VisionMethods()
{
}

void VisionMethods::greyIntensityTransform(int intensityC, int whichImage)
{
    uchar tmp;
    int tmpInt;
    cv::Mat img = imageVersions[1].clone();
    int width = img.cols;
    int height = img.rows;

    for (int x = 0 ; x<width; x++)
        for (int y = 0; y<height; y++)
        {
            tmpInt = img.at<uchar>(x, y);
            if (-1<tmpInt+intensityC<256)
                img.at<uchar>(x, y) = tmpInt+intensityC;
            if (tmpInt + intensityC > 255)
                img.at<uchar>(x, y) = 255;
            if (tmpInt+intensityC < 0)
                img.at<uchar>(x, y) = 0;
        }
    imageVersions.push_back(img);
    imgCounter++;
    ostringstream oss;
    oss << "Image " << imgCounter <<" . Intensity transformed image: " << whichImage << " by a constant of: " << intensityC << endl;
    string description = oss.str();
    cout << description << endl;
    imageDescription.push_back(description);

}

void VisionMethods::calcHistogram(int whichImage)
{
    int histSize = 256; //Number of bins in the histogram
    bool uniform = true; bool accumulate = false; //Arguments needed for the histogram calculation
    cv::Mat histogram; //I think this is where the calculated histogram is stored!
    float range[] = { 0, 256}; //What does [] means? The upper bound is exclusive
    const float* histRange = { range };
    //Meaning of arguments in order {Source arrays, number of source arrays, a mask not defined means it's ignored, where we store the histogram, histogram dimensionality, number of bins, range of values to be measured, uniform histogram, histogram is cleared at the beginning}
    cv::calcHist(&imageVersions[whichImage], 1, 0, cv::Mat(), histogram, 1, &histSize, uniform, accumulate );
    imageVersions.push_back(histogram);
    imgCounter++;
    //IMPLEMENT STRINGSTREAM
    cout << "Histogram for image located at: " << whichImage << " calculated and stored at: " << imgCounter << endl;


}

//General purpose methods
void VisionMethods::showImg(int index)
{
    cout << "Showing image at index: " << index << endl;
    cv::imshow(imageDescription[index], imageVersions[index]);
    cv::waitKey();
}

void VisionMethods::showAllImages()
{
    for (int i = 0; i < imageVersions.size(); i++)
    {
        cv::imshow(imageDescription[i], imageVersions[i]);
    }
    cv::waitKey();
}
