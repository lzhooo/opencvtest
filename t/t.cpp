#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

char input_image[] = "input image";
int line_size = 2;
string text ="";
void draw(Mat &reslutImage,Mat &dst,Scalar color)
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
            line(reslutImage, sanjiao[i], sanjiao[i + 1], color, line_size);
            line(reslutImage, sanjiao[i + 1], sanjiao[i + 2], color, line_size);
            line(reslutImage, sanjiao[i], sanjiao[i + 2], color, line_size);
            cout << "三角形中心" << (i + 1) % 3 << center << endl;
            text += "triangle" + to_string((i + 1) % 3) + "["+  to_string((int)center.x) + ","+ to_string((int)center.y) +"]" + ";";
            circle(reslutImage, center, line_size, color, -1);
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
            line(reslutImage, squares[i], squares[i + 1], color, line_size);
            line(reslutImage, squares[i + 1], squares[i + 2], color, line_size);
            line(reslutImage, squares[i + 2], squares[i + 3], color, line_size);
            line(reslutImage, squares[i + 3], squares[i], color, line_size);
            cout << "矩形中心" << (i + 1) % 4 << center << endl;
            text += "squares" + to_string((int)(i + 1) % 4) + "["+  to_string((int)center.x) + ","+ to_string((int)center.y) +"]" + ";";
            circle(reslutImage, center, line_size, color, -1);
        }

        double area = contourArea(contours[t]);
        if (area < 10000) continue;
        // 通过宽高比进行过滤
        Rect rect = boundingRect(contours[t]);
        float ratio = float(rect.width) / float(rect.height);

        if (ratio<1.1&&ratio>0.9){
            drawContours(reslutImage, contours, t, color, line_size, 8, Mat(), 0, Point());
            
            printf("circle area : %f \n", area);
            printf("circle length: %f \n", arcLength(contours[t], true));
            // text += "triangle" + to_string((int)area) + "["+  to_string((int)arcLength(contours[t], true)) +"]" + ";";
            int x = rect.x + rect.width / 2;
            int y = rect.y + rect.height / 2;
            cc = Point(x, y);
            circle(reslutImage, cc, line_size, color, 2, 8, 0);
            // cout<<rect.width<<" "<<rect.height<<" "<<area<<endl;
            // imshow(to_string(t),reslutImage);
        }
    }
}
void say_hello(int, void *){
    cout<<"hello";
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
    int g_nAlphaValueSlider = 0;
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
        createTrackbar("confirm",input_image,&g_nAlphaValueSlider,1);
        if(g_nAlphaValueSlider == 1){

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
                draw(reslutImage,dst,color2[i]);
            }
            putText(reslutImage,text, Point(20, 50), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255), 1, 2);
            imshow("Reslut", reslutImage);
        }
        g_nAlphaValueSlider = 0;
        text="";
        char key = (char)waitKey(300);
        if (key == 27)
            break;
    }
    return 0;
}