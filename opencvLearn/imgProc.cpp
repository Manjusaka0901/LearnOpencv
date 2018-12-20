#include<iostream>
#include<atlimage.h>
#include<Windows.h>
#include <fstream>
#include <io.h>
#include<list>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

void getAllFiles(string path, list<string> * li);
int Square(const char * srcName, const char * dstName);
void procAllImage(const char * srcPath);

int mainImgProc(int argc, char * argv[])
{
	char srcPath[] = "E:\\图片处理\\image";
	procAllImage(srcPath);
	getchar();
	return 0;
}

void getAllFiles(string path, list<string> * li)
{
	long hFile = 0;               //文件句柄
	struct _finddata_t fileinfo;  //文件信息读取结构
	string p;


	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))    //比较文件类型是否是文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), li);     //如果是文件夹，继续向下遍历
				}
			}
			else    //是文件
			{

				li->push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}

//长方形拓宽成正方形
int Square(const char * srcName, const char * dstName)
{
	Mat L = imread(srcName, -1);
	long oRows = L.rows; //原图的高
	long oCols = L.cols; //原图像素数据的宽
	long NdR = L.rows > L.cols ? L.rows : L.cols;//新图的边长
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

void procAllImage(const char * srcPath)
{
	list<string> * li = new list<string>;
	getAllFiles(srcPath, li);
	list<string>::iterator it;
	string path;
	string src;
	for (it = li->begin(); it != li->end(); it++)
	{
		src = *it;
		path = src.replace(src.find("image"), 5, "Square");
		std::cout << "dst = " << path.c_str() << endl;
		std::cout << "src = " << (*it).c_str() << endl;
		Square((*it).c_str(), path.c_str());
	}
}
