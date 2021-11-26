#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
string winName = "threshould";
Mat src, gray;

int main(int artc, char** argv) {
    src = imread("./dst_1.png", IMREAD_COLOR);
    Mat dst = src.clone();
    if (src.empty()) {
        cout << "can't find picture" << endl;
        return -1;
    }
    Mat blur;
    GaussianBlur(src, blur, Size(3, 3), 0, 0);
    cvtColor(blur, gray, COLOR_BGR2GRAY);
    namedWindow(winName, WINDOW_NORMAL);
    
    gray = gray > 130;
    Mat kernal = getStructuringElement(MORPH_RECT, Size(12, 12));
    Mat img_dilate, img_erosion;
    Mat dstImage = Mat::zeros(src.size(), CV_8U);

    dilate(gray, img_dilate, kernal);
    // imshow("dilate", img_dilate);

    vector<vector<Point> > contours;    // 存储轮廓点
    vector<Vec4i> hierarchy;
    findContours(img_dilate, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<Rect> panelsRect;
    // drawContours(src, contours, -1, Scalar(255, 0, 255), 3);
    // 画出符合条件的轮廓，并生成矩形
    for (int i = 0; i < contours.size(); i++) {
        if (arcLength(contours[i], true) >= 450 && arcLength(contours[i], true) <= 500) {
            drawContours(src, contours, i, Scalar(255, 0, 255), 2);
            Rect boundRect = boundingRect(Mat(contours[i]));
            rectangle(src, boundRect.tl(), boundRect.br(), Scalar(0, 255, 0), 2);
            panelsRect.push_back(boundRect);
        }
    }
    cout << panelsRect.size() << endl;
    imwrite("./panels.png", src);
    // 提取几个小块的光伏板
    Mat panel;
    for (int i = 0; i < 2; i++) {
        dst(panelsRect[i]).copyTo(panel);
        cout << "(" << panel.rows << ", " << panel.cols << ")" << endl;
        imshow("panel_" + to_string(i), panel);
    }
    imshow(winName, src);
    waitKey(0);
	return 0;
}