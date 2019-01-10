// ImageResize.cpp: �������̨Ӧ�ó������ڵ㡣
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

bool FileExits(string sPath);   //�ж��ļ��Ƿ����					
int ParseParam(int argc, char *argv[], map<string, string> &mapParams);   //��������						  
bool outline(Mat &src, int& left, int& right, int& top, int& bottom, int& border);    //��������
void edge(vector<vector<Point> > contours, int &left, int &right, int &top, int &bottom);   //����ͼ�����Ե����
int Square(Mat &src, Mat & out, const int& left, const int& right, const int& top, const int& bottom, const int& border);  //funtion:ͼ�����Ϊ������

int main(int argc, char *argv[])
{
	map<string, string> mapParams;
	int err = ParseParam(argc, argv, mapParams);
	if (err != 0)
	{
		if (err == -1)
			cerr << "û�����������" << endl;
		else if (err == -2)
			cerr << "�����ļ������ڣ�" << endl;
		else if (err == -3)
			cerr << "�����������" << endl;
		return -1;
	}

	string sInput;
	string sOutput;
	int border = 0;
	int left, right, top, bottom;

	//��������ļ�
	sInput = mapParams["input"];
	if (sInput.empty())
	{
		cerr << "δָ�����ļ�·����" << endl;
		return -1;
	}

	//�������·��
	sOutput = mapParams["output"];

	//���ñ߽����ش�С
	border = ::atoi(mapParams["border"].c_str());
	if (border < 0)
		return -1;

	//����ͼƬ
	cv::Mat src = imread(sInput, -1);
	cv::Mat out;
	if (src.empty())
	{
		cerr << "��ȡͼ������ʧ�ܣ�" << endl;
		return -1;
	}


	bool flag = outline(src, left, right, top, bottom, border);
	if (flag)
	{
		if (left - border > 0 && right + border < src.cols && top - border > 0 && bottom + border < src.rows)
		{
			Square(src, out, left, right, top, bottom, border);   //�������ؿ��������
		}
		else
		{
			cerr << "�߽糬��ԭͼ��Χ��" << endl;
			return -1;
		}
	}
	if (out.empty())
	{
		cerr << "ͼƬ����ʧ�ܣ�" << endl;
		return -1;
	}

	try
	{
		imwrite(sOutput, out); //�����ļ�
	}
	catch (...)
	{
		cerr << "�����ļ�ʧ�ܣ�" << endl;
		return -1;
	}


	return 0;
}

//�ж��ļ��Ƿ����
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

//��������
int ParseParam(int argc, char *argv[], map<string, string> &mapParams)
{
	if (argc < 2)
		return -1;        //û���������

	if (!FileExits(argv[1]))
		return -2;          //������ļ�������
	if (argc < 4)
	{
		cerr << "�������㣡" << endl;
		return -1;
	}
	mapParams.insert({ "input", argv[1] });
	mapParams.insert({ "output", argv[2] });
	mapParams.insert({ "border", argv[3] });
	return 0;
}

/*funtion:ʶ������
srcName:ԭͼ
dstName:Ŀ��ͼ
border���߽��С
return��true���ɹ���false��ʧ��
*/
bool outline(Mat &src, int& left, int& right, int& top, int& bottom, int& border)
{
	try
	{
		// ת�ɻҶ�ͼ
		Mat gray;
		cvtColor(src, gray, CV_BGRA2GRAY);

		//��ֵ��
		threshold(gray, gray, 0, 255, THRESH_BINARY | THRESH_OTSU);
		bitwise_not(gray, gray);

		// Ѱ������
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

/*funtion:����ͼ�����Ե����
contours:����
left������ߵ�
right�����ұߵ�
top�����ϱߵ�
bottom�����±ߵ�
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



/*funtion:ͼ�����Ϊ������
srcName:ԭͼ
dstName:Ŀ��ͼ
return��0���ɹ���-1��ʧ��
*/
int Square(Mat &src, Mat & out, const int& left, const int& right, const int& top, const int& bottom, const int& border)
{
	int oRows = bottom - top + 2 * border;
	int oCols = right - left + 2 * border;
	int x = left - border;
	int y = top - border;
	Mat srcImgRoi = src(Rect(x, y, oCols, oRows));
	long NdR = oRows > oCols ? oRows : oCols;//��ͼ�ı߳�
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
		cerr << "�ؿ�ͼ��Ϊ������ʧ�ܣ�" << endl;
		return -1;
	}	
	return 0;
}


