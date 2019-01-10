#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;


Mat imgSrc;
int sliderVal = 160;   //轨迹条初始值

/*轨迹条的回调函数*/
void on_trackbar(int, void *)
{
	Mat bw = sliderVal < 128 ? (imgSrc < sliderVal) : (imgSrc > sliderVal);
	//定义点和向量
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//查找轮廓
	findContours(bw, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//初始化dst
	Mat dst = Mat::zeros(imgSrc.size(), CV_8UC3);
	//开始处理
	if (!contours.empty() && !hierarchy.empty())
	{
		//遍历所有顶层轮廓，随机产生颜色值绘制给各个连接组成部分
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			Scalar color((rand()&255), (rand()&255), (rand()&255));
			//绘制填充轮廓
			drawContours(dst, contours, idx, color, CV_FILLED, 8, hierarchy);
		}
	}
	//显示窗口
	imshow("轨迹条应用",dst);
}

int maintrackbar(int argc, char * argv[])
{
	imgSrc = imread("contrast.png",0);  //加载图片
	if (!imgSrc.data)
	{
		cout << "contrast.png加载失败！" << endl;
		return -1;
	}

	namedWindow("轨迹条应用");
	createTrackbar("trackbar", "轨迹条应用", &sliderVal, 255, on_trackbar);
	on_trackbar(sliderVal, 0);
	waitKey(0);      //展示图片（show和waitKey配套使用）
	return 0;
}

//长方形拓宽成正方形
int mainSquare(int argc, char* argv[])
{
	Mat L = imread("000_1_0.png", -1);
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
	imwrite("new.png", New);
	return 0;
}

//颜色通道分离和合并
int mainSplitMerge(int argc, char * argv[])
{
	imgSrc = imread("green.jpg");  //加载图片
	if (!imgSrc.data)
	{
		cout << "green.jpg加载失败！" << endl;
	}
	Mat imgLogo = imread("rainRoi.jpg");
	if (!imgLogo.data)
	{
		cout << "rainRoi.jpg加载失败！" << endl;
	}
	Mat imgDst;
	vector<Mat> channels;
	vector<Mat> channelLogo;

	split(imgSrc, channels);  //分离色彩通道 B、G、R

	split(imgLogo, channelLogo);

	Mat imgSrcBlue = channels.at(1); //0，将原图blue通道的引用给imgSrcBlue

	imshow("Blue", channels.at(0));
	imshow("Green", channels.at(1));
	imshow("Red", channels.at(2));

	Mat imgRoi = imgSrcBlue(Rect(0, 0, imgLogo.cols, imgLogo.rows));

	addWeighted(imgRoi, 0.1, channelLogo.at(1), 0.9, 0.0, imgRoi);  //加权图长宽和类型要一致，深度（通道数）

	merge(channels, imgDst);


	namedWindow("图像分离合并");

//	imshow("图像分离合并", imgDst);
	waitKey();      //展示图片（show和waitKey配套使用）
	return 0;
}

//加权混合
int mainWeighted(int argc, char * argv[])
{
	double alpha = 0.8;
	double beta = 1.0 - alpha;

	Mat imgSrc = imread("green.jpg");  //加载图片
	if (!imgSrc.data)
	{
		cout << "green.jpg加载失败！" << endl;
	}
	Mat imgLogo = imread("rain.jpg");
	if (!imgLogo.data)
	{
		cout << "rain.jpg加载失败！" << endl;
	}
	Mat imgDst;

	addWeighted(imgSrc, alpha, imgLogo, beta, 0.0, imgDst); //图像线性混合

	namedWindow("图像线性混合");

	imshow("图像线性混合", imgDst);
	waitKey();      //展示图片（show和waitKey配套使用）
	return 0;
}

//掩膜混和计算
int mainmask(int argc, char * argv[])
{
	Mat imgSrc = imread("src.png");  //加载图片
	Mat imgLogo = imread("logo.png");
	Mat imageROI;
	imageROI = imgSrc(Rect(300, 150, imgLogo.cols, imgLogo.rows));  //指定imgSrc感兴趣区域，在imageROI上修改图像就相当于修改原图上的该区域
	Mat mask = imread("logo.png", 0);//加载灰度图为图像掩膜
	imshow("mask", mask);
	imgLogo.copyTo(imageROI, mask); //imgLogo与mask运算后拷贝到imageROI；imageLoge与mask大小需一致

	namedWindow("src与logo叠加");

	imshow("src与logo叠加", imgSrc);
	waitKey();      //展示图片（show和waitKey配套使用）
	return 0;
}

//设置ROI感兴趣区域
int mainroi(int argc, char * argv[])
{
	Mat imgSrc = imread("1_0.png");  //加载图片
	Mat imgLogo = imread("logo.png");
	Mat imageROI;
	imageROI = imgSrc(Rect(75, 172, 314, 388));  //指定imgSrc感兴趣区域，在imageROI上修改图像就相当于修改原图上的该区域
//	addWeighted(imageROI, 0.9, imgLogo, 0.1, 0, imageROI);  //添加加权（融合）αβγ
	namedWindow("src与logo融合");
	imshow("src与logo融合", imgSrc);
	waitKey();      //展示图片（show和waitKey配套使用）
	return 0;
}

//图片加载展示
int mainshow(int argc, char * argv[])
{
	Mat img;
	img = imread("pic.jpg");  //加载图片
	namedWindow("窗口名称");
	imshow("show",img);  
	waitKey(2000);      //展示图片，并2s关闭（show和waitKey配套使用）
	return 0;
}