#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <fstream>

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
	/*参数：输入，       
		   输出，       
		   二值图像的最大值 ，      
		   在一个邻域内计算阈值所采用的算法，有两个取值分别为 ADAPTIVE_THRESH_MEAN_C 和 ADAPTIVE_THRESH_GAUSSIAN_C ，     
		   阈值类型只有两个取值，分别为 THRESH_BINARY 和THRESH_BINARY_INV，（blockSize）adaptiveThreshold的计算单位是像素的邻域块，邻域块取多大就由这个值作决定，    
		   偏移值调整量
		   */
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
	//边缘检测
	Mat cannyImg = canny_Img(src);
	//输入图像转灰度
	Mat dst;// = gray_Img(cannyImg);
	cvtColor(cannyImg, dst, CV_GRAY2BGR);
	namedWindow("input_Img_gray", 0);
	imshow("input_Img_gray", cannyImg);
	//霍夫线变换
	vector<Vec2f> lines;
	HoughLines(cannyImg, lines, 0.1, CV_PI / 360, 10, 0, 0);
	/*绘制*/
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
	//输入图像转灰度
	Mat grayImg = gray_Img(src);
	imshow("input_Img_gray", grayImg);
	
	//二值化图像
	Mat thresholdImg = threshold_Img(grayImg);
	namedWindow("input_Img_threshold", 0);
	imshow("input_Img_threshold", thresholdImg);

	Mat verticalLine = get_Vertical(src);
	Mat horizontalLine = get_Horizontal(src);
	//先腐蚀再膨胀
	Mat vertical_Line_erode = erode_Img(thresholdImg, verticalLine);
	imshow("horizontalLine", vertical_Line_erode);
	//Mat vertical_Line_dilate = dilate_Img(vertical_Line_erode, verticalLine);
	////显示图像中的水平线
	//namedWindow("verticalLine", 0);
	//imshow("verticalLine", vertical_Line_dilate);

	//Mat horizontal_Line_erode = erode_Img(thresholdImg, horizontalLine);
	//Mat horizontal_Line_dilate = dilate_Img(horizontal_Line_erode, horizontalLine);
	////显示图像中的垂直线
	//namedWindow("horizontalLine", 0);
	//imshow("horizontalLine", horizontal_Line_dilate);

	waitKey();
	return 0;
}

void main02()
{
	Mat src = imread("000_1_0.png");//图片路径/*image180.jpg*/
	Mat src1 = imread("000_1_0.png",-1);//图片路径/*image180.jpg*/

	Mat gray_image;

	cvtColor(src, gray_image, CV_BGR2GRAY);

	Mat binary_image;

	adaptiveThreshold(gray_image, binary_image, 254, CV_ADAPTIVE_THRESH_MEAN_C,

		CV_THRESH_BINARY, 25, 10);  ///局部自适应二值化函数

	imwrite("erzhi.png", binary_image);

	//去噪

	Mat de_noise = binary_image.clone();

	//中值滤波

	medianBlur(binary_image, de_noise, 5);

	/////////////////////////   膨胀  ////////////////////

	Mat dilate_img;

	Mat element = getStructuringElement(MORPH_RECT, Size(20, 20/*15, 15*/));

	dilate(de_noise, dilate_img, element);

	imwrite("dilate.png", dilate_img);

	//外部加框

	//检测连通域，每一个连通域以一系列的点表示，FindContours方法只能得到第一个域

	vector<vector<Point>> contours;

	vector<Vec4i> hierarchy;

	findContours(dilate_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNAL只检测外部轮廓，可根据自身需求进行调整



	Mat contoursImage(dilate_img.rows, dilate_img.cols, CV_8U, Scalar(255));

	int index = 0;

	for (; index >= 0; index = hierarchy[index][0]) {

		cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);

		// for opencv 2

		// cv::drawContours(dstImage, contours, index, color,  CV_FILLED, 8, hierarchy);//CV_FILLED所在位置表示轮廓线条粗细度，如果为负值（如thickness==cv_filled），绘制在轮廓内部

		// for opencv 3

		//cv::drawContours(contoursImage, contours, index, color, cv::FILLED, 8, hierarchy);



		cv::drawContours(contoursImage, contours, index, Scalar(0), 1, 8, hierarchy);//描绘字符的外轮廓

		Rect rect = boundingRect(contours[index]);//检测外轮廓

		rectangle(src, rect, Scalar(0, 0, 255), 3);//对外轮廓加矩形框

	}

	imwrite("zt.png", src1);
	cout << "完成检测";
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

		cv::cvtColor(gray, gray, CV_RGBA2GRAY);//灰度化



		int thresh_size = (100 / 4) * 2 + 1; //自适应二值化阈值

		adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, thresh_size, thresh_size / 3);

		morphologyEx(gray, gray, MORPH_OPEN, Mat());//形态学开运算去噪点



		vector<vector<Point> > contours;

		cv::findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

		vector<vector<Point>> contours1;

		for (int i = 0; i < contours.size(); ++i)

		{

			if (contours[i].size() > 200)//将比较小的轮廓剔除掉

			{

				contours1.push_back(contours[i]);

			}

		}

		Mat hole(gray.size(), CV_8U, Scalar(0)); //遮罩图层

		cv::drawContours(hole, contours1, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓

//		namedWindow("My hole");
		imwrite("hole.png", hole);

		Mat crop(original.rows, original.cols, CV_8UC3);

		original.copyTo(crop, hole);//将原图像拷贝进遮罩图层
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
	/// 加载源图像
	src = imread("H842K0006_001_2_0.png", 1);

	/// 转成灰度并模糊化降噪
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	/// 创建窗体
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_NORMAL);
	imshow(source_window, src);
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// 用Canny算子检测边缘
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// 寻找轮廓
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// 绘出轮廓
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, 0, 0, Point());
	}
	// 在窗体中显示结果
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

	/// 用Canny算子检测边缘
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// 寻找轮廓
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// 绘出轮廓
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

	// 在窗体中显示结果
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

