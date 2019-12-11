// #include <iostream>
// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"
 
// using namespace cv;
// using namespace std;
 
//  int main( int argc, char** argv )
//  {
//     VideoCapture cap(0); //capture the video from web cam
 
//     if ( !cap.isOpened() )  // if not success, exit program
//     {
//          cout << "Cannot open the web cam" << endl;
//          return -1;
//     }
 
//   namedWindow("Control", cv::WINDOW_AUTOSIZE); //create a window called "Control"
 
//   int iLowH = 100;
//   int iHighH = 140;
 
//   int iLowS = 90; 
//   int iHighS = 255;
 
//   int iLowV = 90;
//   int iHighV = 255;
 
//   //Create trackbars in "Control" window
//   createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
//   createTrackbar("HighH", "Control", &iHighH, 179);
 
//   createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
//   createTrackbar("HighS", "Control", &iHighS, 255);
 
//   createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
//   createTrackbar("HighV", "Control", &iHighV, 255);
 
//     while (true)
//     {
//         Mat imgOriginal;
 
//         bool bSuccess = cap.read(imgOriginal); // read a new frame from video
 
//          if (!bSuccess) //if not success, break loop
//         {
//              cout << "Cannot read a frame from video stream" << endl;
//              break;
//         }
 
//    Mat imgHSV;
//    vector<Mat> hsvSplit;
//    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
//    //因为我们读取的是彩色图，直方图均衡化需要在HSV空间做
//    split(imgHSV, hsvSplit);
//    equalizeHist(hsvSplit[2],hsvSplit[2]);
//    merge(hsvSplit,imgHSV);
//    Mat imgThresholded;
 
//    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
 
//    //开操作 (去除一些噪点)
//    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
//    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
 
//    //闭操作 (连接一些连通域)
//    morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
 
//    imshow("Thresholded Image", imgThresholded); //show the thresholded image
//    imshow("Original", imgOriginal); //show the original image
 
//    char key = (char) waitKey(300);
//    if(key == 27)
//          break;
//     }
 
//    return 0;
//  }


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
//         switch(color){
//     case 'B':
//         minh = 0;
//         maxh = 180;
//         mins = 0;
//         maxs = 255;
//         minv = 0;
//         maxv = 46;
//         break;
//     case 'H':
//         minh = 0;
//         maxh = 180;
//         mins = 0;
//         maxs = 43;
//         minv = 46;
//         maxv = 220;
//         break;
//     case 'W':
//         minh = 0;
//         maxh = 180;
//         mins = 0;
//         maxs = 30;
//         minv = 221;
//         maxv = 255;
//         break;
//     case 'R':
//         minh = 0;
//         maxh = 10;
//         mins = 43;
//         maxs = 255;
//         minv = 46;
//         maxv = 255;
//         break;
//     case 'O':
//         minh = 11;
//         maxh = 25;
//         mins = 43;
//         maxs = 255;
//         minv = 46;
//         maxv = 255;
//         break;
//     case 'Y':
//         minh = 26;
//         maxh = 34;
//         mins = 43;
//         maxs = 255;
//         minv = 46;
//         maxv = 255;
//         break;
//     case 'G':
//         minh = 35;
//         maxh = 77;
//         mins = 43;
//         maxs = 255;
//         minv = 46;
//         maxv = 255;
//         break;
//     case 'L':
//         minh = 100;
//         maxh = 124;
//         mins = 43;
//         maxs = 255;
//         minv = 46;
//         maxv = 255;
//         break;
//     case 'P':
//         minh = 125;
//         maxh = 155;
//         mins = 43;
//         maxs = 255;
//         minv = 46;
//         maxv = 255;
//         break;
//     default:
//         cout << "输入错误" << endl;
//         exit(0);
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
        
//         inRange(fhsv,Scalar(minh,mins,minv),Scalar(maxh,maxs,maxv),special);          //找寻在要求区间内的颜色
           
//     imshow("[pic]",special);
     
//         imshow("Original", frame); //show the original image    
    
//     if(waitKey(30) >= 0)
//       break;
//     }
  
//     return 0;
// }


// #include <iostream>
// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"

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

//   namedWindow("control", 1);
//   int ctrl = 0;
//   createTrackbar("ctrl", "control", &ctrl, 7);

//   while (true)
//   {
//     Mat imgOriginal;

//     bool bSuccess = cap.read(imgOriginal); // read a new frame from video
//     if (!bSuccess) //if not success, break loop
//     {
//       cout << "Cannot read a frame from video stream" << endl;
//       break;
//     }
//     // Mat imgOriginal;
//     // imgOriginal = imread(argv[1]);

//     // Mat imgHSV, imgBGR;
//     // Mat imgThresholded;

//     // if(0)
//     // {
//     //   vector<Mat> hsvSplit;   //创建向量容器，存放HSV的三通道数据
//     //   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
//     //   split(imgHSV, hsvSplit);      //分类原图像的HSV三通道
//     //   equalizeHist(hsvSplit[2], hsvSplit[2]);    //对HSV的亮度通道进行直方图均衡
//     //   merge(hsvSplit, imgHSV);           //合并三种通道
//     //   cvtColor(imgHSV, imgBGR, COLOR_HSV2BGR);    //将HSV空间转回至RGB空间，为接下来的颜色识别做准备
//     // }
//     // else
//     // {
//     //   imgBGR = imgOriginal.clone();
//     // }



//     // switch(ctrl)
//     // {
//     // case 0:
//     //   {
//     //     inRange(imgBGR, Scalar(128, 0, 0), Scalar(255, 127, 127), imgThresholded); //蓝色
//     //     break;
//     //   }
//     // case 1:
//     //   {
//     //     inRange(imgBGR, Scalar(128, 128, 128), Scalar(255, 255, 255), imgThresholded); //白色
//     //     break;
//     //   }
//     // case 2:
//     //   {
//     //     inRange(imgBGR, Scalar(128, 128, 0), Scalar(255, 255, 127), imgThresholded); //靛色
//     //     cout<<double(cvMax(imgOriginal))<<"!!!!!!"<<endl;
//     //     break;
//     //   }
//     // case 3:
//     //   {
//     //     inRange(imgBGR, Scalar(128, 0, 128), Scalar(255, 127, 255), imgThresholded); //紫色
//     //     break;
//     //   }
//     // case 4:
//     //   {
//     //     inRange(imgBGR, Scalar(0, 128, 128), Scalar(127, 255, 255), imgThresholded); //黄色
//     //     break;
//     //   }
//     // case 5:
//     //   {
//     //     inRange(imgBGR, Scalar(0, 128, 0), Scalar(127, 255, 127), imgThresholded); //绿色
//     //     break;
//     //   }
//     // case 6:
//     //   {
//     //     inRange(imgBGR, Scalar(0, 0, 128), Scalar(127, 127, 255), imgThresholded); //红色
//     //     break;
//     //   }
//     // case 7:
//     //   {
//     //     inRange(imgBGR, Scalar(0, 0, 0), Scalar(127, 127, 127), imgThresholded); //黑色
//     //     break;
//     //   }
//     // }
                                    
//     // imshow("形态学去噪声前", imgThresholded);

//     // Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
//     // morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
//     // morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

//     // imshow("Thresholded Image", imgThresholded); //show the thresholded image
//     // imshow("直方图均衡以后", imgBGR);
//     imshow("Original", imgOriginal); //show the original image

//     char key = (char)waitKey(300);
//     if (key == 27)
//       break;
//   }

//   return 0;

// }

// #include <opencv2/opencv.hpp>
// #include <iostream>
 
// using namespace cv;
// using namespace std;
 
// int main(int argc, const char *argv[])
// {

//   Mat src = imread(argv[1], IMREAD_GRAYSCALE);
//   if (src.empty()) {
//     printf("不能加载图片！\n");
//     return -1;
//   }
//   namedWindow("1--原图", WINDOW_AUTOSIZE);
//   imshow("1--原图", src);
 
//   double minVal, maxVal; 
//   Point minLoc, maxLoc;
//   //查找图像中最小值最大值及它们的位置
//   minMaxLoc(src, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
 
//   printf("min: %.2f, max: %.2f \n", minVal, maxVal);
//   printf("min location: (%d, %d) \n", minLoc.x, minLoc.y);
//   printf("max location: (%d, %d) \n", maxLoc.x, maxLoc.y);
  
//   src = imread(argv[1]);
//   Mat means, stddev;
//   //计算：彩色图像三通道的均值与方差
//   meanStdDev(src, means, stddev);
//   printf("Channel--[Blue]  mean: %.2f, stddev: %.2f\n", means.at<double>(0, 0), stddev.at<double>(0, 0));
//   printf("Channel--[Green] mean: %.2f, stddev: %.2f\n", means.at<double>(1, 0), stddev.at<double>(1, 0));
//   printf("Channel--[Red]   mean: %.2f, stddev: %.2f\n", means.at<double>(2, 0), stddev.at<double>(2, 0));
 
//   waitKey(0);
//   return 0;
// }



#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

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
  printf("Channel--[Blue]  mean: %.2f, stddev: %.2f\n", means.at<double>(0, 0), stddev.at<double>(0, 0));
  printf("Channel--[Green] mean: %.2f, stddev: %.2f\n", means.at<double>(1, 0), stddev.at<double>(1, 0));
  printf("Channel--[Red]   mean: %.2f, stddev: %.2f\n", means.at<double>(2, 0), stddev.at<double>(2, 0));

    char key = (char)waitKey(300);
    if (key == 27)
      break;
  }

  return 0;

}