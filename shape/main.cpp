// #include "ShapeDetector.h"
// int main(int argc, char* argv[])
// {
//     const string imgpath = "D:/TMP/test.png";
//     Mat image = imread(imgpath);
//     if (image.empty())
//     {
//         return -1;
//     }
//     Mat gray;
//     if (image.channels()==3)
//     {
//         cvtColor(image, gray, COLOR_BGR2GRAY);
//     }
//     else
//     {
//         gray = image.clone();
//     }

//     Mat blurred, thresh;
//     GaussianBlur(gray, blurred, Size(5, 5), 0.0);
//     threshold(blurred, thresh, 60, 255, THRESH_BINARY);

//     vector< vector<Point> > contours;
//     vector<Vec4i> hierarchy;
//     findContours(thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

//     ShapeDetector sd;
//     vector<Point> c;
//     for (size_t i = 0; i < contours.size(); i++)
//     {
//         c = contours[i];
//         Rect crect = boundingRect(c);
//         // compute the center of the contour, then detect the name of the
//         // shape using only the contour
//         Moments M = moments(c);
//         int cX = static_cast<int>(M.m10 / M.m00);
//         int cY = static_cast<int>(M.m01 / M.m00);
//         sd.detect(Mat(c));
//         string shape = sd.get_shape_type();
//         drawContours(image, contours, i, Scalar(0, 255, 0), 2);
//         Point pt(cX, cY);
//         putText(image, shape, pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);
//         imshow("Image", image);
//         waitKey(0);
//     }
//     return 0;
// }


// #include "ShapeDetector.h"
// int main(int argc, char* argv[])
// {
//     const string imgpath = "D:/TMP/test.png";
//     Mat image = imread(imgpath);
//     if (image.empty())
//     {
//         return -1;
//     }
//     Mat gray;
//     if (image.channels()==3)
//     {
//         cvtColor(image, gray, COLOR_BGR2GRAY);
//     }
//     else
//     {
//         gray = image.clone();
//     }

//     Mat blurred, thresh;
//     GaussianBlur(gray, blurred, Size(5, 5), 0.0);
//     threshold(blurred, thresh, 60, 255, THRESH_BINARY);

//     vector< vector<Point> > contours;
//     vector<Vec4i> hierarchy;
//     findContours(thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

//     ShapeDetector sd;
//     vector<Point> c;
//     for (size_t i = 0; i < contours.size(); i++)
//     {
//         c = contours[i];
//         Rect crect = boundingRect(c);
//         // compute the center of the contour, then detect the name of the
//         // shape using only the contour
//         Moments M = moments(c);
//         int cX = static_cast<int>(M.m10 / M.m00);
//         int cY = static_cast<int>(M.m01 / M.m00);
//         sd.detect(Mat(c));
//         string shape = sd.get_shape_type();
//         drawContours(image, contours, i, Scalar(0, 255, 0), 2);
//         Point pt(cX, cY);
//         putText(image, shape, pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);
//         imshow("Image", image);
//         waitKey(0);
//     }
//     return 0;
// }

#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include<vector>
using namespace cv;
using namespace std;

int main(int argc,char** argv)
{
    Mat srcImg, tempImg;
    srcImg = imread(argv[1]);
    if (!srcImg.data)
    {
        cout << "no Img" << endl;
        return -1;
    }
    Mat gray;
    resize(srcImg, tempImg, Size(srcImg.cols / 2, srcImg.rows / 2), 0, 0);
    cvtColor(tempImg, gray, COLOR_RGB2GRAY);
    medianBlur(gray, gray, 3);

    vector<Vec3f>circles;//圆
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, srcImg.rows / 20, 100, 60, 0, 0);

    Mat thresh;
    threshold(gray, thresh, 0, 255, THRESH_OTSU);
    
    vector<Point>sanjiao;

    vector<Point>approx;
    vector<Point>squares;

    Mat srcImg1;
    srcImg1 = thresh.clone();
    vector<vector<Point>>contours;
    findContours(srcImg1, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
    Mat dstImg(srcImg1.rows, srcImg1.cols, CV_8UC3, Scalar(255, 255, 255));
    for (size_t i = 0; i < circles.size(); i++)//画圆
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(dstImg, center, radius, Scalar(0, 255, 0), 5, 8, 0);
        circle(dstImg, center, 3, Scalar(0, 255, 0), -1);
        cout << "圆心" << i + 1 << center << endl;
    }
    // for (size_t i = 0; i < contours.size(); i++)
    // {
    //     approxPolyDP(contours[i], approx, arcLength(Mat(contours[i]), true)*0.02, true);
    //     if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
    //     {
    //         double minDist = 1e10;

    //         for (int i = 0; i < 4; i++)
    //         {
    //             Point side = approx[i] - approx[(i + 1) % 4];
    //             double squaredSideLength = side.dot(side);
    //             minDist = min(minDist, squaredSideLength);
    //         }
    //         if (minDist<50)
    //             break;
    //         for (int i = 0; i<4; i++)
    //             squares.push_back(Point(approx[i].x, approx[i].y));
    //     }
    //     approxPolyDP(contours[i], approx, arcLength(Mat(contours[i]), true)*0.1, true);
    //     if (approx.size() == 3 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
    //     {
    //         double minDist = 1e10;

    //         for (int i = 0; i <3; i++)
    //         {
    //             Point side = approx[i] - approx[(i + 1) % 3];
    //             double squaredSideLength = side.dot(side);
    //             minDist = min(minDist, squaredSideLength);
    //         }
    //         if (minDist<50)
    //             break;
    //         for (int i = 0; i<3; i++)
    //             sanjiao.push_back(Point(approx[i].x, approx[i].y));
    //     }
    //     drawContours(dstImg, contours, i, Scalar(0,0,255), 3);
    // }
    for (size_t i = 0; i < squares.size(); i += 4)
    {
        Point center;
        center.x = (squares[i].x + squares[i + 2].x) / 2;
        center.y = (squares[i].y + squares[i + 2].y) / 2;
        line(dstImg, squares[i], squares[i + 1], Scalar(0, 0, 255), 4);
        line(dstImg, squares[i + 1], squares[i + 2], Scalar(0, 0, 255), 4);
        line(dstImg, squares[i + 2], squares[i + 3], Scalar(0, 0, 255), 4);
        line(dstImg, squares[i + 3], squares[i], Scalar(0, 0, 255), 4);
        cout << "矩形中心" << (i + 1) % 4 << center << endl;
        circle(dstImg, center, 3, Scalar(0, 0, 255), -1);
    }
    // for (size_t i = 0; i < sanjiao.size(); i += 3)
    // {
    //     Point center;
    //     center.x = (sanjiao[i].x + sanjiao[i + 1].x + sanjiao[i + 2].x) / 3;
    //     center.y = (sanjiao[i].y + sanjiao[i + 1].y + sanjiao[i + 2].y) / 3;
    //     line(dstImg, sanjiao[i], sanjiao[i + 1], Scalar(255, 0, 0), 4);
    //     line(dstImg, sanjiao[i + 1], sanjiao[i + 2], Scalar(255, 0, 0), 4);
    //     line(dstImg, sanjiao[i], sanjiao[i + 2], Scalar(255, 0, 0), 4);
    //     cout << "三角形中心" << (i + 1) % 3 << center << endl;
    //     circle(dstImg, center, 3, Scalar(255, 0, 0), -1);
    // }
    imshow("123", dstImg);
    waitKey(0);
    return 0;
}