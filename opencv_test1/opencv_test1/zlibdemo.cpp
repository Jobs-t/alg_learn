#include "stdafx.h"
#include "zbar.h"        
#include "opencv2\opencv.hpp"        
//#include "opencv2\highgui.hpp"        
#include <iostream>        

using namespace std;
using namespace zbar;  //添加zbar名称空间      
using namespace cv;

int main4(int argc, char*argv[])
{
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	Mat image = imread("d:\\opencv_leane\\image\\barcode.png");
	if (!image.data)
	{
		cout << "请确认图片" << endl;
		system("pause");
		return 0;
		
	}
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //扫描条码      
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "查询条码失败，请检查图片！" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
		cout << "条码：" << endl << symbol->get_data() << endl << endl;
	}
	imshow("Source Image", image);
	waitKey();
	imageZbar.set_data(NULL, 0);
	return 0;
}

int main() 
{
	Mat image, imageGray, imageGaussian;
	Mat imageSobelX, imageSobelY, imageSobelOut;
	image = imread("d:\\opencv_leane\\image\\2.jpg");
	
	/*resize(image, image, Size(300, 300));*/
	imshow("images", image);
	
	cvtColor(image, imageGray, CV_BGR2GRAY);
	
	GaussianBlur(imageGray, imageGaussian, Size(3,3), 0);
	
	//get gredient in x, y direction
	Mat imageX16S, imageY16S;
	Sobel(imageGaussian, imageX16S, CV_16S, 1, 0, 3, 1, 0, 4);
	Sobel(imageGaussian, imageY16S, CV_16S, 0, 1, 3, 1, 0, 4);
	convertScaleAbs(imageX16S, imageSobelX, 1, 0);
	convertScaleAbs(imageY16S, imageSobelY, 1, 0);
	imageSobelOut = imageSobelX - imageSobelY;
	imshow("4.X方向梯度", imageSobelX);
	imshow("4.Y方向梯度", imageSobelY);
	imshow("4.XY方向梯度差", imageSobelOut);

	blur(imageSobelOut, imageSobelOut, Size(3, 3));
	imshow("blur", imageSobelOut);
	Mat imageSobleOutThreshold;
	threshold(imageSobelOut, imageSobleOutThreshold, 180, 255, CV_THRESH_BINARY);
	imshow("6.二值化", imageSobleOutThreshold);
	
	Mat element = getStructuringElement(0, Size(7, 7));
	morphologyEx(imageSobleOutThreshold, imageSobleOutThreshold, MORPH_CLOSE, element);
	imshow("7.闭运算", imageSobleOutThreshold);

	//8. 腐蚀，去除孤立的点    
	erode(imageSobleOutThreshold, imageSobleOutThreshold, element);
	imshow("8.腐蚀", imageSobleOutThreshold);

	//9. 膨胀，填充条形码间空隙，根据核的大小，有可能需要2~3次膨胀操作    
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	imshow("9.膨胀", imageSobleOutThreshold);
	vector<vector<Point>> contours;
	vector<Vec4i> hiera;
	//10.通过findContours找到条形码区域的矩形边界    
	findContours(imageSobleOutThreshold, contours, hiera, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i<contours.size(); i++)
	{
		Rect rect = boundingRect((Mat)contours[i]);
		rectangle(image, rect, Scalar(255), 2);
	}
	imshow("10.找出二维码矩形区域", image);
	waitKey();
	return 1;
	
}