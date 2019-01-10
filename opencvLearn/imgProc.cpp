#include<iostream>
#include<atlimage.h>
#include<Windows.h>
#include <fstream>
#include <io.h>
#include<direct.h>
#include<list>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

void getAllFiles(string path, list<string> * li);
//�������ؿ��������
int Square(const char * srcName, const char * dstName, const int& left, const int& right, const int& top, const int& bottom, const int& addLen);
//�������ؿ��������
int Square(const char * srcName, const char * dstName);
void procAllImage(const char * srcPath);
//�����������Ե����
void fun(vector<vector<Point> > contours, int &left, int &right, int &top, int &bottom);
//ʶ������
int outline(const char * srcName, const char * dstName, int border);
//funtion:��������������
int conArea(vector<vector<Point> > contours);
//funtion:�����������Ե����
void fun(vector<vector<Point> > contours, int index, int &left, int &right, int &top, int &bottom);

//�����ļ���
void mkdirPath(const char * fileName);
int test();

int mainprc(int argc, char * argv[])
{
//	char srcpath[] = "E:/����/image01";
	char srcpath[] = "E:\\����\\image\\H841L0009_710_2_0.png";
	char dstname[] = "dst.png";
	outline(srcpath, dstname,20);
	procAllImage(srcpath);
	return 0;
}

void getAllFiles(string path, list<string> * li)
{
	long hFile = 0;               //�ļ����
	struct _finddata_t fileinfo;  //�ļ���Ϣ��ȡ�ṹ
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))    //�Ƚ��ļ������Ƿ����ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), li);     //������ļ��У��������±���
				}
			}
			else    //���ļ�
			{
				li->push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}
}

/*funtion:ͼ�����Ϊ������
srcName:ԭͼ
dstName:Ŀ��ͼ
return��0���ɹ���-1�������߽�
*/
int Square(const char * srcName, const char * dstName, const int& left, const int& right, const int& top, const int& bottom, const int& addLen)
{
	Mat L = imread(srcName, -1);
	//long oRows = L.rows; //ԭͼ�ĸ�
	//long oCols = L.cols; //ԭͼ�������ݵĿ�
	int oRows = bottom - top + 2 * addLen;
	int oCols = right - left + 2 * addLen;
	int x = left - addLen;
	int y = top - addLen;
	Mat srcImgRoi = L(Rect(x, y, oCols, oRows));
	long NdR = oRows > oCols ? oRows : oCols;//��ͼ�ı߳�
	Mat New;//(NdR, NdR, CV_8UC4, Scalar(255, 255, 255, 0));
	New.create(NdR, NdR, CV_8UC4);
	New = Scalar(255, 255, 255, 0);
	Mat imgRoi;
	if (NdR == oRows)
	{
		imgRoi = New(Rect((NdR - oCols) / 2, 0, oCols, oRows));
	}
	else
	{
		imgRoi = New(Rect(0, (NdR - oRows) / 2, oCols, oRows));
	}
	addWeighted(imgRoi, 0, srcImgRoi, 1, 0.0, imgRoi);
	imwrite(dstName, New);
	return 0;
}

/*funtion:ͼ�����Ϊ������
 srcName:ԭͼ
 dstName:Ŀ��ͼ
 return��0���ɹ���-1�������߽�
*/
int Square(const char * srcName, const char * dstName)
{
	Mat L = imread(srcName, -1);
	long oRows = L.rows; //ԭͼ�ĸ�
	long oCols = L.cols; //ԭͼ�������ݵĿ�
	long NdR = oRows > oCols ? oRows : oCols;//��ͼ�ı߳�
	Mat New;//(NdR, NdR, CV_8UC4, Scalar(255, 255, 255, 0));
	New.create(NdR, NdR, CV_8UC4);
	New = Scalar(255, 255, 255, 0);
	Mat imgRoi;
	if (NdR == oRows)
	{
		imgRoi = New(Rect((NdR - oCols) / 2, 0, oCols, oRows));
	}
	else
	{
		imgRoi = New(Rect(0, (NdR - oRows) / 2, oCols, oRows));
	}
	addWeighted(imgRoi, 0, L, 1, 0.0, imgRoi);
	imwrite(dstName, New);
	return 0;
}

/*funtion:ʶ������
 srcName:ԭͼ
 dstName:Ŀ��ͼ
 border���߽��С
 return��0���ɹ���-1��ͼƬ����ʧ�ܣ�-2�������߽�
*/
int outline(const char * srcName, const char * dstName,int border)
{
	double ratio = 0.1;
	int left = 0, right = 0, top = 0, bottom = 0;
	/// ����Դͼ��
	Mat src = imread(srcName, 1);
	if (src.empty())
	{
		return -1;
	}
	resize(src, src,Size(4480 * ratio ,6720 * ratio));
	
	/// ת�ɻҶ�ͼ
	Mat gray;
	cvtColor(src, gray, CV_BGRA2GRAY);
	imshow("gray image", gray);

	//��ֵ��
	threshold(gray, gray, 0, 255, THRESH_BINARY | THRESH_OTSU);
	bitwise_not(gray, gray);
	imshow("thresh image", gray);

	// Ѱ������
	vector<vector<Point> > contours;
	findContours(gray, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	// �������
	Mat drawing = Mat::zeros(gray.size(), CV_8UC3);
	cout << "contours.size() = " << contours.size() << endl;
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255,255,0);
		drawContours(drawing, contours, i, color);
	}
	// �ڴ�������ʾ���
	imshow("Contours image", drawing);
	waitKey(0);
//	fun(contours, left, right, top, bottom);
//	int index = conArea(contours);
//	fun(contours,index, left, right, top, bottom);
	if (left - border > 0 && right + border < src.cols && top - border > 0 && bottom + border < src.rows)
	{
		//�������ؿ��������
//		Square(srcName, dstName, left, right, top, bottom, border);
	}
	else
	{
		return -2;
	}

	return 0;
}

int test(){
	Mat src = imread("H842K0006_001_2_0.png", 1);
	if (src.empty())
	{
		return -1;
	}
	resize(src, src, Size(448, 672));
	imshow("src window", src);

	Mat gray;
	cvtColor(src, src, COLOR_BGRA2GRAY);
	imshow("gray window", src);

	threshold(src, src, 0, 255, THRESH_BINARY | THRESH_OTSU);
	bitwise_not(src, src);
	imshow("thresh window", src);

	Mat dst = Mat::zeros(src.size(), CV_8UC3);
	vector<vector<Point>> contours; // vector���Կ����ɱ�����
	//findContours(src, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(src, contours, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i < contours.size(); i++){
		drawContours(dst, contours, i, Scalar(128));
	}
	imshow("hcl window", dst);
	waitKey(0);
}

void procAllImage(const char * srcPath)
{
	list<string> * li = new list<string>;
	getAllFiles(srcPath, li);
	list<string>::iterator it;
	string tailarPath;
	string squarePath;
	string src;
	for (it = li->begin(); it != li->end(); it++)
	{
		src = *it;
		tailarPath = src.replace(src.find("image01"), 7, "square01");
//		squarePath = src.replace(src.find("image"), 6, "square");
		std::cout << "dst = " << tailarPath.c_str() << endl;
		std::cout << "src = " << (*it).c_str() << endl;
		outline((*it).c_str(), tailarPath.c_str(),20);
//		Square(tailarPath.c_str(), squarePath.c_str());
	}
}

/*funtion:��������������
contours:����
return��i �ڼ�������
*/
int conArea(vector<vector<Point> > contours)
{
	double temp = -1;
	int index = -1;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i], true);
		if (temp < area)
		{
			temp = area;
			index = i;
		}
	}
	cout << "index = " << index << endl;
	return index;
}

/*funtion:�����������Ե����
contours:����
index���ڼ�������
left������ߵ�
right�����ұߵ�
top�����ϱߵ�
bottom�����±ߵ�
*/
void fun(vector<vector<Point> > contours,int index, int &left, int &right, int &top, int &bottom)
{
	bool flag = true;
	for (int j = 0; j<contours[index].size(); j++)
	{
		if (flag)
		{
			left = contours[index][j].x;
			right = contours[index][j].x;
			top = contours[index][j].y;
			bottom = contours[index][j].y;
			flag = false;
		}
		if (contours[index][j].x < left)
		{
			left = contours[index][j].x;
		}
		if (contours[index][j].x > right)
		{
			right = contours[index][j].x;
		}
		if (contours[index][j].y < top)
		{
			top = contours[index][j].y;
		}
		if (contours[index][j].y > bottom)
		{
			bottom = contours[index][j].y;
		}
	}
	cout << "left = " << left << endl;
	cout << "right = " << right << endl;
	cout << "top = " << top << endl;
	cout << "bottom = " << bottom << endl;

}



/*funtion:����ͼ�����Ե����
contours:����
left������ߵ�
right�����ұߵ�
top�����ϱߵ�
bottom�����±ߵ�
return��0���ɹ���-1�������߽�
*/
void fun(vector<vector<Point> > contours, int &left, int &right, int &top, int &bottom)
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
	cout << "left = " << left << endl;
	cout << "right = " << right << endl;
	cout << "top = " << top << endl;
	cout << "bottom = " << bottom << endl;

}


//�����ļ���
void mkdirPath(const char * fileName)
{
	if (0 != _access(fileName, 0))
	{
		_mkdir(fileName);
	}
}

