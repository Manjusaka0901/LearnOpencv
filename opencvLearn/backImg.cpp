#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

//灰度化图像
Mat gray_Img(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	cvtColor(src, dst, CV_BGR2GRAY);
	return dst;
}

//自适应阈值(二值化图像)
Mat threshold_Img(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	//参数：输入，       输出，       二值图像的最大值 ，      在一个邻域内计算阈值所采用的算法，有两个取值分别为 ADAPTIVE_THRESH_MEAN_C 和 ADAPTIVE_THRESH_GAUSSIAN_C ，     阈值类型只有两个取值，分别为 THRESH_BINARY 和THRESH_BINARY_INV，（blockSize）adaptiveThreshold的计算单位是像素的邻域块，邻域块取多大就由这个值作决定，    偏移值调整量
	adaptiveThreshold(~src, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
	return dst;
}

//结构元素(获取垂直算子)
Mat get_Vertical(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	return getStructuringElement(MORPH_RECT, Size(src.cols / 16, 1), Point(-1, -1));
}

//结构元素（获取水平算子）
Mat get_Horizontal(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	return getStructuringElement(MORPH_RECT, Size(1, src.rows / 16), Point(-1, -1));
}

//腐蚀
Mat erode_Img(Mat src, Mat kernel)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	erode(src, dst, kernel);
	return dst;
}

//膨胀
Mat dilate_Img(Mat src, Mat kernel)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	dilate(src, dst, kernel);
	return dst;
}


int main()
{
	Mat src = imread("back.png");
	if (src.empty())
	{
		cout << "fail to load image" << endl;
		return -1;
	}

	namedWindow("input_Img", 0);
	imshow("input_Img", src);
	//输入图像转灰度
	Mat grayImg = gray_Img(src);
	namedWindow("input_Img_gray", 0);
	imshow("input_Img_gray", grayImg);
	//二值化图像
	Mat thresholdImg = threshold_Img(grayImg);
	namedWindow("input_Img_threshold", 0);
	imshow("input_Img_threshold", thresholdImg);

	Mat verticalLine = get_Vertical(src);
	Mat horizontalLine = get_Horizontal(src);
	//先腐蚀再膨胀
	Mat vertical_Line_dilate = dilate_Img(thresholdImg, verticalLine);
	Mat vertical_Line_erode = erode_Img(vertical_Line_erode, verticalLine);

	//显示图像中的垂直线
	namedWindow("verticalLine", 0);
	imshow("verticalLine", vertical_Line_erode);

	Mat horizontal_Line_dilate = dilate_Img(thresholdImg , horizontalLine);
	Mat horizontal_Line_erode = erode_Img(horizontal_Line_erode, horizontalLine);

	//显示图像中的垂直线
	namedWindow("horizontalLine", 0);
	imshow("horizontalLine", horizontal_Line_erode);

	waitKey();
	return 0;
}
