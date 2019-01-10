// ImageResize.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
//#include "CVHeader.h"

using namespace std;
using namespace cv;

bool FileExits(string sPath);   //判断文件是否存在					
int ParseParam(int argc, char *argv[], map<string, string> &mapParams);   //解析参数						  
bool outline(Mat &src, int& left, int& right, int& top, int& bottom, int& border);    //查找轮廓
void edge(vector<vector<Point> > contours, int &left, int &right, int &top, int &bottom);   //计算图像最边缘坐标
int Square(Mat &src, Mat & out, const int& left, const int& right, const int& top, const int& bottom, const int& border);  //funtion:图像填充为正方形

int main(int argc, char *argv[])
{
	map<string, string> mapParams;
	int err = ParseParam(argc, argv, mapParams);
	if (err != 0)
	{
		if (err == -1)
			cerr << "没有输入参数！" << endl;
		else if (err == -2)
			cerr << "输入文件不存在！" << endl;
		else if (err == -3)
			cerr << "输入参数错误！" << endl;
		return -1;
	}

	string sInput;
	string sOutput;
	int border = 0;
	int left, right, top, bottom;

	//检查输入文件
	sInput = mapParams["input"];
	if (sInput.empty())
	{
		cerr << "未指定入文件路径！" << endl;
		return -1;
	}

	//设置输出路径
	sOutput = mapParams["output"];

	//设置边界像素大小
	border = ::atoi(mapParams["border"].c_str());
	if (border < 0)
		return -1;

	//处理图片
	cv::Mat src = imread(sInput, -1);
	cv::Mat out;
	if (src.empty())
	{
		cerr << "读取图像数据失败！" << endl;
		return -1;
	}


	bool flag = outline(src, left, right, top, bottom, border);
	if (flag)
	{
		if (left - border > 0 && right + border < src.cols && top - border > 0 && bottom + border < src.rows)
		{
			Square(src, out, left, right, top, bottom, border);   //长方形拓宽成正方形
		}
		else
		{
			cerr << "边界超出原图范围！" << endl;
			return -1;
		}
	}
	if (out.empty())
	{
		cerr << "图片处理失败！" << endl;
		return -1;
	}

	try
	{
		imwrite(sOutput, out); //保存文件
	}
	catch (...)
	{
		cerr << "保存文件失败！" << endl;
		return -1;
	}


	return 0;
}

//判断文件是否存在
bool FileExits(char * sPath)
{
	ifstream f(sPath);
	if (f)
	{
		f.close();
		return true;
	}
	return false;
}

//解析参数
int ParseParam(int argc, char *argv[], map<string, string> &mapParams)
{
	if (argc < 2)
		return -1;        //没有输入参数

	if (!FileExits(argv[1]))
		return -2;          //输入的文件不存在
	if (argc < 4)
	{
		cerr << "参数不足！" << endl;
		return -1;
	}
	mapParams.insert({ "input", argv[1] });
	mapParams.insert({ "output", argv[2] });
	mapParams.insert({ "border", argv[3] });
	return 0;
}

/*funtion:识别轮廓
srcName:原图
dstName:目标图
border：边界大小
return：true：成功；false：失败
*/
bool outline(Mat &src, int& left, int& right, int& top, int& bottom, int& border)
{
	try
	{
		// 转成灰度图
		Mat gray;
		cvtColor(src, gray, CV_BGRA2GRAY);

		//二值化
		threshold(gray, gray, 0, 255, THRESH_BINARY | THRESH_OTSU);
		bitwise_not(gray, gray);

		// 寻找轮廓
		vector<vector<Point> > contours;
		findContours(gray, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
		edge(contours, left, right, top, bottom);
	}
	catch (...)
	{
		return false;
	}	
	return true;
}

/*funtion:计算图像最边缘坐标
contours:轮廓
left：最左边点
right：最右边点
top：最上边点
bottom：最下边点
*/
void edge(vector<vector<Point> > contours, int &left, int &right, int &top, int &bottom)
{
	bool flag = true;
	for (int i = 0; i<contours.size(); i++)
	{
		for (int j = 0; j<contours[i].size(); j++)
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
}



/*funtion:图像填充为正方形
srcName:原图
dstName:目标图
return：0：成功；-1：失败
*/
int Square(Mat &src, Mat & out, const int& left, const int& right, const int& top, const int& bottom, const int& border)
{
	int oRows = bottom - top + 2 * border;
	int oCols = right - left + 2 * border;
	int x = left - border;
	int y = top - border;
	Mat srcImgRoi = src(Rect(x, y, oCols, oRows));
	long NdR = oRows > oCols ? oRows : oCols;//新图的边长
	out.create(NdR, NdR, CV_8UC4);
	out = Scalar(255, 255, 255, 0);
	Mat imgRoi;
	if (NdR == oRows)
	{
		imgRoi = out(Rect((NdR - oCols) / 2, 0, oCols, oRows));
	}
	else
	{
		imgRoi = out(Rect(0, (NdR - oRows) / 2, oCols, oRows));
	}
	try
	{
		addWeighted(imgRoi, 0, srcImgRoi, 1, 0.0, imgRoi);
	}
	catch (...)
	{
		cerr << "拓宽图像为正方形失败！" << endl;
		return -1;
	}	
	return 0;
}


