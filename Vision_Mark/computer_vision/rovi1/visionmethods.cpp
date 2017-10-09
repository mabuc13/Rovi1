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

void VisionMethods::dftFunc(int whichImage){
    cv::Mat img = imageVersions[whichImage];     //Create a dummie image container

    cv::imshow("original",img);

    //Zero pad for easier DFT
    cv::Mat padded;                            //expand input image to optimal size
    const int m = cv::getOptimalDFTSize( img.rows );
    const int n = cv::getOptimalDFTSize( img.cols ); // on the border add zero pixels
    cv::copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    //cv::imshow("padded",padded);

    //Make place for both imagninary and real part
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    //Make DFT
    dft(complexI, complexI);                    //Making the DFT

    //make imaginary and real values
    cv::split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    cv::Mat magI = planes[0];

    //swith to logarithm scale
    magI += cv::Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    //Crop and rearrange
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
    const int cx = magI.cols/2;
    const int cy = magI.rows/2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);


    //Normalize
    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).

    //cv::imshow("plane[1]- maybe phase",planes[1]);

    imageVersions.push_back(magI);
    imgCounter++;
    ostringstream oss;
    oss << "Image " << imgCounter << " . Magnitude image of fourier transform of image:  " << whichImage;
    string description = oss.str();
    imageDescription.push_back(description);
    cout << description << endl;




}

void VisionMethods::butterWorth(int whichImage, int distance, bool high, int order)
{
    int filterRows = imageVersions[whichImage].rows; //Get the dimension of the filter
    int filterCols = imageVersions[whichImage].cols;
    //Filter skal nok også være komplekst, ar jeg hørt...
    cv::Mat filterImg = cv::Mat(filterRows, filterCols, CV_8UC1, cv::Scalar(0)); //Create the filter
    for (int x = 0; x<= filterRows; x++)
        for (int y = 0; y<= filterCols; y++)
        {
            if (!high)
            {
                //filterImg.at<uchar>(x, y) = 1/(1+pow((sqrt(x*x+y*y))/distance,2*order));
                cout << 1/(1+pow((sqrt(x*x+y*y))/distance,2*order)) << " " ;
            }
        }
    imageVersions.push_back(filterImg);
    imgCounter++;
    ostringstream oss;
    oss << "Image " << imgCounter << " . Butterworth filter for image:   " << whichImage << "Distance, order, highpass: " << distance << ", " << order << ", " << high << endl;
    string description = oss.str();
    imageDescription.push_back(description);
    cout << description << endl;
    //SEEE
    //http://breckon.eu/toby/teaching/dip/opencv/lecture_demos/c++/butterworth_lowpass.cpp
}

void VisionMethods::histogramOfRegion(int whichImage, int topLeftX, int topLeftY, int width, int height)
{
    int histSize = 256; //Number of bins in the histogram
    bool uniform = true; bool accumulate = false; //Arguments needed for the histogram calculation
    cv::Mat histogram; //I think this is where the calculated histogram is stored!
    float range[] = { 0, 256}; //What does [] means? The upper bound is exclusive
    const float* histRange = { range };
    //Meaning of arguments in order {Source arrays, number of source arrays, a mask not defined means it's ignored, where we store the histogram, histogram dimensionality, number of bins, range of values to be measured, uniform histogram, histogram is cleared at the beginning}

    //cv::Mat region; //= source(cv::Rect(topLeftX, topLeftY, width, height));
    cv::Mat source =imageVersions[whichImage].clone();

    cv::Mat region = cv::Mat(width, height, CV_8UC1, cv::Scalar(0));
    for(int x = topLeftX; x<topLeftX+width; x++)
        for(int y = topLeftY; y<topLeftY+height; y++)
        {
            region.at<uchar>(x-topLeftX, y-topLeftY) = source.at<uchar>(y, x);
        }
    //Pushing back the region of the original image in our storage
    imageVersions.push_back(region);
    imgCounter++;
    ostringstream oss;
    oss << "Image " << imgCounter << " . Region of image: " << whichImage;
    string description = oss.str();
    imageDescription.push_back(description);
    cout << description << endl;


    cv::calcHist(&region, 1, 0, cv::noArray(), histogram, 1, &histSize, &histRange, uniform, accumulate );
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
    ostringstream oss2;
    oss2 << "Image " << imgCounter << " . Histogram of image: " << whichImage;
    string description2 = oss2.str();
    imageDescription.push_back(description2);
    cout << description2 << endl;


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

void VisionMethods::adaptiveMedianFilter(int whichImage, int maxMaskSize)
{
    int maskSize = 3;
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
            sort(medianVec.begin(), medianVec.begin()+maskSize*maskSize); //Sorting the values in the mask and finding the median
            median = medianVec[medianIndex];

            if (median == 0 || median == 255) //If we still have salt or pepper in the median, we need to increase the mask size
            {
            medianVec.clear();
                while (maskSize <= maxMaskSize) //Keep increasing as long as our mask is under the max size
                {
                    maskSize = maskSize +2;
                    xMax = img.rows-(maskSize-1)/2; //We also need to update our indexes and bounds to not get any errors
                    yMax = img.cols-(maskSize-1)/2;
                    medianIndex = (maskSize*maskSize-1)/2; //The minus one is due to the index at vectors starting at 0
                    for(int maskX = -(maskSize-1)/2; maskX <= (maskSize-1)/2; maskX++) //Looping through the mask
                        for(int maskY = -(maskSize-1)/2; maskY <= (maskSize-1)/2; maskY++)
                        {
                            medianVec.push_back(img.at<uchar>(x+maskX, y+maskY));
                        }
                    sort(medianVec.begin(), medianVec.begin()+maskSize*maskSize); //Sorting the values in the mask and finding the median
                    median = medianVec[medianIndex];
                    medianVec.clear();

                    if(median != 0)
                        if(median != 255)
                            break;

                }
            }
            //Setting our mask size and indexes back to normal
            maskSize = 3;
            xMax = img.rows-(maskSize-1)/2; //We also need to update our indexes and bounds to not get any errors
            yMax = img.cols-(maskSize-1)/2;
            medianIndex = (maskSize*maskSize-1)/2; //The minus one is due to the index at vectors starting at 0
            medianVec.clear();
            img.at<uchar>(x, y) = median;
        }


    imageVersions.push_back(img);
    imgCounter++;
    ostringstream oss;
    oss << "Image " << imgCounter << " . Adaptive median filtered image: " << whichImage;
    string description = oss.str();
    imageDescription.push_back(description);
    cout << description << endl;
}

void VisionMethods::maxFilter(int whichImage, int maskSize)
{
    cv::Mat source =imageVersions[whichImage].clone();//Image using as source for the filter
    cv::Mat destination =imageVersions[whichImage].clone(); //Image used to write the filtered values to
    int xMax = source.rows-(maskSize-1)/2; //These integers asures that we dont acces the image outside of it's borders
    int yMax = source.cols-(maskSize-1)/2;  //(masksize-1)-2 is due to the 1 pixel in the center we are looking at, and divide by two as only the mask part facing the border is the problem
    vector<int> maskVector;
    int maxIndex = maskVector.size();
    int fHat;

    for (int x = (maskSize-1)/2; x <= xMax; x++) //These 2 for loops go through all of the picture //SHould prolly start at 0
        for (int y = (maskSize-1)/2; y<= yMax; y++)
        {
            for(int maskX = -(maskSize-1)/2; maskX <= (maskSize-1)/2; maskX++) //Looping through the mask
                for(int maskY = -(maskSize-1)/2; maskY <= (maskSize-1)/2; maskY++)
                {
                    maskVector.push_back(source.at<uchar>(x+maskX, y+maskY));
                }
            sort(maskVector.begin(), maskVector.begin()+maskVector.size());
            fHat = maskVector[maskVector.size()-1];
            /* DEBUGGING
            for (int i = 0; i< maskVector.size(); i++)
                cout << maskVector[i] << ", ";
            cout << endl;
            */
            maskVector.clear();
            destination.at<uchar>(x, y) = fHat;
        }


    imageVersions.push_back(destination);
    imgCounter++;
    ostringstream oss;
    oss << "Image " << imgCounter << " . Image restoration by max filtering image: " << whichImage;
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
        cv::namedWindow(imageDescription[i], cv::WINDOW_NORMAL);
        cv::imshow(imageDescription[i], imageVersions[i]);
    }
    cv::waitKey();
}
