/************************************************************************/
//�������ɽ𡷿γ�ʹ��
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

//GC2Inpaint��Ķ���
class GC2Inpaint
{
public:
	enum { NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
	static const int radius = 2;
	static const int thickness = -1;

	//ģ�ͻָ���ʼ״̬
	void reset();
	//�趨ͼ�񴰿�����
	void setImageAndWinName(const Mat& _image, const string& _winName);
	//��ʾͼ��
	void showImage() const;
	//����������¼�
	void mouseClick(int event, int x, int y, int flags, void* param);
	//��һ����
	int nextIter();
	//��õ�ǰ����count(ֱ�ӷ��ؽ���ˣ�
	int getIterCount() const { return iterCount; }

private:
	//���þ��� Mask
	void setRectInMask();
	//����Mask ����
	void setLblsInMask(int flags, Point p, bool isPr);

	//�������
	const string* winName;
	const Mat* image;   //����ͼ������
	Mat mask;
	Mat bgdModel, fgdModel;

	uchar rectState, lblsState, prLblsState;
	bool isInitialized;

	Rect rect;
	//�ֱ��Ӧǰ��pixels������pixels�Լ�����ǰ��pixels�Ϳ��ܱ���pixels
	vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
	int iterCount;
};

//GC2Inpaint���ʵ��
//ģ�ͻָ���ʼ״̬,���б����ָ���ʼ״̬
void GC2Inpaint::reset()
{
	if (!mask.empty())
		mask.setTo(Scalar::all(GC_BGD));
	bgdPxls.clear(); 
	fgdPxls.clear();
	prBgdPxls.clear(); 
	prFgdPxls.clear();

	isInitialized = false; //��ʼ��ʱ,û��Initialized
	rectState = NOT_SET;
	lblsState = NOT_SET;
	prLblsState = NOT_SET;
	iterCount = 0;
}

//�趨ͼ�񴰿�����
void GC2Inpaint::setImageAndWinName(const Mat& _image, const string& _winName)
{
	if (_image.empty() || _winName.empty())
		return;
	image = &_image;
	winName = &_winName;
	//�ڴ������ڵ�ͬʱ����mask
	mask.create(image->size(), CV_8UC1);
	reset();
}

//��ʾ���ͼƬ�������ڻ���
void GC2Inpaint::showImage() const
{
	if (image->empty() || winName->empty())
		return;

	Mat res;
	Mat binMask;
	////���ڴ�����
	if (!isInitialized)
		image->copyTo(res);
	else
	{
		//��grabcut���ɵĽ��ģ����н�һ������
		if (mask.empty() || mask.type() != CV_8UC1)
			CV_Error(Error::StsBadArg, "����Ϊ�ջ��ʽ����(����Ϊ CV_8UC1)");
		//����ͬ����С�ķ��ؽ��
		////����Ҫ���ڣ�ֻ��Ϊ��������
		Mat tmp;
		mask.convertTo(tmp, CV_32FC1);

		///////////////////////////////
		binMask = Mat(mask.size(), CV_8UC1);
		//���㲢����
		binMask = mask & 1;//����a&1�������˼��a��1�������Ƶ������㣬����a��������һλ�ǲ���1��Ҳ���Ǳ���������
						   ////����Ҫ���ڣ�ֻ��Ϊ��������
		binMask.convertTo(tmp, CV_32FC1);
		///////////////////////////////
		//����ģ���copyto,��ԭͼ����mask�Ĳ��ֿ�����res��ȥ����ʾ��ͼ�Ľ��
		image->copyTo(res, binMask);
		// ��������ʾ���֣���inpaint�ںϽ���
		Mat inpainted;
		inpaint(*image, binMask, inpainted, 3, CV_INPAINT_TELEA);
		imshow("��ͼ���޲����", inpainted);
	}
	////���ڻ���������
	vector<Point>::const_iterator it;
	//����ʵ�������ɶ��С����������,����������ʾ���
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

//���þ��� Mask
void GC2Inpaint::setRectInMask()
{
	CV_Assert(!mask.empty());
	mask.setTo(GC_BGD); // GC_BGD    = 0
	rect.x = max(0, rect.x); // �߽紦����ֹ�����ӻ���ͼ������ȥ
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, image->cols - rect.x);
	rect.height = min(rect.height, image->rows - rect.y);
	(mask(rect)).setTo(Scalar(GC_PR_FGD)); //GC_PR_FGD = 3  Ĭ�Ͽ����������Ϊ�� ����ǰ��
}
//����Mask ����
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

//������¼�
void GC2Inpaint::mouseClick(int event, int x, int y, int flags, void*)
{
	switch (event)
	{
		//����������
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
	//��������Ҽ�
	case EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
	{
		bool isb = (flags & BGD_KEY) != 0,
			isf = (flags & FGD_KEY) != 0;
		if ((isb || isf) && rectState == SET)
			prLblsState = IN_PROCESS;
	}
	break;
	//̧��������
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
		//̧������Ҽ�
	case EVENT_RBUTTONUP:
		if (prLblsState == IN_PROCESS)
		{
			setLblsInMask(flags, Point(x, y), true);
			prLblsState = SET;
			showImage();
		}
		break;
		//�ƶ����,�ر����ƶ������Ҫ���ǵ��϶�״̬
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

//������һ������
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

		isInitialized = true; //ֻ��������ط��� isInitialized ����Ϊtrue
	}
	iterCount++;

	bgdPxls.clear(); fgdPxls.clear();
	prBgdPxls.clear(); prFgdPxls.clear();

	return iterCount;
}


//���ɾ���ʵ����
GC2Inpaint gcapp;

static void on_mouse(int event, int x, int y, int flags, void* param)
{
	gcapp.mouseClick(event, x, y, flags, param);
}

//������Ҫ����
int main2(int argc, char** argv)
{
	cout << "\n�ȼ�: \n"
		"\tESC - �����˳�\n"
		"\tr - ����ԭʼͼ��\n"
		"\tn - ��һ������\n"
		"\n"
		"\t������ - �趨����\n"
		"\n"
		"\tCTRL+������ - set GC_BGD pixels\n"
		"\tSHIFT+������ - set GC_FGD pixels\n"
		"\n"
		"\tCTRL+����Ҽ� - set GC_PR_BGD pixels\n"
		"\tSHIFT+����Ҽ� - set GC_PR_FGD pixels\n" << endl;
	//����ͼ��
	Mat image = imread("D:\\opencv_leane\\image\\6.jpg", 1);
	if (image.empty())
		return -1;
	//�趨�������Ļص�����������д���������Ǳ���ģ�
	const string winName = "image";
	namedWindow(winName, WINDOW_AUTOSIZE);
	setMouseCallback(winName, on_mouse, 0);
	//����gcapp����ĺ���
	gcapp.setImageAndWinName(image, winName);
	gcapp.showImage();
	//����ѭ��
	bool bExit = false;
	while (!bExit)
	{
		//OpenCV��ʽ��getchar
		char c = (char)waitKey(0);
		switch (c)
		{
		case '\x1b':
			cout << "�˳�����" << endl;
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
	//�ͷ���Դ
	destroyWindow(winName);
	return 0;
}
