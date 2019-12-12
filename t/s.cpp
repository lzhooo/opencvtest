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
    Mat means, stddev;
    meanStdDev(mat, means, stddev);
    printf("Channel--[Blue]  mean: %.2f, stddev: %.2f\n", means.at<double>(0, 0), stddev.at<double>(0, 0));
    printf("Channel--[Green] mean: %.2f, stddev: %.2f\n", means.at<double>(1, 0), stddev.at<double>(1, 0));
    printf("Channel--[Red]   mean: %.2f, stddev: %.2f\n", means.at<double>(2, 0), stddev.at<double>(2, 0));
    return 0;

} 
void check(cv::Mat gray)
{
    // Mat tempImg;
    // tempImg = gray.clone();
    // resize(imgOriginal, tempImg, Size(imgOriginal.cols / 2, imgOriginal.rows / 2), 0, 0);
    // cvtColor(tempImg, gray, cv::COLOR_RGB2GRAY);
    medianBlur(gray, gray, 3);
    imshow("123", gray);
    vector<Vec3f>circles;//圆
    vector<Point>approx;
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, gray.rows / 20, 100, 60, 0, 0);
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
        // approxPolyDP(contours[i], approx, arcLength(Mat(contours[i]), true)*0.1, true);
        // if (approx.size() == 3 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx)))
        // {
        //     double minDist = 1e10;

        //     for (int i = 0; i <3; i++)
        //     {
        //         Point side = approx[i] - approx[(i + 1) % 3];
        //         double squaredSideLength = side.dot(side);
        //         minDist = min(minDist, squaredSideLength);
        //     }
        //     if (minDist<50)
        //         break;
        //     // for (int i = 0; i<3; i++)
        //     //     sanjiao.push_back(Point(approx[i].x, approx[i].y));
        // }
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
}

int main(int argc, char** argv)
{
  // VideoCapture cap(0); //capture the video from web cam
  
  // if (!cap.isOpened())  // if not success, exit program
  // {
  //   cout << "Cannot open the web cam" << endl;
  //   return -1;
  // }

  namedWindow("control", 1);
  int ctrl = 0;
  createTrackbar("ctrl", "control", &ctrl, 7);
  int flag = -1;
  while (true)
  {
    Mat imgOriginal;

    // bool bSuccess = cap.read(imgOriginal); // read a new frame from video
    // if (!bSuccess) //if not success, break loop
    // {
    //   cout << "Cannot read a frame from video stream" << endl;
    //   break;
    // }
    imgOriginal = imread(argv[1]);
    imshow("Original", imgOriginal);
    Mat imgHSV, imgBGR;
    Mat imgThresholded;

    if(0)
    {
      vector<Mat> hsvSplit;   //创建向量容器，存放HSV的三通道数据
      cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
      split(imgHSV, hsvSplit);      //分类原图像的HSV三通道
      equalizeHist(hsvSplit[2], hsvSplit[2]);    //对HSV的亮度通道进行直方图均衡
      merge(hsvSplit, imgHSV);           //合并三种通道
      cvtColor(imgHSV, imgBGR, COLOR_HSV2BGR);    //将HSV空间转回至RGB空间，为接下来的颜色识别做准备
    }
    else
    {
      imgBGR = imgOriginal.clone();
    }


    Mat test;
    int flagg = -1;
    switch(ctrl)
    {
        case 0:
          {
            inRange(imgBGR, Scalar(128, 0, 0), Scalar(255, 127, 127), imgThresholded); //蓝色
            flagg = 0;
            break;
          }
        case 1:
          {
            inRange(imgBGR, Scalar(128, 128, 128), Scalar(255, 255, 255), imgThresholded); //白色
            flagg = 1;
            break;
          }
        case 2:
          {
            inRange(imgBGR, Scalar(128, 128, 0), Scalar(255, 255, 127), imgThresholded); //靛色
            flagg = 2;
            break;
          }
        case 3:
          {
            inRange(imgBGR, Scalar(128, 0, 128), Scalar(255, 127, 255), imgThresholded); //紫色
            flagg = 3;
            break;
          }
        case 4:
          {
            inRange(imgBGR, Scalar(0, 128, 128), Scalar(127, 255, 255), imgThresholded); //黄色
            flagg = 4;
            break;
          }
        case 5:
          {
            inRange(imgBGR, Scalar(0, 128, 0), Scalar(127, 255, 127), imgThresholded); //绿色
            flagg = 5;
            break;
          }
        case 6:
          {
            inRange(imgBGR, Scalar(0, 0, 128), Scalar(127, 127, 255), imgThresholded); //红色
            flagg = 6;
            break;
          }
        case 7:
          {
            inRange(imgBGR, Scalar(0, 0, 0), Scalar(127, 127, 127), imgThresholded); //黑色
            flagg = 7;
            break;
          }
    }
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
    morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
    if(true){
        Mat gray;
        // cvtColor(imgThresholded, gray, COLOR_BGR2HSV);
        check(imgThresholded);
    }
    flag = flagg;                           
    imshow("形态学去噪声前", imgThresholded);
    
    

    // imshow("Thresholded Image", imgThresholded); //show the thresholded image
    
    // imshow("Original", imgOriginal); //show the original image

    char key = (char)waitKey(300);
    if (key == 27)
      break;
  }

  return 0;

}


// #include <opencv2/opencv.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// #include <iostream>
 
// using namespace std;
// using namespace cv;
 
// int minh,maxh,mins,maxs,minv,maxv;
// void helptext()
// {
//     cout << "B——黑色\n";
//     cout << "H——灰色\n";
//     cout << "W——白色\n";
//     cout << "R——红色\n";
//     cout << "O——橙色\n";
//     cout << "Y——黄色\n";
//     cout << "G——绿色\n";
//     cout << "L——蓝色\n";
//     cout << "P——紫色\n";
//     cout << "输入要求识别的颜色对应的字母:" ;
// }
 
// //各种颜色HSV数值设定
// void deal(char color)
// {
//     switch(color)
//     {
//         case 'B':
//             minh = 0;
//             maxh = 180;
//             mins = 0;
//             maxs = 255;
//             minv = 0;
//             maxv = 46;
//             break;
//         case 'H':
//             minh = 0;
//             maxh = 180;
//             mins = 0;
//             maxs = 43;
//             minv = 46;
//             maxv = 220;
//             break;
//         case 'W':
//             minh = 0;
//             maxh = 180;
//             mins = 0;
//             maxs = 30;
//             minv = 221;
//             maxv = 255;
//             break;
//         case 'R':
//             minh = 0;
//             maxh = 10;
//             mins = 43;
//             maxs = 255;
//             minv = 46;
//             maxv = 255;
//             break;
//         case 'O':
//             minh = 11;
//             maxh = 25;
//             mins = 43;
//             maxs = 255;
//             minv = 46;
//             maxv = 255;
//             break;
//         case 'Y':
//             minh = 26;
//             maxh = 34;
//             mins = 43;
//             maxs = 255;
//             minv = 46;
//             maxv = 255;
//             break;
//         case 'G':
//             minh = 35;
//             maxh = 77;
//             mins = 43;
//             maxs = 255;
//             minv = 46;
//             maxv = 255;
//             break;
//         case 'L':
//             minh = 100;
//             maxh = 124;
//             mins = 43;
//             maxs = 255;
//             minv = 46;
//             maxv = 255;
//             break;
//         case 'P':
//             minh = 125;
//             maxh = 155;
//             mins = 43;
//             maxs = 255;
//             minv = 46;
//             maxv = 255;
//             break;
//         default:
//             cout << "输入错误" << endl;
//             exit(0);
//     }
// }
 
// int main(   )
// {
//     VideoCapture cap(0); //调用摄像头，0为计算机摄像头，1为外接USB摄像头
               
//     Mat special;
    
//     helptext();
 
//     char color;
//     cin >> color;
//     deal(color);
 
 
//     while(true){
          
//   Mat frame;              //存储每一帧的图像
 
//         cap >> frame;        //读取当前帧
 
//         Mat fhsv;
        
//     cvtColor(frame,fhsv,COLOR_BGR2HSV);   //将图像转换为HSV模型
        
//     inRange(fhsv,Scalar(minh,mins,minv),Scalar(maxh,maxs,maxv),special);          //找寻在要求区间内的颜色
    
//     Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
//     morphologyEx(special, special, MORPH_OPEN, element);
//     morphologyEx(special, special, MORPH_CLOSE, element);

//     imshow("[pic]",special);
     
//     imshow("Original", frame); //show the original image    
    
//     if(waitKey(30) >= 0)
//       break;
//     }
  
//     return 0;
// }

