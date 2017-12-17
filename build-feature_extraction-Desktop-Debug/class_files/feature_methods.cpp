#include "feature_methods.h"

feature_methods::feature_methods()
{
}

Mat feature_methods::toHSV(Mat inputImg)
{
    Mat img_hsv;
    cvtColor(inputImg,img_hsv,CV_BGR2HSV);

    return img_hsv;
}

Mat feature_methods::HSVtoBGR(Mat inputImg)
{
    Mat bgrImg;
    cvtColor(inputImg, bgrImg, CV_HSV2BGR);
    bgrImg *= 255;

    return bgrImg;
}

Mat feature_methods::findBlue(Mat inputImg, Mat outputImg, int lower, int higher)
{

    inRange(inputImg,Scalar(lower, 100, 50), Scalar(higher,255, 100), outputImg);
    outputImg = erodeWrap(outputImg);
    outputImg = dialateWrap(outputImg);
    return outputImg;

    /*
     *
    blue = feature_methods::findBlue(img_HSV, blue, 110, 130);
    Mat red = img_HSV.clone();
    red = feature_methods::findRed(img_HSV, red, 130, 180);
     */
}

Mat feature_methods::findRed(Mat inputImg, Mat outputImg, int lower, int higher)
{

    Mat1b mask1, mask2;
    inRange(inputImg, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
    inRange(inputImg, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);
    outputImg = mask1 | mask2;
    outputImg = erodeWrap(outputImg);
    outputImg = dialateWrap(outputImg);
    return outputImg;
    //Inspiration: https://stackoverflow.com/questions/32522989/opencv-better-detection-of-red-color
}

Mat feature_methods::erodeWrap(Mat inputImg)
{
    Mat erosion_dst;

    int erosion_elem = 2;
    int erosion_size = 3;

    int const max_elem = 2;
    int const max_kernel_size = 21;

    int erosion_type;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
        else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
            else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

     Mat element = getStructuringElement( erosion_type,
                                          Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                          Point( erosion_size, erosion_size ) );

     /// Apply the erosion operation
     cv::erode( inputImg, erosion_dst, element);
     return erosion_dst;

}

Mat feature_methods::dialateWrap(Mat inputImg)
{
    Mat dilation_dst;

    int dilation_elem = 0;
    int dilation_size = 3;

    int dilation_type = 2;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
        else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
            else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement( dilation_type,
                                           Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                           Point( dilation_size, dilation_size ) );
    /// Apply the dilation operation
    dilate( inputImg, dilation_dst, element );

    return dilation_dst;
}

vector<Vec3f> feature_methods::hoCircle(Mat inputImg, Mat origImg, bool showCircles)
{
    //Inspiration: https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_circle/hough_circle.html
    //Needed for houghCircle actually being able to detect circles
    GaussianBlur( inputImg, inputImg, Size(9, 9), 2, 2 );

    //Vector where the circles will be stored [x, y, r]
    vector<Vec3f> circles;

    //Hough circle detection using the canny edge dectortor
    //Parameters after the 1 are: distance between circles, , canny upper trheshold, treshhold for center detection
    //Minimum radio to tbe detected, uknown set it to zero.
    //Maximum radius, also set default to zero if unkonwn
    HoughCircles( inputImg, circles, CV_HOUGH_GRADIENT, 1, inputImg.rows/8, 100, 20, 0, 0 );

    //Drawing circles if showCircles is true
    if (showCircles)
    {
        Mat drawingImg = origImg.clone();

        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle( drawingImg, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            circle( drawingImg, center, radius, Scalar(0,0,255), 3, 8, 0 );
         }

        namedWindow( "Hough Circle detection", CV_WINDOW_AUTOSIZE );
        imshow( "Hough Circle detection", drawingImg );
        waitKey(0);
    }

    return circles;
}

vector<double> feature_methods::markerOneDetection(Mat &inputImg)
{
    vector<double> returnVector;
    Mat img_HSV = feature_methods::toHSV(inputImg);

    Mat blue = img_HSV.clone();

    blue = feature_methods::findBlue(img_HSV, blue, 110, 130);
    Mat red = img_HSV.clone();
    red = feature_methods::findRed(img_HSV, red, 130, 180);
    vector<Vec3f> blueCircles = feature_methods::hoCircle(blue, inputImg, false);// <------- HER LIGGER der 3 punkter til dig, hvis du får sådanne lyster brug find x ved redCircles[i].x samme med y
    vector<Vec3f> redCircles = feature_methods::hoCircle(red, inputImg, false);
    vector<Vec3f> allCircles = blueCircles;
    allCircles.push_back(redCircles[0]);
    //Find the COM of the whole marker, sum up individual x and y coordinates then divide
    double sumX, sumY = 0.0;
    double comX, comY = 0.0;
    for (auto i = 0; i < allCircles.size(); i++)
    {
        sumX += allCircles[i][0];
        sumY += allCircles[i][1];
    }
    comX = sumX/allCircles.size();
    comY = sumY/allCircles.size();
    Point2f COMPoint;
    COMPoint.x = comX;
    COMPoint.y = comY;
    circle(inputImg, COMPoint, 10, Scalar(255, 255, 255), 2, 8);
    cout << "Center of marker (x, y): (" << comX << ", " << comY << ")" << endl;
    returnVector.push_back(comX);
    returnVector.push_back(comY);
    return returnVector;
}

vector<cluster> feature_methods::findClusters(vector<Point2f> points, Mat &img)
{
    float acceptDistance = 8.0;
    vector<cluster> returnVector;
    Point2f dummy;
    vector<Point2f> vecDummy;
    for (auto i = 0; i < points.size(); i++)
    {
        //cout << "Outer for loop" << endl;
        cluster oneCluster;
       // cout << "Cluster nr: " << i << endl;
        oneCluster.amountPoints = 0;
        oneCluster.COM = dummy;
        oneCluster.points = vecDummy;
        oneCluster.points.push_back(points[i]);
        oneCluster.amountPoints++;
       // cout << "Added point with x, y : " << points[i].x << ", " << points[i].y << endl;
        for (auto j = 0; j < points.size(); j++)
        {
            if (i != j)
            {
                //cout << j << " " << points.size() << endl;
                float distance = sqrt(pow(points[i].x - points[j].x, 2) + pow(points[i].y - points[j].y, 2));
                //cout << "Distance: " << distance << endl;
                //cout << "Distance calculated" << endl;
                if (distance < acceptDistance)
                {
                    if (newPoint(oneCluster.points, points[j]))
                    {
                        //cout << "i, j: " << i << ", " << j << endl;
                        //cout << "Added point with x, y : " << points[j].x << ", " << points[j].y << endl;
                        //cout << "Do we ever accept a distance" << endl;
                        oneCluster.points.push_back(points[j]);
                        oneCluster.amountPoints++;
                    }
                }
                //cout << "Distance accepted" << endl;
            }
        }
        //cout << oneCluster.amountPoints << endl;
        if ((oneCluster.points.size() > 3) && (oneCluster.points.size() < 10))
        {

            assignCOM(oneCluster, 1, img);
            returnVector.push_back(oneCluster);
        }
    }
    //cout << "Finished findClusters" << endl;
    return returnVector;
}

vector<float> feature_methods::trackMarker2(Mat &img)
{
    //    namedWindow("trackmarker2", false);
    //    imshow("trackmarker2", img);
    //    waitKey(0);
    Mat passingImage = img.clone();
   // cout << "Entering locateIntersections" << endl;
    vector<Point2f> intersections;
    //cout << "Entering locateIntersections" << endl;
    intersections = locateIntersections(img);
   // cout << "Entering locateIntersections" << endl;
    vector<cluster> clusters = findClusters(intersections, img);
    vector<float> result = findMarker2COM(clusters, img);
    return result;

}

vector<Point2f> feature_methods::locateIntersections(Mat &inputImage)
{
    //Finding marker 2
    vector<Vec4i> lines;
    vector<Point2f> intersections;
    Mat grayImg, canny;
    cvtColor(inputImage, grayImg, CV_BGR2GRAY);
    Canny(inputImage, canny, 50, 200, 3);
    HoughLinesP(canny, lines, 1, CV_PI/180, 50, 50, 10);

    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l1 = lines[i];
        //Drawing the line number at the start of the line

        //Drawing lines found
        //line( inputImage, Point(l1[0], l1[1]), Point(l1[2], l1[3]), Scalar( 0, 255, 0 ), 2, 8 );
//        cout << "l1 start (x, y): " << l1[0] << ", " << l1[1] << endl;
//        cout << "l1 end (x, y): " << l1[2] << ", " << l1[3] << endl;
        for( size_t j = 0; j < lines.size(); j++ )
        {
            Vec4i l2 = lines[j];
            //cout << "Pair: " << pairCounter << " with line " << i << " and " << j << endl;
            //Inner loop where another lines is picked to compare to the first
            //If we're comparing the same line with itself, don't do anything
            if(i != j)
            {
//                cout << "l2 start (x, y): " << l2[0] << ", " << l2[1] << endl;
//                cout << "l2 end (x, y): " << l2[2] << ", " << l2[3] << endl;
             //We always want to make the line with the smallest X start value o1, due to checking wether the intersection is within the box
//             if ((l1[0] > l2[0]) && (l1[1] < l2[1]))
//             {
//                 cout << "How often do we swap l1 and l2" << endl;
//                 Vec4i tmp = l1;
//                 l1 = l2;
//                 l2 = tmp;
//                 cout << "l1: " << l1;
//                 cout << "l2: " << l2;
//             }
             Point2f crossSection, o1, o2, p1, p2;
             o1.x = (float)l1[0];
             o1.y  = (float)l1[1];
             p1.x = (float)l1[2];
             p1.y = (float)l1[3];
             o2.x = (float)l2[0];
             o2.y  = (float)l2[1];
             p2.x = (float)l2[2];
             p2.y = (float)l2[3];
             if (intersecting(inputImage, o1, p1, o2, p2, crossSection))
             {
                 if (newPoint(intersections, crossSection))
                 {
                     circle(inputImage, crossSection, 2, Scalar(255, 0, 0), -1, 8);
                     //cout << "Found intersection with x, y: " << crossSection.x << ", " << crossSection.y << endl;
                     intersections.push_back(crossSection);

                 }

             }

            }

        }

    }
    return intersections;
}

void feature_methods::assignCOM(cluster &oneCluster, bool paint, Mat &img)
{
   // cout << "Entering assignCOM" << endl;
    float sumX;
    float sumY;
    for (auto i = 0; i < oneCluster.points.size(); i++)
    {
        sumX += oneCluster.points[i].x;
        //cout << "Clusterpoint with size, x, y: " << oneCluster.points.size() << ", " << oneCluster.points[i].x << ", " << oneCluster.points[i].y << endl;
        sumY += oneCluster.points[i].y;
    }
    oneCluster.COM.x = sumX/oneCluster.points.size();
    oneCluster.COM.y = sumY/oneCluster.points.size();
    if (paint)
    {
        Point2f COMPoint;
        COMPoint.x = oneCluster.COM.x;
        //cout << "Compoint x, y: " << oneCluster.COM.x <<", " << oneCluster.COM.y << endl;
        COMPoint.y = oneCluster.COM.y;
        circle(img, COMPoint, 10, Scalar(0, 0, 255), 2, 8);
    }
//        namedWindow("assignCOM", false);
//        imshow("assignCOM", img);
//        waitKey(0);
}

bool feature_methods::intersecting(Mat &img, Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r)
{

    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < /*EPS*/ 2)//vector<float> trackMarker2(Mat img);
    {
        //cout << "returning false" << endl;
        return false;
    }
    //cout << "Cross: " << cross << endl;
   // if (cross == 0)
   //     cross = 0.001;
    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    //cout << "t1: " << t1 << endl;
    r = o1 + d1 * t1;
    //cout << "Intersection at location: (" << r.x << ", " << r.y << ")" << endl;
    //Making two rectangles, where they each span over the space one of the lines takes up. If r then is within BOTH rectangles it will be a valid intersection
    //A padding with a few pixels will make this approach good agains the case where the lines are either perfectly vertical and perfecly horizontal

    //p1-o1 line
    Point2f l1RectTL, l1RectLR; //eg. line 1 rectangle top left poiint
    if (o1.x < p1.x)
    {
        l1RectTL.x = o1.x-1; //-1 and +1 is the padding
        l1RectLR.x = p1.x+1;
    }
    else
    {
        l1RectTL.x = p1.x-1;
        l1RectLR.x = o1.x+1;
    }
    if (o1.y < p1.y)
    {
        l1RectTL.y = o1.y-1;
        l1RectLR.y = p1.y+1;
    }
    else
    {
        l1RectTL.y = p1.y-1;
        l1RectLR.y = o1.y+1;
    }

    //p2-o2 line
    Point2f l2RectTL, l2RectLR; //eg. line 1 rectangle top left poiint
    if (o2.x < p2.x)
    {
        l2RectTL.x = o2.x-1; //-1 and +1 is the padding
        l2RectLR.x = p2.x+1;
    }
    else
    {
        l2RectTL.x = p2.x-1;
        l2RectLR.x = o2.x+1;
    }
    if (o2.y < p2.y)
    {
        l2RectTL.y = o2.y-1;
        l2RectLR.y = p2.y+1;
    }
    else
    {
        l2RectTL.y = p2.y-1;
        l2RectLR.y = o2.y+1;
    }
//    //Methods for drawing the rectangles!
//    l1Rect = Rectangle(workspace, l1RectTL, l1RectLR,cv::Scalar(0, 0, 255 ));
//    l2Rect = Rectangle(workspace, l2RectTL, l2RectLR,cv::Scalar(0, 0, 255 ));
    Rect l1Rect(l1RectTL, l1RectLR);
    Rect l2Rect(l2RectTL, l2RectLR);
    //cv::rectangle(img, pt1, pt2, cv::Scalar(0, 255, 0));
    if (l1Rect.contains(r) && l2Rect.contains(r))
        return true;
//    if (r.y > o1.y && r.y < p1.y && r.y < o2.y && r.y > p2.y ) //Checking if the intersection is within the y values of the lines
//    {
//        cout << "Y check makes it true" << endl;
//        return true;
//    }    //cout << "Intersection! " << o1.x << ", " << o1.y << " - " << p1.x << ", " << p1.y << " || " <<  o2.x << ", " << o2.y << " - " << p2.x << ", " << p2.y << endl;

//    if (r.x > o1.x && r.x < p1.x && r.x > o2.x && r.x < p2.x ) //Checking if the intersection is within x values of the lines
//    {
//        cout << "X check makes it true" << endl;
//        return true;
//    }
//    else
//        circle(workspace, r, 2, Scalar(0, 0, 255), -1, 8);
    return false;
}

bool feature_methods::newPoint(vector<Point2f> list, Point2f point)
{
    //Find the difference between the two points, and THEN see if it's higher than a low value
   // cout << "Entering newPoint with point x, y: " << point.x << ", " << point.y << endl;

    for (auto i = 0; i < list.size(); i++)
    {
        float distance = sqrt(pow(list[i].x - point.x, 2) + pow(list[i].y - point.y, 2));
        //cout << "Comparing to this point: " << list[i].x << ", " << list[i].y << endl;
        //cout << "Distance: " << distance << endl;
        if (abs(distance) < 0.001)
        {

               // cout << "fucking falsified biiitch" << endl;
                return false;
        }

    }
    //cout << "Returned true!" << endl;
    return true;
}

void feature_methods::writeImg(Mat img, string name)
{
    imwrite(name, img);
}

void feature_methods::startClock(high_resolution_clock::time_point &t1)
{
    t1 = high_resolution_clock::now();

}

void feature_methods::stopClock(high_resolution_clock::time_point &t1, high_resolution_clock::time_point &t2)
{
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t2 - t1).count();
    cout << "The methods execution time was: " << duration << " microseconds." << endl;
}

vector<float> feature_methods::findMarker2COM(vector<cluster> clusters, Mat &img)
{
    float sumX;
    float sumY;
    vector<float> returnVector;
    float resultX, resultY;
    for (auto i = 0; i < clusters.size(); i++)
    {
        sumX += clusters[i].COM.x;
        //cout << "Clusterpoint with size, x, y: " << oneCluster.points.size() << ", " << oneCluster.points[i].x << ", " << oneCluster.points[i].y << endl;
        sumY += clusters[i].COM.y;
    }
    resultX = sumX/clusters.size();
    resultY = sumY/clusters.size();

    Point2f COMPoint;
    COMPoint.x = resultX;
    //cout << "Compoint x, y: " << oneCluster.COM.x <<", " << oneCluster.COM.y << endl;
    COMPoint.y = resultY;
    circle(img, COMPoint, 10, Scalar(255, 255, 255), 2, 8);
    returnVector.push_back(resultX);
    returnVector.push_back(resultY);
    return returnVector;
}
