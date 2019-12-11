#include <iostream>
#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include<vector>
using namespace cv;
using namespace std;

string Convert(float Num)
{
  ostringstream oss;
  oss<<Num;
  string str(oss.str());
  return str;
}

float cvMax(cv::Mat& mat) 
{ 
float max=0; 
float* pData=(float*)mat.data; 
for(int i=0;i<mat.rows;i++) 
{ 
    for(int j=0;j<mat.cols;j++) 
    { 
     float value = pData[j+i*mat.cols]; 
     if(value>max) 
     {
      cout<<max<<endl;
      max=value; 
     } 
    } 
} 
return max; 
} 

int main(int argc, char** argv)
{
  VideoCapture cap(0); //capture the video from web cam
  
  if (!cap.isOpened())  // if not success, exit program
  {
    cout << "Cannot open the web cam" << endl;
    return -1;
  }

  while (true)
  {
    Mat imgOriginal;

    bool bSuccess = cap.read(imgOriginal); // read a new frame from video
    if (!bSuccess) //if not success, break loop
    {
      cout << "Cannot read a frame from video stream" << endl;
      break;
    }

    imshow("Original", imgOriginal); //show the original image
    Mat means, stddev;
//   //计算：彩色图像三通道的均值与方差
    meanStdDev(imgOriginal, means, stddev);
    // printf("Channel--[Blue]  mean: %.2f, stddev: %.2f\n", means.at<double>(0, 0), stddev.at<double>(0, 0));
    // printf("Channel--[Green] mean: %.2f, stddev: %.2f\n", means.at<double>(1, 0), stddev.at<double>(1, 0));
    // printf("Channel--[Red]   mean: %.2f, stddev: %.2f\n", means.at<double>(2, 0), stddev.at<double>(2, 0));

    Mat gray,tempImg;
    resize(imgOriginal, tempImg, Size(imgOriginal.cols / 2, imgOriginal.rows / 2), 0, 0);
    cvtColor(tempImg, gray, COLOR_RGB2GRAY);
    medianBlur(gray, gray, 3);

    vector<Vec3f>circles;//圆
    vector<Point>approx;
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, imgOriginal.rows / 20, 100, 60, 0, 0);
    vector<Point>squares;
    vector<vector<Point>>contours;
    Mat srcImg1;
    Mat thresh;
    threshold(gray, thresh, 0, 255, THRESH_OTSU);
    srcImg1 = thresh.clone();
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

        for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], approx, arcLength(Mat(contours[i]), true)*0.02, true);
        if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
        {
            double minDist = 1e10;

            for (int i = 0; i < 4; i++)
            {
                Point side = approx[i] - approx[(i + 1) % 4];
                double squaredSideLength = side.dot(side);
                minDist = min(minDist, squaredSideLength);
            }
            if (minDist<50)
                break;
            for (int i = 0; i<4; i++)
                squares.push_back(Point(approx[i].x, approx[i].y));
        }
        approxPolyDP(contours[i], approx, arcLength(Mat(contours[i]), true)*0.1, true);
        if (approx.size() == 3 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
        {
            double minDist = 1e10;

            for (int i = 0; i <3; i++)
            {
                Point side = approx[i] - approx[(i + 1) % 3];
                double squaredSideLength = side.dot(side);
                minDist = min(minDist, squaredSideLength);
            }
            if (minDist<50)
                break;
            // for (int i = 0; i<3; i++)
            //     sanjiao.push_back(Point(approx[i].x, approx[i].y));
        }
        drawContours(dstImg, contours, i, Scalar(0,0,255), 3);
    }
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
    imshow("123", dstImg);
    char key = (char)waitKey(300);
    if (key == 27)
      break;
  }

  return 0;

}