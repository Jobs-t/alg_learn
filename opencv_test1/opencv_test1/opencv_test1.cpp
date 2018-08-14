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
////GC2Inpaint��Ķ���
//class GC2Inpaint
//{
//public:
//	enum { NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
//	static const int radius = 2;
//	static const int thickness = -1;
//
//	//ģ�ͻָ���ʼ״̬
//	void reset();
//	//�趨ͼ�񴰿�����
//	void setImageAndWinName(const Mat& _image, const string& _winName);
//	//��ʾͼ��
//	void showImage() const;
//	//����������¼�
//	void mouseClick(int event, int x, int y, int flags, void* param);
//	//��һ����
//	int nextIter();
//	//��õ�ǰ����count(ֱ�ӷ��ؽ���ˣ�
//	int getIterCount() const { return iterCount; }
//
//private:
//	//���þ��� Mask
//	void setRectInMask();
//	//����Mask ����
//	void setLblsInMask(int flags, Point p, bool isPr);
//
//	//�������
//	const string* winName;
//	const Mat* image;   //����ͼ������
//	Mat mask;
//	Mat bgdModel, fgdModel;
//
//	uchar rectState, lblsState, prLblsState;
//	bool isInitialized;
//
//	Rect rect;
//	//�ֱ��Ӧǰ��pixels������pixels�Լ�����ǰ��pixels�Ϳ��ܱ���pixels
//	vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
//	int iterCount;
//};
//
////GC2Inpaint���ʵ��
////ģ�ͻָ���ʼ״̬,���б����ָ���ʼ״̬
//void GC2Inpaint::reset()
//{
//	if (!mask.empty())
//		mask.setTo(Scalar::all(GC_BGD));
//	bgdPxls.clear(); 
//	fgdPxls.clear();
//	prBgdPxls.clear(); 
//	prFgdPxls.clear();
//
//	isInitialized = false; //��ʼ��ʱ,û��Initialized
//	rectState = NOT_SET;
//	lblsState = NOT_SET;
//	prLblsState = NOT_SET;
//	iterCount = 0;
//}
//
////�趨ͼ�񴰿�����
//void GC2Inpaint::setImageAndWinName(const Mat& _image, const string& _winName)
//{
//	if (_image.empty() || _winName.empty())
//		return;
//	image = &_image;
//	winName = &_winName;
//	//�ڴ������ڵ�ͬʱ����mask
//	mask.create(image->size(), CV_8UC1);
//	reset();
//}
//
////��ʾ���ͼƬ�������ڻ���
//void GC2Inpaint::showImage() const
//{
//	if (image->empty() || winName->empty())
//		return;
//
//	Mat res;
//	Mat binMask;
//	////���ڴ�����
//	if (!isInitialized)
//		image->copyTo(res);
//	else
//	{
//		//��grabcut���ɵĽ��ģ����н�һ������
//		if (mask.empty() || mask.type() != CV_8UC1)
//			CV_Error(Error::StsBadArg, "����Ϊ�ջ��ʽ����(����Ϊ CV_8UC1)");
//		//����ͬ����С�ķ��ؽ��
//		////����Ҫ���ڣ�ֻ��Ϊ��������
//		Mat tmp;
//		mask.convertTo(tmp, CV_32FC1);
//
//		///////////////////////////////
//		binMask = Mat(mask.size(), CV_8UC1);
//		//���㲢����
//		binMask = mask & 1;//����a&1�������˼��a��1�������Ƶ������㣬����a��������һλ�ǲ���1��Ҳ���Ǳ���������
//						   ////����Ҫ���ڣ�ֻ��Ϊ��������
//		binMask.convertTo(tmp, CV_32FC1);
//		///////////////////////////////
//		//����ģ���copyto,��ԭͼ����mask�Ĳ��ֿ�����res��ȥ����ʾ��ͼ�Ľ��
//		image->copyTo(res, binMask);
//		// ��������ʾ���֣���inpaint�ںϽ���
//		Mat inpainted;
//		inpaint(*image, binMask, inpainted, 3, CV_INPAINT_TELEA);
//		imshow("��ͼ���޲����", inpainted);
//	}
//	////���ڻ���������
//	vector<Point>::const_iterator it;
//	//����ʵ�������ɶ��С����������,����������ʾ���
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
////���þ��� Mask
//void GC2Inpaint::setRectInMask()
//{
//	CV_Assert(!mask.empty());
//	mask.setTo(GC_BGD); // GC_BGD    = 0
//	rect.x = max(0, rect.x); // �߽紦����ֹ�����ӻ���ͼ������ȥ
//	rect.y = max(0, rect.y);
//	rect.width = min(rect.width, image->cols - rect.x);
//	rect.height = min(rect.height, image->rows - rect.y);
//	(mask(rect)).setTo(Scalar(GC_PR_FGD)); //GC_PR_FGD = 3  Ĭ�Ͽ����������Ϊ�� ����ǰ��
//}
////����Mask ����
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
////������¼�
//void GC2Inpaint::mouseClick(int event, int x, int y, int flags, void*)
//{
//	switch (event)
//	{
//		//����������
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
//	//��������Ҽ�
//	case EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
//	{
//		bool isb = (flags & BGD_KEY) != 0,
//			isf = (flags & FGD_KEY) != 0;
//		if ((isb || isf) && rectState == SET)
//			prLblsState = IN_PROCESS;
//	}
//	break;
//	//̧��������
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
//		//̧������Ҽ�
//	case EVENT_RBUTTONUP:
//		if (prLblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), true);
//			prLblsState = SET;
//			showImage();
//		}
//		break;
//		//�ƶ����,�ر����ƶ������Ҫ���ǵ��϶�״̬
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
////������һ������
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
//		isInitialized = true; //ֻ��������ط��� isInitialized ����Ϊtrue
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
////���ɾ���ʵ����
//GC2Inpaint gcapp;
//
//static void on_mouse(int event, int x, int y, int flags, void* param)
//{
//	gcapp.mouseClick(event, x, y, flags, param);
//}
//
////������Ҫ����
//int capdogface()
//{
//	cout << "\n�ȼ�: \n"
//		"\tESC - �����˳�\n"
//		"\tr - ����ԭʼͼ��\n"
//		"\tn - ��һ������\n"
//		"\n"
//		"\t������ - �趨����\n"
//		"\n"
//		"\tCTRL+������ - set GC_BGD pixels\n"
//		"\tSHIFT+������ - set GC_FGD pixels\n"
//		"\n"
//		"\tCTRL+����Ҽ� - set GC_PR_BGD pixels\n"
//		"\tSHIFT+����Ҽ� - set GC_PR_FGD pixels\n" << endl;
//	//����ͼ��
//	Mat image = imread("D:\\dogface.jpg", 1);
//	if (image.empty())
//		return -1;
//	//�趨�������Ļص�����������д���������Ǳ���ģ�
//	const string winName = "image";
//	namedWindow(winName, WINDOW_AUTOSIZE);
//	setMouseCallback(winName, on_mouse, 0);
//	//����gcapp����ĺ���
//	gcapp.setImageAndWinName(image, winName);
//	gcapp.showImage();
//	//����ѭ��
//	bool bExit = false;
//	while (!bExit)
//	{
//		//OpenCV��ʽ��getchar
//		char c = (char)waitKey(0);
//		switch (c)
//		{
//		case '\x1b':
//			cout << "�˳�����" << endl;
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
//	//�ͷ���Դ
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
	// 1�����ǵ��Ѿ���������ʾͼ��Ĵ��ڣ��ǾͲ����ٴ���������ͷ�ˣ���������������Ҳ������ͷ�ѱ�ռ�á�
	//if (IsWindowVisible(FindWindow(NULL, g_szTitle)))
	//{
	//	return (-1);
	//}

	//������ȡ����ͷ��Ŀ
	int m_iCamCount = CCameraDS::CameraCount();
	printf("There are %d cameras.\n", m_iCamCount);

	if (m_iCamCount == 0)
	{
		return (-1);
	}

	CCameraDS m_CamDS;

	//��ȡ��������ͷ������
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

	// 2�����ǵ�����ж������ͷ��������������ĳ����ĳ�������ڱ���������ռ�У�����Ҫ���������
	// ֱ���ҵ����õ�Ϊֹ��
	int m_iCamNum = 0; // ����ͷ���

	IplImage *pFrame = NULL;

	while (m_iCamNum < m_iCamCount)
	{
		if ((!m_CamDS.OpenCamera(m_iCamNum, false, 320, 240)) || ((pFrame = m_CamDS.QueryFrame()) == NULL))
		{
			m_iCamNum++;
		}
		else
		{ // �ҵ����ʵ�����ͷ���˳�ѭ����
			break;
		}

		// �ر�����ͷ������Ҫ�رգ���Ϊ����Ҫ������һ�εļ�⣬���ǰҪ��յ�ǰ��ռ�ÿռ䡣
		m_CamDS.CloseCamera();
	}

	if (m_iCamNum == m_iCamCount)
	{
		fprintf(stderr, "Can not open camera or is used by another app.\n");

		return (-1);
	}

	cvNamedWindow(g_szTitle);
	//��ʾ
	cvShowImage(g_szTitle, pFrame);

	while (1)
	{
		//��ȡһ֡
		pFrame = m_CamDS.QueryFrame();

		//��ʾ
		cvShowImage(g_szTitle, pFrame);

		if (cvWaitKey(20) == 'q')
		{
			break;
		}
	}

	m_CamDS.CloseCamera(); //�ɲ����ô˺�����CCameraDS����ʱ���Զ��ر�����ͷ

	cvDestroyWindow(g_szTitle);

	return 0;
}



// ��������ʾ���ͼ��ĺ���
void cvShowMultiImages(char* title, int nArgs, ...)
{
	// �ԣ����ѧϰ�ʼǣ�5��
}


int showtwocamera()
{
	int cam_count;

	//������ȡ����ͷ��Ŀ
	cam_count = CCameraDS::CameraCount();
	printf("There are %d cameras./n", cam_count);


	//��ȡ��������ͷ������
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

	// ����2������ͷ��
	CCameraDS camera1;
	CCameraDS camera2;

	//�򿪵�һ������ͷ
	//if(! camera.OpenCamera(0, true)) //��������ѡ�񴰿�
	if (!camera1.OpenCamera(0, false, 640, 480)) //����������ѡ�񴰿ڣ��ô����ƶ�ͼ���͸�
	{
		fprintf(stderr, "Can not open camera./n");
		return -1;
	}
	//�򿪵ڶ�������ͷ
	camera2.OpenCamera(1, false, 640, 480);


	cvNamedWindow("Multiple Cameras");

	// ��ʼ������ͼ������ʾ�ַ��������ʽ
	CvFont tFont;
	cvInitFont(&tFont, CV_FONT_HERSHEY_COMPLEX, 0.5f, 0.7f, 0, 1, 8);

	char cam1str[] = "Camera #1";
	char cam2str[] = "Camera #2";

	// Ϊ��ȡϵͳʱ����Ϣ�����ڴ�
	char timestr[25];
	memset(timestr, 0, 25 * sizeof(char));

	while (1)
	{
		//��ȡһ֡
		IplImage *pFrame1 = camera1.QueryFrame();
		IplImage *pFrame2 = camera2.QueryFrame();

		// ��ȡ��ǰ֡�ĻҶ�ͼ
		//IplImage* frame_gray_1 = cvCreateImage(cvGetSize(pFrame1), pFrame1->depth, 1);
		//IplImage* frame_gray_2 = cvCreateImage(cvGetSize(pFrame2), pFrame2->depth, 1);
		//cvCvtColor(pFrame1, frame_gray_1, CV_RGB2GRAY);
		//cvCvtColor(pFrame2, frame_gray_2, CV_RGB2GRAY);

		// �ԻҶ�ͼ�����Canny��Ե���
		// Ȼ��ͼ��ͨ������Ϊ��ͨ��
		//IplImage* frame_canny_1 = cvCreateImage(cvGetSize(pFrame1), pFrame1->depth, 1);
		//IplImage* frame_canny_2 = cvCreateImage(cvGetSize(pFrame2), pFrame2->depth, 1);
		//IplImage* frame1 = cvCreateImage(cvGetSize(pFrame1), pFrame1->depth, pFrame1->nChannels);
		//IplImage* frame2 = cvCreateImage(cvGetSize(pFrame2), pFrame2->depth, pFrame2->nChannels);
		//cvCanny(frame_gray_1, frame_canny_1, 20, 75, 3);
		//cvCanny(frame_gray_2, frame_canny_2, 20, 75, 3);
		//cvCvtColor(frame_canny_1, frame1, CV_GRAY2BGR);
		//cvCvtColor(frame_canny_2, frame2, CV_GRAY2BGR);


		// ��ȡϵͳʱ����Ϣ
		time_t rawtime;
		struct tm* timeinfo;

		rawtime = time(NULL);
		timeinfo = localtime(&rawtime);
		char* p = asctime(timeinfo);

		// �ַ��� p �ĵ�25���ַ��ǻ��з� '/n'
		// ������ͼ���н�������ʾ
		// �ʽ���ȡ p ��ǰ 24 ���ַ�
		for (int i = 0; i < 24; i++)
		{
			timestr[i] = *p;
			p++;
		}
		p = NULL;

		// ��ÿ����ͼ������ʾ����ͷ����Լ�ϵͳʱ����Ϣ
		//cvPutText(pFrame1, cam1str, cvPoint(95, 15), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(pFrame2, cam2str, cvPoint(95, 15), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(frame1, cam1str, cvPoint(95, 15), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(frame2, cam2str, cvPoint(95, 15), &tFont, CV_RGB(255, 0, 0));

		//cvPutText(pFrame1, timestr, cvPoint(5, 225), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(pFrame2, timestr, cvPoint(5, 225), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(frame1, timestr, cvPoint(5, 225), &tFont, CV_RGB(255, 0, 0));
		//cvPutText(frame2, timestr, cvPoint(5, 225), &tFont, CV_RGB(255, 0, 0));

		// ��ʾʵʱ������ͷ��Ƶ
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

	camera1.CloseCamera(); //�ɲ����ô˺�����CCameraDS����ʱ���Զ��ر�����ͷ
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
	//cap.open("d:\\work\\4.mp4");//�Ǻǣ�����һ��ؼ�
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
//		camera1.retrieve(frame1, 0); //�����õ��������ǿյģ�
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
