////
//#include "opencv2\opencv.hpp"
#include "stdafx.h"
//#include "tools.h"
//using namespace std;
//using namespace cv; 
//
//
//Rect DrawXYProjection(const Mat image, Mat &imageOut, const int threshodValue, const int binaryzationValue)
//{
//	Mat img = image.clone();
//	if (img.channels()>1)
//	{
//		cvtColor(img, img, CV_RGB2GRAY);
//	}
//	Mat out(img.size(), img.type(), Scalar(255));
//	imageOut = out;
//	//��ÿһ�������ͼƬ���Ҷȹ�һ�����Ա�ʹ��ͬһ����ֵ����  
//	normalize(img, img, 0, 255, NORM_MINMAX);
//	vector<int> vectorVertical(img.cols, 0);
//	for (int i = 0; i<img.cols; i++)
//	{
//		for (int j = 0; j<img.rows; j++)
//		{
//			if (img.at<uchar>(j, i)<binaryzationValue)
//			{
//				vectorVertical[i]++;
//			}
//		}
//	}
//	//��ֵ��һ��  
//	int high = img.rows / 6;
//	normalize(vectorVertical, vectorVertical, 0, high, NORM_MINMAX);
//	for (int i = 0; i<img.cols; i++)
//	{
//		for (int j = 0; j<img.rows; j++)
//		{
//			if (vectorVertical[i]>threshodValue)
//			{
//				line(imageOut, Point(i, img.rows), Point(i, img.rows - vectorVertical[i]), Scalar(0));
//			}
//		}
//	}
//	//ˮƽͶӰ  
//	vector<int> vectorHorizontal(img.rows, 0);
//	for (int i = 0; i<img.rows; i++)
//	{
//		for (int j = 0; j<img.cols; j++)
//		{
//			if (img.at<uchar>(i, j)<binaryzationValue)
//			{
//				vectorHorizontal[i]++;
//			}
//		}
//	}
//	normalize(vectorHorizontal, vectorHorizontal, 0, high, NORM_MINMAX);
//	for (int i = 0; i<img.rows; i++)
//	{
//		for (int j = 0; j<img.cols; j++)
//		{
//			if (vectorHorizontal[i]>threshodValue)
//			{
//				line(imageOut, Point(img.cols - vectorHorizontal[i], i), Point(img.cols, i), Scalar(0));
//			}
//		}
//	}
//	//�ҵ�ͶӰ�ĸ��ǵ�����  
//	vector<int>::iterator beginV = vectorVertical.begin();
//	vector<int>::iterator beginH = vectorHorizontal.begin();
//	vector<int>::iterator endV = vectorVertical.end() - 1;
//	vector<int>::iterator endH = vectorHorizontal.end() - 1;
//	int widthV = 0;
//	int widthH = 0;
//	int highV = 0;
//	int highH = 0;
//	while (*beginV<threshodValue)
//	{
//		beginV++;
//		widthV++;
//	}
//	while (*endV<threshodValue)
//	{
//		endV--;
//		widthH++;
//	}
//	while (*beginH<threshodValue)
//	{
//		beginH++;
//		highV++;
//	}
//	while (*endH<threshodValue)
//	{
//		endH--;
//		highH++;
//	}
//	//ͶӰ����  
//	Rect rect(widthV, highV, img.cols - widthH - widthV, img.rows - highH - highV);
//	return rect;
//}
//
