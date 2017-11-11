#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

vector<cv::Point> FindBigestContour(Mat src)
{
	int imax = 0; //代表最大轮廓的序号
	int imaxcontour = -1; //代表最大轮廓的大小
	std::vector<std::vector<cv::Point>>contours;
	findContours(src, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i<contours.size(); i++)
	{
		int itmp = contourArea(contours[i]);//这里采用的是轮廓大小
		if (imaxcontour < itmp)
		{
			imax = i;
			imaxcontour = itmp;
		}
	}
	return contours[imax];
}

int main3()
{

	string src1 = "D:\\opencv_leane\\image\\kdqu.png";
	Mat src_img = imread(src1);
	if (src_img.empty()) {
		cout<<"the image :"<< src1 << "open failed" <<endl;
	}
	Mat src = src_img.clone();
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	
	resize(src, src, Size(src.cols, src.rows));
	//resize(src, src, static_cast<Size>(nullptr), 6, 6, INTER_LINEAR);
	Mat hsv;

	GaussianBlur(src, src, Size(3, 3), 3);
	cvtColor(src, hsv, COLOR_BGR2HSV);
	vector<Mat> hsvSplit;
	split(hsv, hsvSplit);
	
	imshow("h", hsvSplit[0]);
	imshow("s", hsvSplit[1]);
	imshow("v", hsvSplit[2]);
	Mat h;
	threshold(hsvSplit[2],h, 100, 255, THRESH_BINARY_INV);
	imshow("h2", h);

	vector<std::vector<Point>> contours;
	findContours(h, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		if (contourArea(contours[i]) < 200) {
			drawContours(h, contours, i, Scalar(0, 0, 0), CV_FILLED);
		}
	}
	
	imshow("h32", h);
	
	//waitKey();
	//return 1;
	Mat dstclone = h.clone();
	vector<int>vectorV;
	for (int i = 0; i < h.cols; i++) {
		Mat data = h.col(i);
		int itmp = countNonZero(data);
		vectorV.push_back(itmp);
	}
	vector<int> up, down;

	for (int i = 0; i < vectorV.size()-1; i++) {
		if (vectorV[i] > 0 && vectorV[i + 1] == 0) {
			down.push_back(i);
			
		}
		if (vectorV[i] == 0 && vectorV[i + 1] > 0) {
			up.push_back(i);

		}
	}
	for (int i = 0; i < up.size(); i++) {
		if (down[i] - up[i] > 10) {
			line(dstclone, cv::Point(up[i], 0), Point(up[i], h.cols), Scalar(255));
			line(dstclone, cv::Point(down[i], 0), Point(down[i], h.cols), Scalar(255));
		}
		//else {
		//	up[i] = -1;
		//	down[i] = -1;
		//}
	}
	Mat ostu;
	vector<int> os_up, os_down,vectorL,code;
	for(int i = 0; i < up.size(); i++) 
	{
		Mat roitmp = h(Rect(up[i], 0, down[i] - up[i], h.rows));
		string name ="";
		stringstream ss;
		ss << i;
		ss << ".jpg";
		ss >> name;
		
		imwrite(name, roitmp);
		
		GaussianBlur(roitmp, roitmp, Size(3, 3), 3);
		for (int j = 0; j < h.rows; j++)
		{
			Mat data = roitmp.row(j);
			int sum = countNonZero(data);
			vectorL.push_back(sum);
		}
		int whtile = 0;
		for (int n = 0; n < vectorL.size()-1; n++)
		{
			if ((vectorL[n] == 0 && vectorL[n + 1] > 0) || (vectorL[n] >0 && vectorL[n + 1] == 0)) {
				whtile++;
			}
	
		}
		int code_velue = 0;
		if (whtile >= 4) {
			code_velue = 0;
			code.push_back(code_velue);
		}else {
			code_velue = 1;
			code.push_back(code_velue);
		}
		vectorL.clear();
		vectorL.swap(vector<int>());
	}
	for (int i = 0; i < code.size(); i++)
	{
		cout << code[i] << endl;
	}

	imshow("home2", dstclone);


	

	waitKey();
	return 0;

}
