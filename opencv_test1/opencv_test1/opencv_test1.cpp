/************************************************************************/
//《炼数成金》课程使用
// by jsxyhelu
/************************************************************************/
#include "stdafx.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"
#include <iostream>
//#include "GC2Inpaint.h"

using namespace std;
//using namespace cv;


//const Scalar RED = Scalar(0, 0, 255);
//const Scalar PINK = Scalar(230, 130, 255);
//const Scalar BLUE = Scalar(255, 0, 0);
//const Scalar LIGHTBLUE = Scalar(255, 255, 160);
//const Scalar GREEN = Scalar(0, 255, 0);
//
//const int BGD_KEY = EVENT_FLAG_CTRLKEY;
//const int FGD_KEY = EVENT_FLAG_SHIFTKEY;
//
////GC2Inpaint类的定义
//class GC2Inpaint
//{
//public:
//	enum { NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
//	static const int radius = 2;
//	static const int thickness = -1;
//
//	//模型恢复初始状态
//	void reset();
//	//设定图像窗口名称
//	void setImageAndWinName(const Mat& _image, const string& _winName);
//	//显示图像
//	void showImage() const;
//	//处理鼠标点击事件
//	void mouseClick(int event, int x, int y, int flags, void* param);
//	//下一迭代
//	int nextIter();
//	//获得当前迭代count(直接返回结果了）
//	int getIterCount() const { return iterCount; }
//
//private:
//	//设置矩形 Mask
//	void setRectInMask();
//	//设置Mask 名称
//	void setLblsInMask(int flags, Point p, bool isPr);
//
//	//定义变量
//	const string* winName;
//	const Mat* image;   //处理图像主体
//	Mat mask;
//	Mat bgdModel, fgdModel;
//
//	uchar rectState, lblsState, prLblsState;
//	bool isInitialized;
//
//	Rect rect;
//	//分别对应前景pixels，背景pixels以及可能前景pixels和可能北京pixels
//	vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
//	int iterCount;
//};
//
////GC2Inpaint类的实现
////模型恢复初始状态,所有变量恢复初始状态
//void GC2Inpaint::reset()
//{
//	if (!mask.empty())
//		mask.setTo(Scalar::all(GC_BGD));
//	bgdPxls.clear(); 
//	fgdPxls.clear();
//	prBgdPxls.clear(); 
//	prFgdPxls.clear();
//
//	isInitialized = false; //初始化时,没有Initialized
//	rectState = NOT_SET;
//	lblsState = NOT_SET;
//	prLblsState = NOT_SET;
//	iterCount = 0;
//}
//
////设定图像窗口名称
//void GC2Inpaint::setImageAndWinName(const Mat& _image, const string& _winName)
//{
//	if (_image.empty() || _winName.empty())
//		return;
//	image = &_image;
//	winName = &_winName;
//	//在创建窗口的同时创建mask
//	mask.create(image->size(), CV_8UC1);
//	reset();
//}
//
////显示结果图片，处理窗口回显
//void GC2Inpaint::showImage() const
//{
//	if (image->empty() || winName->empty())
//		return;
//
//	Mat res;
//	Mat binMask;
//	////用于处理结果
//	if (!isInitialized)
//		image->copyTo(res);
//	else
//	{
//		//对grabcut生成的结果模板进行进一步处理
//		if (mask.empty() || mask.type() != CV_8UC1)
//			CV_Error(Error::StsBadArg, "输入为空或格式不对(必须为 CV_8UC1)");
//		//创建同样大小的返回结果
//		////不必要存在，只是为了理解代码
//		Mat tmp;
//		mask.convertTo(tmp, CV_32FC1);
//
//		///////////////////////////////
//		binMask = Mat(mask.size(), CV_8UC1);
//		//运算并返回
//		binMask = mask & 1;//变量a&1代表的意思是a和1做二进制的且运算，即看a的最后边那一位是不是1。也就是保留奇数。
//						   ////不必要存在，只是为了理解代码
//		binMask.convertTo(tmp, CV_32FC1);
//		///////////////////////////////
//		//带有模板的copyto,将原图像中mask的部分拷贝到res中去，显示抠图的结果
//		image->copyTo(res, binMask);
//		// 在最后的显示部分，将inpaint融合进来
//		Mat inpainted;
//		inpaint(*image, binMask, inpainted, 3, CV_INPAINT_TELEA);
//		imshow("抠图并修补结果", inpainted);
//	}
//	////用于回显鼠标操作
//	vector<Point>::const_iterator it;
//	//曲线实际上是由多个小点连起来的,遍历数组显示结果
//	for (it = bgdPxls.begin(); it != bgdPxls.end(); ++it)
//		circle(res, *it, radius, BLUE, thickness);
//	for (it = fgdPxls.begin(); it != fgdPxls.end(); ++it)
//		circle(res, *it, radius, RED, thickness);
//	for (it = prBgdPxls.begin(); it != prBgdPxls.end(); ++it)
//		circle(res, *it, radius, LIGHTBLUE, thickness);
//	for (it = prFgdPxls.begin(); it != prFgdPxls.end(); ++it)
//		circle(res, *it, radius, PINK, thickness);
//
//	if (rectState == IN_PROCESS || rectState == SET)
//		rectangle(res, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);
//
//	imshow(*winName, res);
//}
//
////设置矩形 Mask
//void GC2Inpaint::setRectInMask()
//{
//	CV_Assert(!mask.empty());
//	mask.setTo(GC_BGD); // GC_BGD    = 0
//	rect.x = max(0, rect.x); // 边界处理，防止将框子画到图像外面去
//	rect.y = max(0, rect.y);
//	rect.width = min(rect.width, image->cols - rect.x);
//	rect.height = min(rect.height, image->rows - rect.y);
//	(mask(rect)).setTo(Scalar(GC_PR_FGD)); //GC_PR_FGD = 3  默认框子里面的认为是 可能前景
//}
////设置Mask 名称
//void GC2Inpaint::setLblsInMask(int flags, Point p, bool isPr)
//{
//	vector<Point> *bpxls, *fpxls;
//	uchar bvalue, fvalue;
//	if (!isPr)
//	{
//		bpxls = &bgdPxls;
//		fpxls = &fgdPxls;
//		bvalue = GC_BGD;
//		fvalue = GC_FGD;
//	}
//	else
//	{
//		bpxls = &prBgdPxls;
//		fpxls = &prFgdPxls;
//		bvalue = GC_PR_BGD;
//		fvalue = GC_PR_FGD;
//	}
//	if (flags & BGD_KEY)
//	{
//		bpxls->push_back(p);
//		circle(mask, p, radius, bvalue, thickness);
//	}
//	if (flags & FGD_KEY)
//	{
//		fpxls->push_back(p);
//		circle(mask, p, radius, fvalue, thickness);
//	}
//}
//
////鼠标点击事件
//void GC2Inpaint::mouseClick(int event, int x, int y, int flags, void*)
//{
//	switch (event)
//	{
//		//按下鼠标左键
//	case EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels
//	{
//		bool isb = (flags & BGD_KEY) != 0,
//			isf = (flags & FGD_KEY) != 0;
//		if (rectState == NOT_SET && !isb && !isf)
//		{
//			rectState = IN_PROCESS;
//			rect = Rect(x, y, 1, 1);
//		}
//		if ((isb || isf) && rectState == SET)
//			lblsState = IN_PROCESS;
//	}
//	break;
//	//按下鼠标右键
//	case EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
//	{
//		bool isb = (flags & BGD_KEY) != 0,
//			isf = (flags & FGD_KEY) != 0;
//		if ((isb || isf) && rectState == SET)
//			prLblsState = IN_PROCESS;
//	}
//	break;
//	//抬起鼠标左键
//	case EVENT_LBUTTONUP:
//		if (rectState == IN_PROCESS)
//		{
//			rect = Rect(Point(rect.x, rect.y), Point(x, y));
//			rectState = SET;
//			setRectInMask();
//			CV_Assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
//			showImage();
//		}
//		if (lblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), false);
//			lblsState = SET;
//			showImage();
//		}
//		break;
//		//抬起鼠标右键
//	case EVENT_RBUTTONUP:
//		if (prLblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), true);
//			prLblsState = SET;
//			showImage();
//		}
//		break;
//		//移动鼠标,特别是移动鼠标需要考虑到较多状态
//	case EVENT_MOUSEMOVE:
//		if (rectState == IN_PROCESS)
//		{
//			rect = Rect(Point(rect.x, rect.y), Point(x, y));
//			CV_Assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
//			showImage();
//		}
//		else if (lblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), false);
//			showImage();
//		}
//		else if (prLblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), true);
//			showImage();
//		}
//		break;
//	}
//}
//
////处理下一处迭代
//int GC2Inpaint::nextIter()
//{
//	if (isInitialized)
//		grabCut(*image, mask, rect, bgdModel, fgdModel, 30);
//	else
//	{
//
//		if (rectState != SET)
//			return iterCount;
//		if (lblsState == SET || prLblsState == SET)
//			grabCut(*image, mask, rect, bgdModel, fgdModel, 30, GC_INIT_WITH_MASK);
//		else
//			grabCut(*image, mask, rect, bgdModel, fgdModel, 30, GC_INIT_WITH_RECT);
//
//		isInitialized = true; //只有在这个地方将 isInitialized 设置为true
//	}
//	iterCount++;
//
//	bgdPxls.clear(); fgdPxls.clear();
//	prBgdPxls.clear(); prFgdPxls.clear();
//
//	return iterCount;
//}
//
//
////生成具体实例类
//GC2Inpaint gcapp;
//
//static void on_mouse(int event, int x, int y, int flags, void* param)
//{
//	gcapp.mouseClick(event, x, y, flags, param);
//}
//
////程序主要部分
//int capdogface()
//{
//	cout << "\n热键: \n"
//		"\tESC - 程序退出\n"
//		"\tr - 重置原始图像\n"
//		"\tn - 下一个迭代\n"
//		"\n"
//		"\t鼠标左键 - 设定举行\n"
//		"\n"
//		"\tCTRL+鼠标左键 - set GC_BGD pixels\n"
//		"\tSHIFT+鼠标左键 - set GC_FGD pixels\n"
//		"\n"
//		"\tCTRL+鼠标右键 - set GC_PR_BGD pixels\n"
//		"\tSHIFT+鼠标右键 - set GC_PR_FGD pixels\n" << endl;
//	//读入图像
//	Mat image = imread("D:\\dogface.jpg", 1);
//	if (image.empty())
//		return -1;
//	//设定用于鼠标的回调函数（这种写法在这里是必须的）
//	const string winName = "image";
//	namedWindow(winName, WINDOW_AUTOSIZE);
//	setMouseCallback(winName, on_mouse, 0);
//	//调用gcapp对象的函数
//	gcapp.setImageAndWinName(image, winName);
//	gcapp.showImage();
//	//不断循环
//	bool bExit = false;
//	while (!bExit)
//	{
//		//OpenCV方式的getchar
//		char c = (char)waitKey(0);
//		switch (c)
//		{
//		case '\x1b':
//			cout << "退出……" << endl;
//			bExit = true;
//		case 'r':
//			cout <<"xx"<< endl;
//			gcapp.reset();
//			gcapp.showImage();
//			break;
//		case 'n':
//			int iterCount = gcapp.getIterCount();
//		
//			int needcount = 1;
//			int newIterCount = 0;
//			while (needcount >= 0)
//			{
//
//				cout << "<" << iterCount << "... ";
//				newIterCount = gcapp.nextIter();
//
//				needcount--;
//			}
//			if (newIterCount > iterCount)
//			{
//				gcapp.showImage();
//				cout << iterCount << ">" << endl;
//			}
//			else
//				cout << "rect must be determined>" << endl;
//			break;
//		}
//	}
//	//释放资源
//	destroyWindow(winName);
//	return 0;
//}

#include "camerads.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
const char *g_szTitle = "Camera";

int dshowopencamera()
{
	// 1、考虑到已经存在了显示图像的窗口，那就不必再次驱动摄像头了，即便往下驱动那也是摄像头已被占用。
	//if (IsWindowVisible(FindWindow(NULL, g_szTitle)))
	//{
	//	return (-1);
	//}

	//仅仅获取摄像头数目
	int m_iCamCount = CCameraDS::CameraCount();
	printf("There are %d cameras.\n", m_iCamCount);

	if (m_iCamCount == 0)
	{
		return (-1);
	}

	CCameraDS m_CamDS;

	//获取所有摄像头的名称
	for (int i = 0; i < m_iCamCount; i++)
	{
		char szCamName[1024];

		int retval = m_CamDS.CameraName(i, szCamName, sizeof(szCamName));

		if (retval >0)
		{
			printf("Camera #%d's Name is '%s'.\n", i, szCamName);
		}
		else
		{
			printf("Can not get Camera #%d's name.\n", i);
		}
	}

	// 2、考虑到如果有多个摄像头，或者又有其中某个或某几个正在被其它程序占有，故需要逐个遍历，
	// 直到找到可用的为止。
	int m_iCamNum = 0; // 摄像头编号

	IplImage *pFrame = NULL;

	while (m_iCamNum < m_iCamCount)
	{
		if ((!m_CamDS.OpenCamera(m_iCamNum, false, 320, 240)) || ((pFrame = m_CamDS.QueryFrame()) == NULL))
		{
			m_iCamNum++;
		}
		else
		{ // 找到合适的摄像头，退出循环。
			break;
		}

		// 关闭摄像头，必须要关闭，因为即将要进行下一次的检测，检测前要清空当前的占用空间。
		m_CamDS.CloseCamera();
	}

	if (m_iCamNum == m_iCamCount)
	{
		fprintf(stderr, "Can not open camera or is used by another app.\n");

		return (-1);
	}

	cvNamedWindow(g_szTitle);
	//显示
	cvShowImage(g_szTitle, pFrame);

	while (1)
	{
		//获取一帧
		pFrame = m_CamDS.QueryFrame();

		//显示
		cvShowImage(g_szTitle, pFrame);

		if (cvWaitKey(20) == 'q')
		{
			break;
		}
	}

	m_CamDS.CloseCamera(); //可不调用此函数，CCameraDS析构时会自动关闭摄像头

	cvDestroyWindow(g_szTitle);

	return 0;
}



// 单窗口显示多幅图像的函数
void cvShowMultiImages(char* title, int nArgs, ...)
{
	// 略，详见学习笔记（5）
}


int showtwocamera()
{
	int cam_count;

	//仅仅获取摄像头数目
	cam_count = CCameraDS::CameraCount();
	printf("There are %d cameras./n", cam_count);


	//获取所有摄像头的名称
	for (int i = 0; i < cam_count; i++)
	{
		char camera_name[1024];
		int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name));

		if (retval >0)
			printf("Camera #%d's Name is '%s'./n", i, camera_name);
		else
			printf("Can not get Camera #%d's name./n", i);
	}

	if (cam_count == 0)
		return -1;

	// 创建2个摄像头类
	CCameraDS camera1;
	CCameraDS camera2;

	//打开第一个摄像头
	//if(! camera.OpenCamera(0, true)) //弹出属性选择窗口
	if (!camera1.OpenCamera(0, false, 640, 480)) //不弹出属性选择窗口，用代码制定图像宽和高
	{
		fprintf(stderr, "Can not open camera./n");
		return -1;
	}
	//打开第二个摄像头
	camera2.OpenCamera(1, false, 640, 480);


	cvNamedWindow("Multiple Cameras");

	// 初始化在子图像中显示字符的字体格式
	CvFont tFont;
	cvInitFont(&tFont, CV_FONT_HERSHEY_COMPLEX, 0.5f, 0.7f, 0, 1, 8);

	char cam1str[] = "Camera #1";
	char cam2str[] = "Camera #2";

	// 为读取系统时间信息分配内存
	char timestr[25];
	memset(timestr, 0, 25 * sizeof(char));

	while (1)
	{
		//获取一帧
		IplImage *pFrame1 = camera1.QueryFrame();
		IplImage *pFrame2 = camera2.QueryFrame();

		// 获取当前帧的灰度图
		//IplImage* frame_gray_1 = cvCreateImage(cvGetSize(pFrame1), pFrame1->depth, 1);
		//IplImage* frame_gray_2 = cvCreateImage(cvGetSize(pFrame2), pFrame2->depth, 1);
		//cvCvtColor(pFrame1, frame_gray_1, CV_RGB2GRAY);
		//cvCvtColor(pFrame2, frame_gray_2, CV_RGB2GRAY);

		// 对灰度图像进行Canny边缘检测
		// 然后将图像通道数改为三通道
		//IplImage* frame_canny_1 = cvCreateImage(cvGetSize(pFrame1), pFrame1->depth, 1);
		//IplImage* frame_canny_2 = cvCreateImage(cvGetSize(pFrame2), pFrame2->depth, 1);
		//IplImage* frame1 = cvCreateImage(cvGetSize(pFrame1), pFrame1->depth, pFrame1->nChannels);
		//IplImage* frame2 = cvCreateImage(cvGetSize(pFrame2), pFrame2->depth, pFrame2->nChannels);
		//cvCanny(frame_gray_1, frame_canny_1, 20, 75, 3);
		//cvCanny(frame_gray_2, frame_canny_2, 20, 75, 3);
		//cvCvtColor(frame_canny_1, frame1, CV_GRAY2BGR);
		//cvCvtColor(frame_canny_2, frame2, CV_GRAY2BGR);


		// 获取系统时间信息
		time_t rawtime;
		struct tm* timeinfo;

		rawtime = time(NULL);
		timeinfo = localtime(&rawtime);
		char* p = asctime(timeinfo);

		// 字符串 p 的第25个字符是换行符 '/n'
		// 但在子图像中将乱码显示
		// 故仅读取 p 的前 24 个字符
		for (int i = 0; i < 24; i++)
		{
			timestr[i] = *p;
			p++;
		}
		p = NULL;

		// 在每个子图像上显示摄像头序号以及系统时间信息
		//cvPutText(pFrame1, cam1str, cvPoint(95, 15), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(pFrame2, cam2str, cvPoint(95, 15), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(frame1, cam1str, cvPoint(95, 15), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(frame2, cam2str, cvPoint(95, 15), &tFont, CV_RGB(255, 0, 0));

		//cvPutText(pFrame1, timestr, cvPoint(5, 225), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(pFrame2, timestr, cvPoint(5, 225), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(frame1, timestr, cvPoint(5, 225), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(frame2, timestr, cvPoint(5, 225), &tFont, CV_RGB(255, 0, 0));

		// 显示实时的摄像头视频
		//cvShowMultiImages("Multiple Cameras", 4, pFrame1, pFrame2, frame1, frame2);
		cvShowImage("1", pFrame1);
		cvShowImage("2", pFrame2);
		//cvWaitKey(33);
		int key = cvWaitKey(33);
		if (key == 27) break;

		//cvReleaseImage(&frame1);
		//cvReleaseImage(&frame2);
		//cvReleaseImage(&frame_gray_1);
		//cvReleaseImage(&frame_gray_2);
		//cvReleaseImage(&frame_canny_1);
		//cvReleaseImage(&frame_canny_2);
	}

	camera1.CloseCamera(); //可不调用此函数，CCameraDS析构时会自动关闭摄像头
	camera2.CloseCamera();

	cvDestroyWindow("Multiple Cameras");

	return 0;
}

int capture_video(const char*url);
int opentwocamera();
int main()
{
	showtwocamera();
	//opentwocamera();
	//capdogface();
	//capture_video("rtsp://admin:admin@10.10.97.11/mpeg4");
	//cap.open("d:\\work\\4.mp4");//呵呵，就这一句关键
	//capture_video(0);
	return 0;

}

//int capture_video(const char*url)
//{
//	cv::VideoCapture* stream = new cv::VideoCapture(url);
//	if (!stream->isOpened())
//		return -1;
//	//string window_name = url;
//	cv::namedWindow(url, CV_WINDOW_AUTOSIZE);
//	cv::Mat frame;
//	while (true)
//	{
//		if (!stream->read(frame))
//			return -1;
//		cv::imshow(url, frame);
//		cv::waitKey(15);
//	}
//	delete stream;
//	cv::destroyWindow(url);
//
//	return 1;
//}
//
////#include <opencv2/opencv.hpp>
////using namespace std;
//
//int opentwocamera()
//{
//	//int cam_cnt = cvcamGetCamerasCount();
//	//initialize and allocate memory to load the video stream from camera 
//	cv::VideoCapture camera0;
//	camera0.set(CV_CAP_PROP_FRAME_WIDTH, 640);
//	camera0.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
//	cv::VideoCapture camera1;
//	camera1.set(CV_CAP_PROP_FRAME_WIDTH, 640);
//	camera1.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
//	bool ret = camera0.open(1, 0);
//	bool ret2 = camera1.open(0, 0);
//	if (!camera0.isOpened()) return 1;
//	if (!camera1.isOpened()) return 1;
//
//	while (true) {
//		//grab and retrieve each frames of the video sequentially 
//		cv::Mat3b frame0;
//		camera0 >> frame0;
//		cv::Mat frame1;
//		//camera1 >> frame1;
//		camera1.retrieve(frame1, 0); //这里拿到的数据是空的，
//
//		cv::imshow("Video0", frame0);
//		cv::imshow("Video1", frame1);
//		//      std::cout << frame1.rows() << std::endl;
//		//wait for 40 milliseconds
//		int c = cvWaitKey(40);
//
//		//exit the loop if user press "Esc" key  (ASCII value of "Esc" is 27) 
//		if (27 == char(c)) break;
//	}
//
//	return 0;
//}
//
//
