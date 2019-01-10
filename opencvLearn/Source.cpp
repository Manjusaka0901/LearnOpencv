#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;


Mat imgSrc;
int sliderVal = 160;   //�켣����ʼֵ

/*�켣���Ļص�����*/
void on_trackbar(int, void *)
{
	Mat bw = sliderVal < 128 ? (imgSrc < sliderVal) : (imgSrc > sliderVal);
	//����������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//��������
	findContours(bw, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//��ʼ��dst
	Mat dst = Mat::zeros(imgSrc.size(), CV_8UC3);
	//��ʼ����
	if (!contours.empty() && !hierarchy.empty())
	{
		//�������ж������������������ɫֵ���Ƹ�����������ɲ���
		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			Scalar color((rand()&255), (rand()&255), (rand()&255));
			//�����������
			drawContours(dst, contours, idx, color, CV_FILLED, 8, hierarchy);
		}
	}
	//��ʾ����
	imshow("�켣��Ӧ��",dst);
}

int maintrackbar(int argc, char * argv[])
{
	imgSrc = imread("contrast.png",0);  //����ͼƬ
	if (!imgSrc.data)
	{
		cout << "contrast.png����ʧ�ܣ�" << endl;
		return -1;
	}

	namedWindow("�켣��Ӧ��");
	createTrackbar("trackbar", "�켣��Ӧ��", &sliderVal, 255, on_trackbar);
	on_trackbar(sliderVal, 0);
	waitKey(0);      //չʾͼƬ��show��waitKey����ʹ�ã�
	return 0;
}

//�������ؿ��������
int mainSquare(int argc, char* argv[])
{
	Mat L = imread("000_1_0.png", -1);
	long oRows = L.rows; //ԭͼ�ĸ�
	long oCols = L.cols; //ԭͼ�������ݵĿ�
	long NdR = L.rows > L.cols ? L.rows : L.cols;//��ͼ�ı߳�
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

//��ɫͨ������ͺϲ�
int mainSplitMerge(int argc, char * argv[])
{
	imgSrc = imread("green.jpg");  //����ͼƬ
	if (!imgSrc.data)
	{
		cout << "green.jpg����ʧ�ܣ�" << endl;
	}
	Mat imgLogo = imread("rainRoi.jpg");
	if (!imgLogo.data)
	{
		cout << "rainRoi.jpg����ʧ�ܣ�" << endl;
	}
	Mat imgDst;
	vector<Mat> channels;
	vector<Mat> channelLogo;

	split(imgSrc, channels);  //����ɫ��ͨ�� B��G��R

	split(imgLogo, channelLogo);

	Mat imgSrcBlue = channels.at(1); //0����ԭͼblueͨ�������ø�imgSrcBlue

	imshow("Blue", channels.at(0));
	imshow("Green", channels.at(1));
	imshow("Red", channels.at(2));

	Mat imgRoi = imgSrcBlue(Rect(0, 0, imgLogo.cols, imgLogo.rows));

	addWeighted(imgRoi, 0.1, channelLogo.at(1), 0.9, 0.0, imgRoi);  //��Ȩͼ���������Ҫһ�£���ȣ�ͨ������

	merge(channels, imgDst);


	namedWindow("ͼ�����ϲ�");

//	imshow("ͼ�����ϲ�", imgDst);
	waitKey();      //չʾͼƬ��show��waitKey����ʹ�ã�
	return 0;
}

//��Ȩ���
int mainWeighted(int argc, char * argv[])
{
	double alpha = 0.8;
	double beta = 1.0 - alpha;

	Mat imgSrc = imread("green.jpg");  //����ͼƬ
	if (!imgSrc.data)
	{
		cout << "green.jpg����ʧ�ܣ�" << endl;
	}
	Mat imgLogo = imread("rain.jpg");
	if (!imgLogo.data)
	{
		cout << "rain.jpg����ʧ�ܣ�" << endl;
	}
	Mat imgDst;

	addWeighted(imgSrc, alpha, imgLogo, beta, 0.0, imgDst); //ͼ�����Ի��

	namedWindow("ͼ�����Ի��");

	imshow("ͼ�����Ի��", imgDst);
	waitKey();      //չʾͼƬ��show��waitKey����ʹ�ã�
	return 0;
}

//��Ĥ��ͼ���
int mainmask(int argc, char * argv[])
{
	Mat imgSrc = imread("src.png");  //����ͼƬ
	Mat imgLogo = imread("logo.png");
	Mat imageROI;
	imageROI = imgSrc(Rect(300, 150, imgLogo.cols, imgLogo.rows));  //ָ��imgSrc����Ȥ������imageROI���޸�ͼ����൱���޸�ԭͼ�ϵĸ�����
	Mat mask = imread("logo.png", 0);//���ػҶ�ͼΪͼ����Ĥ
	imshow("mask", mask);
	imgLogo.copyTo(imageROI, mask); //imgLogo��mask����󿽱���imageROI��imageLoge��mask��С��һ��

	namedWindow("src��logo����");

	imshow("src��logo����", imgSrc);
	waitKey();      //չʾͼƬ��show��waitKey����ʹ�ã�
	return 0;
}

//����ROI����Ȥ����
int mainroi(int argc, char * argv[])
{
	Mat imgSrc = imread("1_0.png");  //����ͼƬ
	Mat imgLogo = imread("logo.png");
	Mat imageROI;
	imageROI = imgSrc(Rect(75, 172, 314, 388));  //ָ��imgSrc����Ȥ������imageROI���޸�ͼ����൱���޸�ԭͼ�ϵĸ�����
//	addWeighted(imageROI, 0.9, imgLogo, 0.1, 0, imageROI);  //��Ӽ�Ȩ���ںϣ����¦�
	namedWindow("src��logo�ں�");
	imshow("src��logo�ں�", imgSrc);
	waitKey();      //չʾͼƬ��show��waitKey����ʹ�ã�
	return 0;
}

//ͼƬ����չʾ
int mainshow(int argc, char * argv[])
{
	Mat img;
	img = imread("pic.jpg");  //����ͼƬ
	namedWindow("��������");
	imshow("show",img);  
	waitKey(2000);      //չʾͼƬ����2s�رգ�show��waitKey����ʹ�ã�
	return 0;
}