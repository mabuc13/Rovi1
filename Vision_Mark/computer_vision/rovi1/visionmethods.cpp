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
    cv::Mat input =imageVersions[whichImage].clone();

    cv::calcHist(&input, 1, 0, cv::noArray(), histogram, 1, &histSize, &histRange, uniform, accumulate );
    //cout << "Histogram values: " << cvRound(histogram.at<float>(5)) << endl;
    int hist_w = 512; int hist_h=400;
    int bin_w = cvRound((double) hist_w/histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(0));
    normalize(histogram, histogram, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    //Lets determine the bin with the highest value!!
    float maxBinValue = 0;
    float tmp;
    for (int i = 0; i <histSize; i++)
    {
        tmp =histogram.at<float>(i);
        if ( tmp > maxBinValue)
            maxBinValue = tmp;
    }
    for (int i = 0; i < histSize; i++)
    {
        cv::line( histImage, cv::Point(bin_w*(i), 0), //Image we're drawing in, point(x, y)
                cv::Point( bin_w*(i), hist_h -cvRound(histogram.at<float>(i))),
                cv::Scalar(255), 2, 8, 0);
    }




    imageVersions.push_back(histImage);
    imgCounter++;
    ostringstream oss;
    oss << "Image " << imgCounter << " . Histogram of image: " << whichImage;
    string description = oss.str();
    imageDescription.push_back(description);
    cout << description << endl;


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

//void VisionMethods::addBorder(int bWidth, int whichImage)
//{
    //cv::Mat* source = &imageVersions[1];
    //cv::Mat destination;
    //if( !(!(i <100 || i>220) && !(j<350 || j>440)) )
    
//}

void VisionMethods::medianFilter(int whichImage, int maskSize) //Mask size is the length of the mask
//This method is an order statistical filter, the method is to sort all the values in the mask, and choose the median as the value for the pixel in the middle
{
    cv::Mat img =imageVersions[whichImage].clone(); //Image we're filtering
    int xMax = img.rows-(maskSize-1)/2; //These integers asures that we dont acces the image outside of it's borders
    int yMax = img.cols-(maskSize-1)/2;  //(masksize-1)-2 is due to the 1 pixel in the center we are looking at, and divide by two as only the mask part facing the border is the problem
    vector<int> medianVec;
    int median;
    int medianIndex = (maskSize*maskSize-1)/2; //The minus one is due to the index at vectors starting at 0
    for (int x = (maskSize-1)/2; x <= xMax; x++) //These 2 for loops go through all of the picture
        for (int y = (maskSize-1)/2; y<= yMax; y++)
        {
            for(int maskX = -(maskSize-1)/2; maskX <= (maskSize-1)/2; maskX++) //Looping through the mask
                for(int maskY = -(maskSize-1)/2; maskY <= (maskSize-1)/2; maskY++)
                {
                    medianVec.push_back(img.at<uchar>(x+maskX, y+maskY));
                }
            sort(medianVec.begin(), medianVec.begin()+maskSize*maskSize);
            median = medianVec[medianIndex];
            medianVec.clear();
            img.at<uchar>(x, y) = median;
        }


    imageVersions.push_back(img);
    imgCounter++;
    ostringstream oss;
    oss << "Image " << imgCounter << " . Median filtered image: " << whichImage;
    string description = oss.str();
    imageDescription.push_back(description);
    cout << description << endl;
}
