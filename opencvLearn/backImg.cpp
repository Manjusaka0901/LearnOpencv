#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

//�ҶȻ�ͼ��
Mat gray_Img(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	cvtColor(src, dst, CV_BGR2GRAY);
	return dst;
}

//����Ӧ��ֵ(��ֵ��ͼ��)
Mat threshold_Img(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	/*���������룬       
		   �����       
		   ��ֵͼ������ֵ ��      
		   ��һ�������ڼ�����ֵ�����õ��㷨��������ȡֵ�ֱ�Ϊ ADAPTIVE_THRESH_MEAN_C �� ADAPTIVE_THRESH_GAUSSIAN_C ��     
		   ��ֵ����ֻ������ȡֵ���ֱ�Ϊ THRESH_BINARY ��THRESH_BINARY_INV����blockSize��adaptiveThreshold�ļ��㵥λ�����ص�����飬�����ȡ���������ֵ��������    
		   ƫ��ֵ������
		   */
	adaptiveThreshold(~src, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
	return dst;
}

//�ṹԪ��(��ȡ��ֱ����)
Mat get_Vertical(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	return getStructuringElement(MORPH_RECT, Size(src.cols / 16, 1), Point(-1, -1));
}

//�ṹԪ�أ���ȡˮƽ���ӣ�
Mat get_Horizontal(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	return getStructuringElement(MORPH_RECT, Size(1, src.rows / 16), Point(-1, -1));
}

//��ʴ
Mat erode_Img(Mat src, Mat kernel)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	erode(src, dst, kernel);
	return dst;
}

//����
Mat dilate_Img(Mat src, Mat kernel)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	dilate(src, dst, kernel);
	return dst;
}

Mat canny_Img(Mat src)
{
	Mat dst = Mat::zeros(src.size(), src.type());
	Canny(src, dst, 50, 150, 3);
	return dst;
}

int mainHough()
{
	Mat src = imread("back.png");
	if (src.empty())
	{
		cout << "fail to load image" << endl;
		return -1;
	}

	namedWindow("input_Img", 0);
	imshow("input_Img", src);
	//��Ե���
	Mat cannyImg = canny_Img(src);
	//����ͼ��ת�Ҷ�
	Mat dst;// = gray_Img(cannyImg);
	cvtColor(cannyImg, dst, CV_GRAY2BGR);
	namedWindow("input_Img_gray", 0);
	imshow("input_Img_gray", cannyImg);
	//�����߱任
	vector<Vec2f> lines;
	HoughLines(cannyImg, lines, 0.1, CV_PI / 360, 10, 0, 0);
	/*����*/
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][0];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(dst, pt1, pt2, Scalar(55, 100, 195), 1, CV_AA);
	}

	imshow("dst", dst);
	waitKey();
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
	//����ͼ��ת�Ҷ�
	Mat grayImg = gray_Img(src);
	imshow("input_Img_gray", grayImg);
	
	//��ֵ��ͼ��
	Mat thresholdImg = threshold_Img(grayImg);
	namedWindow("input_Img_threshold", 0);
	imshow("input_Img_threshold", thresholdImg);

	Mat verticalLine = get_Vertical(src);
	Mat horizontalLine = get_Horizontal(src);
	//�ȸ�ʴ������
	Mat vertical_Line_erode = erode_Img(thresholdImg, verticalLine);
	imshow("horizontalLine", vertical_Line_erode);
	//Mat vertical_Line_dilate = dilate_Img(vertical_Line_erode, verticalLine);
	////��ʾͼ���е�ˮƽ��
	//namedWindow("verticalLine", 0);
	//imshow("verticalLine", vertical_Line_dilate);

	//Mat horizontal_Line_erode = erode_Img(thresholdImg, horizontalLine);
	//Mat horizontal_Line_dilate = dilate_Img(horizontal_Line_erode, horizontalLine);
	////��ʾͼ���еĴ�ֱ��
	//namedWindow("horizontalLine", 0);
	//imshow("horizontalLine", horizontal_Line_dilate);

	waitKey();
	return 0;
}
