// #include <iostream>
// #include<opencv2/opencv.hpp>
// #include "opencv2/core/core.hpp"
// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"
// #include "opencv2/features2d/features2d.hpp"
// #include<vector>
// using namespace cv;
// using namespace std;

// string Convert(float Num)
// {
//   ostringstream oss;
//   oss<<Num;
//   string str(oss.str());
//   return str;
// }

// float cvMax(cv::Mat& mat) 
// { 
// float max=0; 
// float* pData=(float*)mat.data; 
// for(int i=0;i<mat.rows;i++) 
// { 
//     for(int j=0;j<mat.cols;j++) 
//     { 
//      float value = pData[j+i*mat.cols]; 
//      if(value>max) 
//      {
//       cout<<max<<endl;
//       max=value; 
//      } 
//     } 
// } 
// return max; 
// } 

// int main(int argc, char** argv)
// {
//   VideoCapture cap(0); //capture the video from web cam
  
//   if (!cap.isOpened())  // if not success, exit program
//   {
//     cout << "Cannot open the web cam" << endl;
//     return -1;
//   }

//   while (true)
//   {
//     Mat imgOriginal;

//     bool bSuccess = cap.read(imgOriginal); // read a new frame from video
//     if (!bSuccess) //if not success, break loop
//     {
//       cout << "Cannot read a frame from video stream" << endl;
//       break;
//     }

//     imshow("Original", imgOriginal); //show the original image
//     Mat means, stddev;
// //   //计算：彩色图像三通道的均值与方差
//     meanStdDev(imgOriginal, means, stddev);
//     // printf("Channel--[Blue]  mean: %.2f, stddev: %.2f\n", means.at<double>(0, 0), stddev.at<double>(0, 0));
//     // printf("Channel--[Green] mean: %.2f, stddev: %.2f\n", means.at<double>(1, 0), stddev.at<double>(1, 0));
//     // printf("Channel--[Red]   mean: %.2f, stddev: %.2f\n", means.at<double>(2, 0), stddev.at<double>(2, 0));

//     Mat gray,tempImg;
//     resize(imgOriginal, tempImg, Size(imgOriginal.cols / 2, imgOriginal.rows / 2), 0, 0);
//     cvtColor(tempImg, gray, COLOR_RGB2GRAY);
//     medianBlur(gray, gray, 3);

//     vector<Vec3f>circles;//圆
//     vector<Point>approx;
//     HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, imgOriginal.rows / 20, 100, 60, 0, 0);
//     vector<Point>squares;
//     vector<vector<Point>>contours;
//     Mat srcImg1;
//     Mat thresh;
//     threshold(gray, thresh, 0, 255, THRESH_OTSU);
//     srcImg1 = thresh.clone();
//     findContours(srcImg1, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
//     Mat dstImg(srcImg1.rows, srcImg1.cols, CV_8UC3, Scalar(255, 255, 255));
//     for (size_t i = 0; i < circles.size(); i++)//画圆
//     {
//         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//         int radius = cvRound(circles[i][2]);
//         circle(dstImg, center, radius, Scalar(0, 255, 0), 5, 8, 0);
//         circle(dstImg, center, 3, Scalar(0, 255, 0), -1);
//         cout << "圆心" << i + 1 << center << endl;
//     }

//         for (size_t i = 0; i < contours.size(); i++)
//     {
//         approxPolyDP(contours[i], approx, arcLength(Mat(contours[i]), true)*0.02, true);
//         if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
//         {
//             double minDist = 1e10;

//             for (int i = 0; i < 4; i++)
//             {
//                 Point side = approx[i] - approx[(i + 1) % 4];
//                 double squaredSideLength = side.dot(side);
//                 minDist = min(minDist, squaredSideLength);
//             }
//             if (minDist<50)
//                 break;
//             for (int i = 0; i<4; i++)
//                 squares.push_back(Point(approx[i].x, approx[i].y));
//         }
//         approxPolyDP(contours[i], approx, arcLength(Mat(contours[i]), true)*0.1, true);
//         if (approx.size() == 3 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
//         {
//             double minDist = 1e10;

//             for (int i = 0; i <3; i++)
//             {
//                 Point side = approx[i] - approx[(i + 1) % 3];
//                 double squaredSideLength = side.dot(side);
//                 minDist = min(minDist, squaredSideLength);
//             }
//             if (minDist<50)
//                 break;
//             // for (int i = 0; i<3; i++)
//             //     sanjiao.push_back(Point(approx[i].x, approx[i].y));
//         }
//         drawContours(dstImg, contours, i, Scalar(0,0,255), 3);
//     }
//     for (size_t i = 0; i < squares.size(); i += 4)
//     {
//         Point center;
//         center.x = (squares[i].x + squares[i + 2].x) / 2;
//         center.y = (squares[i].y + squares[i + 2].y) / 2;
//         line(dstImg, squares[i], squares[i + 1], Scalar(0, 0, 255), 4);
//         line(dstImg, squares[i + 1], squares[i + 2], Scalar(0, 0, 255), 4);
//         line(dstImg, squares[i + 2], squares[i + 3], Scalar(0, 0, 255), 4);
//         line(dstImg, squares[i + 3], squares[i], Scalar(0, 0, 255), 4);
//         cout << "矩形中心" << (i + 1) % 4 << center << endl;
//         circle(dstImg, center, 3, Scalar(0, 0, 255), -1);
//     }
//     imshow("123", dstImg);
//     char key = (char)waitKey(300);
//     if (key == 27)
//       break;
//   }

//   return 0;

// }



#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <string.h>
#include <unistd.h>
using namespace cv;
using namespace std;

char input_image[] = "input image";
int line_size = 2;
string text ="";
void draw(Mat &reslutImage,Mat &dst)
{
    Point cc;
    vector <vector<Point>> contours;
    vector<Vec4i> hireachy;
    vector<Point>approx;
    findContours(dst, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    for (size_t t = 0; t < contours.size(); t++){
        vector<Point>squares;
        vector<Point>sanjiao;
        approxPolyDP(contours[t], approx, arcLength(Mat(contours[t]), true)*0.1, true);
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
            for (int i = 0; i<3; i++)
                sanjiao.push_back(Point(approx[i].x, approx[i].y));
        }
        for (size_t i = 0; i < sanjiao.size(); i += 3)
        {
            Point center;
            center.x = (sanjiao[i].x + sanjiao[i + 1].x + sanjiao[i + 2].x) / 3;
            center.y = (sanjiao[i].y + sanjiao[i + 1].y + sanjiao[i + 2].y) / 3;
            line(reslutImage, sanjiao[i], sanjiao[i + 1], Scalar(255, 0, 0), line_size);
            line(reslutImage, sanjiao[i + 1], sanjiao[i + 2], Scalar(255, 0, 0), line_size);
            line(reslutImage, sanjiao[i], sanjiao[i + 2], Scalar(255, 0, 0), line_size);
            cout << "三角形中心" << (i + 1) % 3 << center << endl;
            text += "triangle" + to_string((i + 1) % 3) + "["+  to_string((int)center.x) + ","+ to_string((int)center.y) +"]" + ";";
            circle(reslutImage, center, line_size, Scalar(255, 0, 0), -1);
        }

        approxPolyDP(contours[t], approx, arcLength(Mat(contours[t]), true)*0.02, true);
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
        for (size_t i = 0; i < squares.size(); i += 4)
        {
            Point center;
            center.x = (squares[i].x + squares[i + 2].x) / 2;
            center.y = (squares[i].y + squares[i + 2].y) / 2;
            line(reslutImage, squares[i], squares[i + 1], Scalar(0, 0, 255), line_size);
            line(reslutImage, squares[i + 1], squares[i + 2], Scalar(0, 0, 255), line_size);
            line(reslutImage, squares[i + 2], squares[i + 3], Scalar(0, 0, 255), line_size);
            line(reslutImage, squares[i + 3], squares[i], Scalar(0, 0, 255), line_size);
            cout << "矩形中心" << (i + 1) % 4 << center << endl;
            text += "squares" + to_string((int)(i + 1) % 4) + "["+  to_string((int)center.x) + ","+ to_string((int)center.y) +"]" + ";";
            circle(reslutImage, center, line_size, Scalar(0, 0, 255), -1);
        }

        double area = contourArea(contours[t]);
        if (area < 10000) continue;
        // 通过宽高比进行过滤
        Rect rect = boundingRect(contours[t]);
        float ratio = float(rect.width) / float(rect.height);

        if (ratio<1.1&&ratio>0.9){
            drawContours(reslutImage, contours, t, Scalar(0, 0, 255), line_size, 8, Mat(), 0, Point());
            
            printf("circle area : %f \n", area);
            printf("circle length: %f \n", arcLength(contours[t], true));
            // text += "triangle" + to_string((int)area) + "["+  to_string((int)arcLength(contours[t], true)) +"]" + ";";
            int x = rect.x + rect.width / 2;
            int y = rect.y + rect.height / 2;
            cc = Point(x, y);
            circle(reslutImage, cc, line_size, Scalar(0, 0, 255), 2, 8, 0);
            // cout<<rect.width<<" "<<rect.height<<" "<<area<<endl;
            // imshow(to_string(t),reslutImage);
        }
    }
}
int main(int argc, char ** argv)
{
    VideoCapture cap(0); //capture the video from web cam
  
    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    vector<Scalar> color1,color2;
    color1.push_back(Scalar(128, 0, 0));
    color1.push_back(Scalar(0, 128, 0));
    color1.push_back(Scalar(0, 0, 128));
    color1.push_back(Scalar(128, 128, 128));
    color2.push_back(Scalar(255, 127, 127));
    color2.push_back(Scalar(127, 255, 127));
    color2.push_back(Scalar(127, 127, 255));
    color2.push_back(Scalar(255, 255, 255));
    while (true){
        Mat src, dst, gray_src,reslutImage;
        bool bSuccess = cap.read(src); // read a new frame from video
        if (!bSuccess) //if not success, break loop
        {
          cout << "Cannot read a frame from video stream" << endl;
          break;
        }
        // src = imread(argv[1]);
        // if (src.empty()){
        //     printf("colud not load image ..\n");
        //     return -1;
        // }
        reslutImage = src.clone();
        namedWindow(input_image, cv::WINDOW_AUTOSIZE);
        imshow(input_image, src);
        for(int i = 0;i < (int)color1.size()&&i < (int)color2.size();i++){
            // 二值化
            inRange(src, color1[i], color2[i], gray_src);
            threshold(gray_src, gray_src, 0, 255, THRESH_OTSU | THRESH_BINARY);

            // 闭操作连接黑点
            Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
            morphologyEx(gray_src, dst, MORPH_CLOSE, kernel, Point(-1, -1));

            // 闭操作去毛点
            kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
            morphologyEx(dst, dst, MORPH_OPEN, kernel, Point(-1, -1));
            // imshow(to_string(i),dst);
            draw(reslutImage,dst);
        }
        putText(reslutImage,text, Point(20, 50), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255), 1, 2);
        imshow("Reslut", reslutImage);
        
        // sleep(5);
        char key = (char)waitKey(300);
        if (key == 27)
            break;
    }


    // Mat src, dst, gray_src,reslutImage;
    // src = imread(argv[1]);
    // if (src.empty()){
    //     printf("colud not load image ..\n");
    //     return -1;
    // }
    // reslutImage = src.clone();
    // namedWindow(input_image, cv::WINDOW_AUTOSIZE);
    // imshow(input_image, src);
    // vector<Scalar> color1,color2;
    // color1.push_back(Scalar(128, 0, 0));
    // color1.push_back(Scalar(0, 128, 0));
    // color1.push_back(Scalar(0, 0, 128));
    // color1.push_back(Scalar(128, 128, 128));
    // color2.push_back(Scalar(255, 127, 127));
    // color2.push_back(Scalar(127, 255, 127));
    // color2.push_back(Scalar(127, 127, 255));
    // color2.push_back(Scalar(255, 255, 255));
    // for(int i = 0;i < (int)color1.size()&&i < (int)color2.size();i++){
    //     // 二值化
    //     inRange(src, color1[i], color2[i], gray_src);
    //     threshold(gray_src, gray_src, 0, 255, THRESH_OTSU | THRESH_BINARY);

    //     // 闭操作连接黑点
    //     Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
    //     morphologyEx(gray_src, dst, MORPH_CLOSE, kernel, Point(-1, -1));

    //     // 闭操作去毛点
    //     kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
    //     morphologyEx(dst, dst, MORPH_OPEN, kernel, Point(-1, -1));
    //     imshow(to_string(i),dst);
    //     draw(reslutImage,dst);
    // }
    // putText(reslutImage,text, Point(20, 50), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255), 1, 2);
    // imshow("Reslut", reslutImage);
    // waitKey(0);
    return 0;
}