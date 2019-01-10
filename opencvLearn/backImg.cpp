#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <fstream>

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
	return 0;
}

int main01()
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

void main02()
{
	Mat src = imread("000_1_0.png");//ͼƬ·��/*image180.jpg*/
	Mat src1 = imread("000_1_0.png",-1);//ͼƬ·��/*image180.jpg*/

	Mat gray_image;

	cvtColor(src, gray_image, CV_BGR2GRAY);

	Mat binary_image;

	adaptiveThreshold(gray_image, binary_image, 254, CV_ADAPTIVE_THRESH_MEAN_C,

		CV_THRESH_BINARY, 25, 10);  ///�ֲ�����Ӧ��ֵ������

	imwrite("erzhi.png", binary_image);

	//ȥ��

	Mat de_noise = binary_image.clone();

	//��ֵ�˲�

	medianBlur(binary_image, de_noise, 5);

	/////////////////////////   ����  ////////////////////

	Mat dilate_img;

	Mat element = getStructuringElement(MORPH_RECT, Size(20, 20/*15, 15*/));

	dilate(de_noise, dilate_img, element);

	imwrite("dilate.png", dilate_img);

	//�ⲿ�ӿ�

	//�����ͨ��ÿһ����ͨ����һϵ�еĵ��ʾ��FindContours����ֻ�ܵõ���һ����

	vector<vector<Point>> contours;

	vector<Vec4i> hierarchy;

	findContours(dilate_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNALֻ����ⲿ�������ɸ�������������е���



	Mat contoursImage(dilate_img.rows, dilate_img.cols, CV_8U, Scalar(255));

	int index = 0;

	for (; index >= 0; index = hierarchy[index][0]) {

		cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);

		// for opencv 2

		// cv::drawContours(dstImage, contours, index, color,  CV_FILLED, 8, hierarchy);//CV_FILLED����λ�ñ�ʾ����������ϸ�ȣ����Ϊ��ֵ����thickness==cv_filled���������������ڲ�

		// for opencv 3

		//cv::drawContours(contoursImage, contours, index, color, cv::FILLED, 8, hierarchy);



		cv::drawContours(contoursImage, contours, index, Scalar(0), 1, 8, hierarchy);//����ַ���������

		Rect rect = boundingRect(contours[index]);//���������

		rectangle(src, rect, Scalar(0, 0, 255), 3);//���������Ӿ��ο�

	}

	imwrite("zt.png", src1);
	cout << "��ɼ��";
	de_noise.release();

	element.release();

	dilate_img.release();

	binary_image.release();

	gray_image.release();

}

int main03()
{

		Mat original = imread("000_1_0.png",-1);

//		namedWindow("My original");

//		imshow("My original", original);

		Mat gray = original;

		cv::cvtColor(gray, gray, CV_RGBA2GRAY);//�ҶȻ�



		int thresh_size = (100 / 4) * 2 + 1; //����Ӧ��ֵ����ֵ

		adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, thresh_size, thresh_size / 3);

		morphologyEx(gray, gray, MORPH_OPEN, Mat());//��̬ѧ������ȥ���



		vector<vector<Point> > contours;

		cv::findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //������

		vector<vector<Point>> contours1;

		for (int i = 0; i < contours.size(); ++i)

		{

			if (contours[i].size() > 200)//���Ƚ�С�������޳���

			{

				contours1.push_back(contours[i]);

			}

		}

		Mat hole(gray.size(), CV_8U, Scalar(0)); //����ͼ��

		cv::drawContours(hole, contours1, -1, Scalar(255), CV_FILLED); //������ͼ���ϣ��ð�ɫ�����������

//		namedWindow("My hole");
		imwrite("hole.png", hole);

		Mat crop(original.rows, original.cols, CV_8UC3);

		original.copyTo(crop, hole);//��ԭͼ�񿽱�������ͼ��
		imwrite("crop.png",crop);

//		namedWindow("My warpPerspective");

//		imshow("My warpPerspective", crop);

		waitKey(0);
	return 0;
}



Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void*);
void fun(vector<vector<Point> > contours);

/** @function main */
int main04(int argc, char** argv)
{
	/// ����Դͼ��
	src = imread("H842K0006_001_2_0.png", 1);

	/// ת�ɻҶȲ�ģ��������
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	/// ��������
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_NORMAL);
	imshow(source_window, src);
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// ��Canny���Ӽ���Ե
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Ѱ������
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// �������
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, 0, 0, Point());
	}
	// �ڴ�������ʾ���
	namedWindow("Contours", CV_WINDOW_NORMAL);
	imshow("Contours", drawing);

	//createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
	//thresh_callback(0, 0);

	waitKey(0);
	return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// ��Canny���Ӽ���Ե
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Ѱ������
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// �������
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	//int len = contours[0].size();
	//int i;
	//int index = 0;
	for (int i = 0; i< contours.size(); i++)
	{
		//if (contours[i].size() > len)
		//{
		//	len = contours[i].size();
		//	index = i;
		//}
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, 0, 0, Point());
	}
	//cout << "index = " << index << endl;
	//cout << "contours size = " << contours.size() << endl;

//	fun(contours);

	// �ڴ�������ʾ���
	namedWindow("Contours", CV_WINDOW_NORMAL);
	imshow("Contours", drawing);
	waitKey(0);
}

void fun(vector<vector<Point> > contours)
{
	int left, right, top, bottom;
	bool flag = true;
	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = 0; j < contours[i].size(); j++)
		{
			if (flag)
			{
				left = contours[i][j].x;
				right = contours[i][j].x;
				top = contours[i][j].y;
				bottom = contours[i][j].y;
				flag = false;
			}
			if (contours[i][j].x < left)
			{
				left = contours[i][j].x;
			}
			if (contours[i][j].x > right)
			{
				right = contours[i][j].x;
			}
			if (contours[i][j].y < top)
			{
				top = contours[i][j].y;
			}
			if (contours[i][j].y > bottom)
			{
				bottom = contours[i][j].y;
			}
		}
	}
	cout << "left = " << left << endl;
	cout << "right = " << right << endl;
	cout << "top = " << top << endl;
	cout << "bottom = " << bottom << endl;

}

