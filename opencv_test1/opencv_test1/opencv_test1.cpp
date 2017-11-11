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
using namespace cv;


const Scalar RED = Scalar(0, 0, 255);
const Scalar PINK = Scalar(230, 130, 255);
const Scalar BLUE = Scalar(255, 0, 0);
const Scalar LIGHTBLUE = Scalar(255, 255, 160);
const Scalar GREEN = Scalar(0, 255, 0);

const int BGD_KEY = EVENT_FLAG_CTRLKEY;
const int FGD_KEY = EVENT_FLAG_SHIFTKEY;

//GC2Inpaint类的定义
class GC2Inpaint
{
public:
	enum { NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
	static const int radius = 2;
	static const int thickness = -1;

	//模型恢复初始状态
	void reset();
	//设定图像窗口名称
	void setImageAndWinName(const Mat& _image, const string& _winName);
	//显示图像
	void showImage() const;
	//处理鼠标点击事件
	void mouseClick(int event, int x, int y, int flags, void* param);
	//下一迭代
	int nextIter();
	//获得当前迭代count(直接返回结果了）
	int getIterCount() const { return iterCount; }

private:
	//设置矩形 Mask
	void setRectInMask();
	//设置Mask 名称
	void setLblsInMask(int flags, Point p, bool isPr);

	//定义变量
	const string* winName;
	const Mat* image;   //处理图像主体
	Mat mask;
	Mat bgdModel, fgdModel;

	uchar rectState, lblsState, prLblsState;
	bool isInitialized;

	Rect rect;
	//分别对应前景pixels，背景pixels以及可能前景pixels和可能北京pixels
	vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
	int iterCount;
};

//GC2Inpaint类的实现
//模型恢复初始状态,所有变量恢复初始状态
void GC2Inpaint::reset()
{
	if (!mask.empty())
		mask.setTo(Scalar::all(GC_BGD));
	bgdPxls.clear(); 
	fgdPxls.clear();
	prBgdPxls.clear(); 
	prFgdPxls.clear();

	isInitialized = false; //初始化时,没有Initialized
	rectState = NOT_SET;
	lblsState = NOT_SET;
	prLblsState = NOT_SET;
	iterCount = 0;
}

//设定图像窗口名称
void GC2Inpaint::setImageAndWinName(const Mat& _image, const string& _winName)
{
	if (_image.empty() || _winName.empty())
		return;
	image = &_image;
	winName = &_winName;
	//在创建窗口的同时创建mask
	mask.create(image->size(), CV_8UC1);
	reset();
}

//显示结果图片，处理窗口回显
void GC2Inpaint::showImage() const
{
	if (image->empty() || winName->empty())
		return;

	Mat res;
	Mat binMask;
	////用于处理结果
	if (!isInitialized)
		image->copyTo(res);
	else
	{
		//对grabcut生成的结果模板进行进一步处理
		if (mask.empty() || mask.type() != CV_8UC1)
			CV_Error(Error::StsBadArg, "输入为空或格式不对(必须为 CV_8UC1)");
		//创建同样大小的返回结果
		////不必要存在，只是为了理解代码
		Mat tmp;
		mask.convertTo(tmp, CV_32FC1);

		///////////////////////////////
		binMask = Mat(mask.size(), CV_8UC1);
		//运算并返回
		binMask = mask & 1;//变量a&1代表的意思是a和1做二进制的且运算，即看a的最后边那一位是不是1。也就是保留奇数。
						   ////不必要存在，只是为了理解代码
		binMask.convertTo(tmp, CV_32FC1);
		///////////////////////////////
		//带有模板的copyto,将原图像中mask的部分拷贝到res中去，显示抠图的结果
		image->copyTo(res, binMask);
		// 在最后的显示部分，将inpaint融合进来
		Mat inpainted;
		inpaint(*image, binMask, inpainted, 3, CV_INPAINT_TELEA);
		imshow("抠图并修补结果", inpainted);
	}
	////用于回显鼠标操作
	vector<Point>::const_iterator it;
	//曲线实际上是由多个小点连起来的,遍历数组显示结果
	for (it = bgdPxls.begin(); it != bgdPxls.end(); ++it)
		circle(res, *it, radius, BLUE, thickness);
	for (it = fgdPxls.begin(); it != fgdPxls.end(); ++it)
		circle(res, *it, radius, RED, thickness);
	for (it = prBgdPxls.begin(); it != prBgdPxls.end(); ++it)
		circle(res, *it, radius, LIGHTBLUE, thickness);
	for (it = prFgdPxls.begin(); it != prFgdPxls.end(); ++it)
		circle(res, *it, radius, PINK, thickness);

	if (rectState == IN_PROCESS || rectState == SET)
		rectangle(res, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);

	imshow(*winName, res);
}

//设置矩形 Mask
void GC2Inpaint::setRectInMask()
{
	CV_Assert(!mask.empty());
	mask.setTo(GC_BGD); // GC_BGD    = 0
	rect.x = max(0, rect.x); // 边界处理，防止将框子画到图像外面去
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, image->cols - rect.x);
	rect.height = min(rect.height, image->rows - rect.y);
	(mask(rect)).setTo(Scalar(GC_PR_FGD)); //GC_PR_FGD = 3  默认框子里面的认为是 可能前景
}
//设置Mask 名称
void GC2Inpaint::setLblsInMask(int flags, Point p, bool isPr)
{
	vector<Point> *bpxls, *fpxls;
	uchar bvalue, fvalue;
	if (!isPr)
	{
		bpxls = &bgdPxls;
		fpxls = &fgdPxls;
		bvalue = GC_BGD;
		fvalue = GC_FGD;
	}
	else
	{
		bpxls = &prBgdPxls;
		fpxls = &prFgdPxls;
		bvalue = GC_PR_BGD;
		fvalue = GC_PR_FGD;
	}
	if (flags & BGD_KEY)
	{
		bpxls->push_back(p);
		circle(mask, p, radius, bvalue, thickness);
	}
	if (flags & FGD_KEY)
	{
		fpxls->push_back(p);
		circle(mask, p, radius, fvalue, thickness);
	}
}

//鼠标点击事件
void GC2Inpaint::mouseClick(int event, int x, int y, int flags, void*)
{
	switch (event)
	{
		//按下鼠标左键
	case EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels
	{
		bool isb = (flags & BGD_KEY) != 0,
			isf = (flags & FGD_KEY) != 0;
		if (rectState == NOT_SET && !isb && !isf)
		{
			rectState = IN_PROCESS;
			rect = Rect(x, y, 1, 1);
		}
		if ((isb || isf) && rectState == SET)
			lblsState = IN_PROCESS;
	}
	break;
	//按下鼠标右键
	case EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
	{
		bool isb = (flags & BGD_KEY) != 0,
			isf = (flags & FGD_KEY) != 0;
		if ((isb || isf) && rectState == SET)
			prLblsState = IN_PROCESS;
	}
	break;
	//抬起鼠标左键
	case EVENT_LBUTTONUP:
		if (rectState == IN_PROCESS)
		{
			rect = Rect(Point(rect.x, rect.y), Point(x, y));
			rectState = SET;
			setRectInMask();
			CV_Assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
			showImage();
		}
		if (lblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), false);
			lblsState = SET;
			showImage();
		}
		break;
		//抬起鼠标右键
	case EVENT_RBUTTONUP:
		if (prLblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), true);
			prLblsState = SET;
			showImage();
		}
		break;
		//移动鼠标,特别是移动鼠标需要考虑到较多状态
	case EVENT_MOUSEMOVE:
		if (rectState == IN_PROCESS)
		{
			rect = Rect(Point(rect.x, rect.y), Point(x, y));
			CV_Assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
			showImage();
		}
		else if (lblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), false);
			showImage();
		}
		else if (prLblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), true);
			showImage();
		}
		break;
	}
}

//处理下一处迭代
int GC2Inpaint::nextIter()
{
	if (isInitialized)
		grabCut(*image, mask, rect, bgdModel, fgdModel, 30);
	else
	{

		if (rectState != SET)
			return iterCount;
		if (lblsState == SET || prLblsState == SET)
			grabCut(*image, mask, rect, bgdModel, fgdModel, 30, GC_INIT_WITH_MASK);
		else
			grabCut(*image, mask, rect, bgdModel, fgdModel, 30, GC_INIT_WITH_RECT);

		isInitialized = true; //只有在这个地方将 isInitialized 设置为true
	}
	iterCount++;

	bgdPxls.clear(); fgdPxls.clear();
	prBgdPxls.clear(); prFgdPxls.clear();

	return iterCount;
}


//生成具体实例类
GC2Inpaint gcapp;

static void on_mouse(int event, int x, int y, int flags, void* param)
{
	gcapp.mouseClick(event, x, y, flags, param);
}

//程序主要部分
int main2(int argc, char** argv)
{
	cout << "\n热键: \n"
		"\tESC - 程序退出\n"
		"\tr - 重置原始图像\n"
		"\tn - 下一个迭代\n"
		"\n"
		"\t鼠标左键 - 设定举行\n"
		"\n"
		"\tCTRL+鼠标左键 - set GC_BGD pixels\n"
		"\tSHIFT+鼠标左键 - set GC_FGD pixels\n"
		"\n"
		"\tCTRL+鼠标右键 - set GC_PR_BGD pixels\n"
		"\tSHIFT+鼠标右键 - set GC_PR_FGD pixels\n" << endl;
	//读入图像
	Mat image = imread("D:\\opencv_leane\\image\\6.jpg", 1);
	if (image.empty())
		return -1;
	//设定用于鼠标的回调函数（这种写法在这里是必须的）
	const string winName = "image";
	namedWindow(winName, WINDOW_AUTOSIZE);
	setMouseCallback(winName, on_mouse, 0);
	//调用gcapp对象的函数
	gcapp.setImageAndWinName(image, winName);
	gcapp.showImage();
	//不断循环
	bool bExit = false;
	while (!bExit)
	{
		//OpenCV方式的getchar
		char c = (char)waitKey(0);
		switch (c)
		{
		case '\x1b':
			cout << "退出……" << endl;
			bExit = true;
		case 'r':
			cout <<"xx"<< endl;
			gcapp.reset();
			gcapp.showImage();
			break;
		case 'n':
			int iterCount = gcapp.getIterCount();
		
			int needcount = 1;
			int newIterCount = 0;
			while (needcount >= 0)
			{

				cout << "<" << iterCount << "... ";
				newIterCount = gcapp.nextIter();

				needcount--;
			}
			if (newIterCount > iterCount)
			{
				gcapp.showImage();
				cout << iterCount << ">" << endl;
			}
			else
				cout << "rect must be determined>" << endl;
			break;
		}
	}
	//释放资源
	destroyWindow(winName);
	return 0;
}
