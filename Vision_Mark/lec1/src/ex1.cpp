#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    // Parse command line arguments
    cv::CommandLineParser parser(argc, argv,
        "{help   |            | print this message}"
        "{@image | ./book.jpg | image path}"
    );

    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }
    // Load image
    std::string filename = parser.get<std::string>("@image");
    cv::Mat img = cv::imread(filename);

    if (img.empty()) {
        std::cout << "Input image not found at '" << filename << "'\n";
        return 1;
    }

    //Method 1 for black box painting
    cv::Mat box1;
    box1 = img.clone();
    cv::Vec3b temp;
    int brush;
    for (int i = 100; i<220; i++)
        for (int j = 350; j<440; j++)
            {
                for (int c = 0; c<3; c++)
                    {
                        if (c == 0)
                            brush = 255;
                        else
                            brush = 0;
                        temp[c] = brush;             
                    }
                    box1.at<cv::Vec3b>(i,j) = temp;
            }
    //Method 2 for black box painting
    cv::Mat box2;
    box2 = img.clone();
    
    for (int i = 100; i<220; i++)
        {
        cv::Vec3b* p_row = box2.ptr<cv::Vec3b>(i);
        for (int j = 350; j<440; j++)
            {
                p_row[j] = cv::Vec3b::all(0);
            }
        }
    //Method 3 for black box painting
    cv::Mat_<cv::Vec3b> box3(img.rows, img.cols);     
    for (int i = 0; i<img.rows; i++)
        for (int j = 0; j<img.cols; j++)
            {     
                if( !(!(i <100 || i>220) && !(j<350 || j>440)) )
                    {
                        box3.at<cv::Vec3b>(i,j) = cv::Vec3b::all(1);
                        //cv::Vec3b intensity = box3.at<cv::Vec3b>(i,j); Template for accesing individual channels
                        //int output = intensity.val[0];
                        //std::cout << output << std::endl;
                    }
                else
                    box3.at<cv::Vec3b>(i,j) = cv::Vec3b::all(0);                           
            }
    uchar blue, green, red;
    int   blueint1, greenint1, redint1;
    int blueint2, greenint2, redint2;
    int bluetemp, greentemp, redtemp;
    cv::Vec3b output;
    cv::Mat_<cv::Vec3b> box3_o(img.rows, img.cols); 
    for (int i = 0; i<img.rows; i++)
        for (int j = 0; j<img.cols; j++)
            {   
                //By default is uchar values in the vec3b datatype. Therefore i make it to ints, so i'm able to multiply the 2 images on each other.
                cv::Vec3b intensity1 = box3.at<cv::Vec3b>(i,j);
                cv::Vec3b intensity2 = img.at<cv::Vec3b>(i,j);
                blueint1 = intensity1.val[0];
                blueint2 = intensity2.val[0];
                greenint1 = intensity1.val[1];
                greenint2 = intensity2.val[1];
                redint1 = intensity1.val[2];
                redint2 = intensity2.val[2];
                bluetemp = blueint1*blueint2;
                greentemp = greenint1*greenint2;
                redtemp = redint1*redint2;
                blue = bluetemp;
                green = greentemp;
                red = redtemp;
                output.val[0] = blue;
                output.val[1] = green;
                output.val[2] = red;
                box3_o.at<cv::Vec3b>(i,j) = output;
            }

   //Greyscale magic
    cv::Mat gray_img;
    cvtColor(img, gray_img, CV_BGR2GRAY);
    
    for (int i = 100; i<220; i++)
        {
        for (int j = 350; j<440; j++)
            {
                uchar tmp = 0;
                gray_img.at<uchar>(i,j) = tmp;
            }
        }    


    // Show the images
    cv::imshow("Image", img);
    cv::imshow("Box1", box1);
    cv::imshow("Box2", box2);
    cv::imshow("Box3", box3_o);
    cv::imshow("Fifty shades of grey", gray_img);
    cv::waitKey();
    
    return 0;
}
