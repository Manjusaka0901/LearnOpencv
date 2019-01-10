#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;

//opencv版本号
#define MY_OPENCV_VER "341" 

//获取跟编译方式相对应的目录
#if defined(_DLL)
	#define MY_OPENCV_LIB_DIR "lib\\"
#else
	#define MY_OPENCV_LIB_DIR "staticlib\\"
#endif

#define _S(s) #s

#define _StrConn2(a,b) a b         
#define _StrConn3(a,b,c) a b c   




//判断lib文件尾部
#ifdef _DEBUG
	#define MY_OPENCV_LIB_EXT "d.lib"   //lib文件扩展名
	#define MY_OPENCV_LIB_TAIL _StrConn2(MY_OPENCV_VER, MY_OPENCV_LIB_EXT)
#else
	#define MY_OPENCV_LIB_EXT ".lib"   //lib文件扩展名
	#define MY_OPENCV_LIB_TAIL _StrConn2(MY_OPENCV_VER, ".lib")
#endif



//如果为静态编译，还需要连接其他第三方库
#if !defined(_DLL)
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "IlmImf", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn2(MY_OPENCV_LIB_DIR, "ippicvmt.lib"))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "ippiw", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "ittnotify", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "libjasper", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "libjpeg", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "libpng", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "libprotobuf", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "libtiff", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "libwebp", MY_OPENCV_LIB_EXT))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "zlib", MY_OPENCV_LIB_EXT))
#endif



#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_calib3d", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_core", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_dnn", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_features2d", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_flann", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_highgui", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_imgcodecs", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_imgproc", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_ml", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_objdetect", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_photo", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_shape", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_stitching", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_superres", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_video", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_videoio", MY_OPENCV_LIB_TAIL))
#pragma comment(lib, _StrConn3(MY_OPENCV_LIB_DIR, "opencv_videostab", MY_OPENCV_LIB_TAIL))



