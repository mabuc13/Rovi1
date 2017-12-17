#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <sstream>
#include "feature_methods.h"

using namespace std;
using namespace cv;


//vector<cluster> findClusters(vector<Point2f>);

//vector<float> trackMarker2(Mat img);

//bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r)
//{
//    Point2f x = o2 - o1;
//    Point2f d1 = p1 - o1;
//    Point2f d2 = p2 - o2;

//    float cross = d1.x*d2.y - d1.y*d2.x;
//    if (abs(cross) < /*EPS*/ 2)vector<float> trackMarker2(Mat img);
//    {
//        //cout << "returning false" << endl;
//        return false;
//    }
//    //cout << "Cross: " << cross << endl;
//   // if (cross == 0)
//   //     cross = 0.001;
//    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
//    //cout << "t1: " << t1 << endl;
//    r = o1 + d1 * t1;
//    //cout << "Intersection at location: (" << r.x << ", " << r.y << ")" << endl;
//    //Making two rectangles, where they each span over the space one of the lines takes up. If r then is within BOTH rectangles it will be a valid intersection
//    //A padding with a few pixels will make this approach good agains the case where the lines are either perfectly vertical and perfecly horizontal

//    //p1-o1 line
//    Point2f l1RectTL, l1RectLR; //eg. line 1 rectangle top left poiint
//    if (o1.x < p1.x)
//    {
//        l1RectTL.x = o1.x-1; //-1 and +1 is the padding
//        l1RectLR.x = p1.x+1;
//    }
//    else
//    {
//        l1RectTL.x = p1.x-1;
//        l1RectLR.x = o1.x+1;
//    }
//    if (o1.y < p1.y)
//    {
//        l1RectTL.y = o1.y-1;
//        l1RectLR.y = p1.y+1;
//    }
//    else
//    {
//        l1RectTL.y = p1.y-1;
//        l1RectLR.y = o1.y+1;
//    }

//    //p2-o2 line
//    Point2f l2RectTL, l2RectLR; //eg. line 1 rectangle top left poiint
//    if (o2.x < p2.x)
//    {
//        l2RectTL.x = o2.x-1; //-1 and +1 is the padding
//        l2RectLR.x = p2.x+1;
//    }
//    else
//    {
//        l2RectTL.x = p2.x-1;
//        l2RectLR.x = o2.x+1;
//    }
//    if (o2.y < p2.y)
//    {
//        l2RectTL.y = o2.y-1;
//        l2RectLR.y = p2.y+1;
//    }
//    else
//    {
//        l2RectTL.y = p2.y-1;
//        l2RectLR.y = o2.y+1;
//    }
////    //Methods for drawing the rectangles!
////    l1Rect = Rectangle(workspace, l1RectTL, l1RectLR,cv::Scalar(0, 0, 255 ));
////    l2Rect = Rectangle(workspace, l2RectTL, l2RectLR,cv::Scalar(0, 0, 255 ));
//    Rect l1Rect(l1RectTL, l1RectLR);
//    Rect l2Rect(l2RectTL, l2RectLR);
//    //cv::rectangle(img, pt1, pt2, cv::Scalar(0, 255, 0));
//    if (l1Rect.contains(r) && l2Rect.contains(r))
//        return true;
////    if (r.y > o1.y && r.y < p1.y && r.y < o2.y && r.y > p2.y ) //Checking if the intersection is within the y values of the lines
////    {
////        cout << "Y check makes it true" << endl;
////        return true;
////    }    //cout << "Intersection! " << o1.x << ", " << o1.y << " - " << p1.x << ", " << p1.y << " || " <<  o2.x << ", " << o2.y << " - " << p2.x << ", " << p2.y << endl;

////    if (r.x > o1.x && r.x < p1.x && r.x > o2.x && r.x < p2.x ) //Checking if the intersection is within x values of the lines
////    {
////        cout << "X check makes it true" << endl;
////        return true;
////    }
////    else
////        circle(workspace, r, 2, Scalar(0, 0, 255), -1, 8);
//    return false;
//}

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
    //cout << img.type() << endl;
    if (img.empty()) {
        std::cout << "Input image not found at '" << filename << "'\n";
        return 1;
    }


    //--------------TRACKING COLOR MARKER-----------------------------
    cout << "Tracking marker in picture: " << filename << endl;
    vector<double> center;
    high_resolution_clock::time_point t1, t2;
    feature_methods::startClock(t1);
    center = feature_methods::markerOneDetection(img);
    feature_methods::stopClock(t1, t2);
    string folder = "output/";
    filename.erase(0, 0);
    folder = folder + filename;
    feature_methods::writeImg(img, folder);
    //----------------------------------------------------------------
    //--------------TRACKING MARKER THINLINE--------------------------

    //    vector<float> centerOfMarker;
//    cout << "Tracking marker in picture: " << filename << endl;
//    high_resolution_clock::time_point t1, t2;
//    feature_methods::startClock(t1);
//    centerOfMarker = feature_methods::trackMarker2(img);
//    feature_methods::stopClock(t1, t2);
//    cout << "(x, y) coordinates of marker: (" << centerOfMarker[0] << ", " << centerOfMarker[1] << ")" << endl;
//    string folder = "output/";
//    filename.erase(0, 0);
//    folder = folder + filename;
//    feature_methods::writeImg(img, folder);
    //-------------------------------------------------------------------
    cout << "End of progam" << endl;
    return 0;
}

/* Probably a line intersection detection method with better results, buuut it's not working, it doesen't seem like the line iterators are that precise and will think a line has a point outside of its actual pixels
 *
 *
 *  Mat dst, cdst, grayImg;
    cvtColor(img, grayImg, CV_BGR2GRAY);
    Canny(grayImg, dst, 190, 200, 3);
    //imshow("after canny", dst);
    //waitKey(0);
    cdst = img.clone();
    //cvtColor(dst, cdst, CV_GRAY2BGR);waitKey(0);
    //dst = img.clone();
    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10);
    vector<Vec2i> sharedPoints;

 *     //Debugging parameters
    int pairCounter = 0;
    int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    double fontScale = 1;
    int thickness = 3;
    int baseline=0;
    int hitCounter = 0;
    baseline += thickness;
    for( size_t i = 0; i < lines.size(); i++ )
    {

        //Drawing the line number at the start of the line
        int Number = i;//number to convert int a string
        Vec4i l1 = lines[i];

        string Result;//string which will contain the result

        stringstream convert; // stringstream used for the conversion

        convert << Number;//add the value of Number to the characters in the stream

        Result = convert.str();//set Result to the content of the stream
        putText(img, Result, Point(l1[0], l1[1]), fontFace, fontScale, Scalar(0, 0, 255), 1, 8, false );
        //Outer loop, where one line is picked for comparison
        //line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);


        //Drawing lines found
        line( img, Point(l1[0], l1[1]), Point(l1[2], l1[3]), Scalar( 0, 255, 0 ), 2, 8 );

        for( size_t j = 0; j < lines.size(); j++ )
        {
            //cout << "Pair: " << pairCounter << " with line " << i << " and " << j << endl;
            pairCounter++;
            //Inner loop where another lines is picked to compare to the first
             Vec4i l2 = lines[j];
            //If we're comparing the same line with itself, don't do anything
            if(i != j)
            {

                LineIterator it(img, Point(l1[0], l1[1]), Point(l1[2], l1[3]), 4, 1);
                LineIterator it2(img, Point(l2[0], l2[1]), Point(l2[2], l2[3]), 4, 1);
                //Compare all points of the two line iterators
//                if (it.pos().x - it2.pos().x < 420) //Greater distances and they shouldn't both be starting on the marker
//                {
                    //Double loop looking for shared points in the two lines
                    for(int j2 = 0; j2 < it.count; j2++, it++)
                        for(int i2 = 0; i2 < it2.count; i2++, it2++)
                        {
                            if (hitCounter > 10)
                                break;
                            if (it.pos().x == it2.pos().x)
                                if (it.pos().y == it2.pos().y)
                                {
                                    Vec2i shared;
                                    shared[0] = it2.pos().x;
                                    shared[1] = it2.pos().y;
                                    cout << "iterator1 (x, y): (" << it.pos().x << ", " << it.pos().y << " Line number: " << i << ")\n";
                                    cout << "iterator2 (x, y): (" << it2.pos().x << ", " << it2.pos().y << " Line number: " << j << ")\n\n";
                                    cout << "HIT between line " << i << " and " << j << endl;
                                    sharedPoints.push_back(shared);
                                    circle(img, Point(shared[0], shared[1]), 2, Scalar(255, 0, 0), -1, 8);

                                    string Result2;//string which will contain the result

                                    stringstream convert2; // stringstream used for the conversion

                                    convert2 << i << 99 << j;//add the value of Number to the characters in the stream

                                    Result2 = convert2.str();//set Result to the content of the stream

                                    putText(img, Result2, Point(shared[0], shared[1]-40), fontFace, fontScale, Scalar(255, 0, 255), 1, 8, false );
                                    hitCounter++;
                                }
                        }
                //}

            }

        }

//        LineIterator it(img, Point(l[0], l[1]), Point(l[2], l[3]), 8);
//        LineIterator it2 = it;
//        vector<Vec3b> buf(it.count);
//        cout << "Points along line " << i << endl;
//        for(int j = 0; j < it.count; j++, ++it)
//            cout << "(" << it.pos().x << ", " << it.pos().y << ") ";
        //All the lines are stored in lines where every line is sorted with ascending x-values
        //To check for intersections all lines have to be compared to each other
        //To reduce the computational workload the starting x-values of 2 lines could be compared
    //If the distance between the first x-values are too big, there's no need to run through all of the points
    //Gather all the points with the same x and y values in a new vector
    }

 */


