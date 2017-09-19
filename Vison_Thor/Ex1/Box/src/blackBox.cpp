

#include </opt/ros/kinetic/include/opencv-3.2.0-dev/opencv2/opencv.hpp>
#include <iostream>
#include <vector>

//using namespace std;

int main(int argc, char* argv[] )
{
    std::cout << "hello world"<<std::endl;
    
    cv::CommandLineParser parser(argc, argv,
        "{help   |            | print this message}"
        "{@image | ./book.jpg | image path}"
    );
    
        //Load and image
    
    std::string filename = parser.get<std::string>("@image");
    cv::Mat orig = cv::imread(filename);

    if (orig.empty()) {
        std::cout << "Input image not found at '" << filename << "'\n";
        return 1;
    }
    cv::imshow("Original Image",orig);

/*
** Converting to a gray-scale image
*/
    cv::Mat gray_img;
    cvtColor(orig, gray_img, CV_BGR2GRAY );
    
    for(int i =100 ; i<220 ; i++){
        for(int j =350 ; j<440; j++){
            uchar temp = 0;
            gray_img.at<uchar>(i,j) = temp;
        }    
    }
    cv::imshow("gray image",gray_img);

/* 
** Drawing a black square with the use of cv::mat.at func    
*/
    cv::Mat newOne;
    newOne = orig.clone();
    //cv::imshow("Cloned",newOne);
    
    for(int i =100 ; i<220 ; i++){
        for(int j =350 ; j<440; j++){
            newOne.at<cv::Vec3b>(i,j) = cv::Vec3b::all(0);
        }    
    }
    cv::imshow("Boxed image",newOne);
/*
** Drawing a black coloured square with a pointer method
*/
    cv::Mat aPointed;
    aPointed = orig.clone();
    
    
    for(int i = 100; i<220; i++){    
    cv::Vec3b* p_row =aPointed.ptr<cv::Vec3b>(i);
        
        for(int j = 350 ; j<440 ; j++){
        p_row[j] = cv::Vec3b::all(0);
            }
        }
    //cv::imshow("Box2",aPointed);


/*
** Drawing a black box in the beginning and then copying/merging it onto the image
*/   
    cv::Mat_<cv::Vec3b> boxImg(orig.rows,orig.cols);
    //cv::Mat boxImg(orig.rows,orig.cols, CV_8U);
    for(int i=0 ; i<boxImg.rows ; i ++) {

        for(int j=0; j< boxImg.cols; j++)   {

        if ( i <100){
            
            boxImg.at<cv::Vec3b>(i,j) = cv::Vec3b::all(100);
                    }
        if ( i >220){
            boxImg.at<cv::Vec3b>(i,j) = cv::Vec3b::all(100);
                    }
        if ( j <350){
            boxImg.at<cv::Vec3b>(i,j) = cv::Vec3b::all(100);
                    }
        if ( j >440){
            boxImg.at<cv::Vec3b>(i,j) = cv::Vec3b::all(100);
                    }
        }
    }

    cv::imshow("box3",boxImg);
    cv::Mat merg;
    cv::Mat dummie;    
    merg = orig.clone();
    merg.copyTo(dummie,boxImg);
    cv::imshow("Copied to location",dummie);

/*
** Now we want to change the color when we put in our box(es)
*/
    cv::Vec3b black;
    cv::Vec3b color;
    black = (0,0,0);
    //color = (255,255,255,255);
    int count = 0;
    for(int i= 0 ; i < dummie.rows; i++){
        for(int j = 0 ; j<dummie.cols; j++){
        if(dummie.at<cv::Vec3b>(i,j) == black ){
            dummie.at<cv::Vec3b>(i,j) [0]= 100;
            dummie.at<cv::Vec3b>(i,j) [1]= 100;
            dummie.at<cv::Vec3b>(i,j) [2]= 0;
            count = count +1;            
            //dummie.at<Vec3b>(i,j) = Vec255,10,30;
            }        
        }
    }
    
    std::cout<<count<<std::endl;
    cv::imshow("new dummie",dummie);


    cv::waitKey(0);
    return 0;
}
