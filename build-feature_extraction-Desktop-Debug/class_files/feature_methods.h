#ifndef FEATURE_METHODS_H
#define FEATURE_METHODS_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <sstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <chrono>

using namespace::std;
using namespace::cv;
using namespace std::chrono;

struct cluster
{
    vector<Point2f> points;
    int amountPoints;
    Point2f COM;
};

class feature_methods
{
public:
    feature_methods();
    static Mat toHSV(Mat inputImg);
    static Mat HSVtoBGR(Mat inputImg);
    static Mat findBlue(Mat inputImg, Mat outputImg, int lower, int higher);
    static Mat findRed(Mat inputImg, Mat outputImg, int lower, int higher);
    static Mat erodeWrap(Mat inputImg);
    static Mat dialateWrap(Mat inputImg);
    static vector<Vec3f> hoCircle(Mat inputImg, Mat origImg, bool showCircles);//Detecting circles using hough transform
    static vector<double> markerOneDetection(Mat &inputImg);
    static bool intersecting(Mat &img, Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r);
    static vector<cluster> findClusters(vector<Point2f> points, Mat &img);
    static vector<float> trackMarker2(Mat &img); //the output vector contains x, y.
    static vector<Point2f> locateIntersections(Mat &inputImg);
    static void assignCOM(cluster &oneCluster, bool paint, Mat &img);
    static bool newPoint(vector<Point2f> list, Point2f point);
    static void writeImg(Mat img, string name); //<-
    static void startClock(high_resolution_clock::time_point &t1);
    static void stopClock(high_resolution_clock::time_point &t1, high_resolution_clock::time_point &t2);
    static vector<float> findMarker2COM(vector<cluster> clusters, Mat &img);
  //  static void startPerformance(string name); //<-
   // static void endPerformance(string name); //<-

private:
   // high_resolution_clock::time_point t1;
    //high_resolution_clock::time_point t2;
};

#endif // FEATURE_METHODS_H
