#include "stdafx.h"
#include "opencv2/opencv.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace cv;
using namespace std;

Point Center(vector<vector<Point> > contours, int i)
{
	int centerx = 0, centery = 0, n = contours[i].size();
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}
int main12(int argc, char** argv[])
{
	Mat src = imread("F:/BaiduNetdiskDownload/第8课（加密）/L8 基于轮廓分析进行二维码定位/L8 基于轮廓分析进行二维码定位/qrcode.jpg", 1);
	resize(src, src, Size(800, 600));//标准大小
	Mat src_gray;
	Mat src_all = src.clone();
	Mat threshold_output;
	vector<vector<Point> > contours, contours2;
	vector<Vec4i> hierarchy;
	//预处理
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3)); //模糊，去除毛刺
	threshold(src_gray, threshold_output, 100, 255, THRESH_OTSU);
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));
	//轮廓筛选
	int c = 0, ic = 0, area = 0;
	int parentIdx = -1;
	for (int i = 0; i< contours.size(); i++)
	{
		drawContours(threshold_output, contours, i, Scalar(round(255), round(255), round(255)));
		//hierarchy[i][2] != -1 表示不是最外面的轮廓
		if (hierarchy[i][2] != -1 && ic == 0)
		{
			parentIdx = i;
			ic++;
		}
		else if (hierarchy[i][2] != -1)
		{
			ic++;
		}
		//最外面的清0
		else if (hierarchy[i][2] == -1)
		{
			ic = 0;
			parentIdx = -1;
		}
		//找到定位点信息
		if (ic >= 2)
		{
			contours2.push_back(contours[parentIdx]);
			ic = 0;
			parentIdx = -1;
		}
	}
	//填充定位点
	for (int i = 0; i<contours2.size(); i++)
		drawContours(src_all, contours2, i, CV_RGB(0, 255, 0), -1);
	//连接定位点
	Point point[3];
	for (int i = 0; i<contours2.size(); i++)
	{
		point[i] = Center(contours2, i);
	}

	line(src_all, point[0], point[1], Scalar(0, 0, 255), 2);
	line(src_all, point[1], point[2], Scalar(0, 0, 255), 2);
	line(src_all, point[0], point[2], Scalar(0, 0, 255), 2);

	imshow("img", src_all);
	imshow("img", threshold_output);
	waitKey(0);
	return(0);
}