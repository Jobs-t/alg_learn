#include "stdafx.h"
//#define  __STDC_LIMIT_MACROS
////#include "zbar.h"        
///*#include "opencv2\opencv.hpp"  */      
////#include "opencv2\highgui.hpp"        
//#include <dlib/opencv.h>
//#include <dlib/dnn.h>
//#include <dlib/data_io.h>
//#include <opencv2/dnn.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
//
//
//#include <fstream>
//#include <iostream>
//#include <Windows.h>
//#include <numeric>
//#include <time.h>
//#include<memory>
//
//using namespace dlib;
//using namespace cv;
//using namespace cv::dnn;
//using namespace std;
////using namespace zbar;  //添加zbar名称空间      
//
//template <long num_filters, typename SUBNET> using con5d = con<num_filters, 5, 5, 2, 2, SUBNET>;
//template <long num_filters, typename SUBNET> using con5 = con<num_filters, 5, 5, 1, 1, SUBNET>;
//
//template <typename SUBNET> using downsampler = relu<affine<con5d<32, relu<affine<con5d<32, relu<affine<con5d<16, SUBNET>>>>>>>>>;
//template <typename SUBNET> using rcon5 = relu<affine<con5<45, SUBNET>>>;
//
//using net_type = loss_mmod<con<1, 9, 9, 1, 1, rcon5<rcon5<rcon5<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;
//
//// function to create vector of class names
//std::vector<String> createClaseNames() {
//	std::vector<String> classNames;
//	classNames.push_back("background");
//	classNames.push_back("aeroplane");
//	classNames.push_back("bicycle");
//	classNames.push_back("bird");
//	classNames.push_back("boat");
//	classNames.push_back("bottle");
//	classNames.push_back("bus");
//	classNames.push_back("car");
//	classNames.push_back("cat");
//	classNames.push_back("chair");
//	classNames.push_back("cow");
//	classNames.push_back("diningtable");
//	classNames.push_back("dog");
//	classNames.push_back("horse");
//	classNames.push_back("motorbike");
//	classNames.push_back("person");
//	classNames.push_back("pottedplant");
//	classNames.push_back("sheep");
//	classNames.push_back("sofa");
//	classNames.push_back("train");
//	classNames.push_back("tvmonitor");
//	return classNames;
//}
////prepare the sounds options for attracting the dogs
//std::vector<LPCWSTR> createSoundNames() {
//	std::vector<LPCWSTR> sounds;
//	sounds.push_back(L"cat.wav");
//	sounds.push_back(L"cat2.wav");
//	sounds.push_back(L"squeaky_toy.wav");
//	sounds.push_back(L"one_squeak.wav");
//	return sounds;
//}
//
//// dog detector
//int dogDetector(Mat &img, Size imgSize, Net net, std::vector<String> classNames) {
//
//	// create input blob
//	Mat img300;
//	resize(img, img300, Size(300, 300));
//	Mat inputBlob = blobFromImage(img300, 0.007843, Size(300, 300), Scalar(127.5)); //Convert Mat to dnn::Blob image batch
//
//																					// apply the blob on the input layer
//	net.setInput(inputBlob); //set the network input
//
//							 // classify the image by applying the blob on the net
//	Mat detections = net.forward("detection_out"); //compute output
//
//												   // look what the detector found
//	int nrDog = 0;
//	for (int i = 0; i < detections.size[2]; i++) {
//
//		// detected class
//		int indxCls[4] = { 0, 0, i, 1 };
//		int cls = detections.at<float>(indxCls);
//
//		// confidence
//		int indxCnf[4] = { 0, 0, i, 2 };
//		float cnf = detections.at<float>(indxCnf);
//
//		// mark with bbox only dogs
//		if (cls == 12 && cnf > 0.3) {
//			// count the dog
//			nrDog = nrDog++;
//			// bounding box
//			int indxBx[4] = { 0, 0, i, 3 };
//			int indxBy[4] = { 0, 0, i, 4 };
//			int indxBw[4] = { 0, 0, i, 5 };
//			int indxBh[4] = { 0, 0, i, 6 };
//			int Bx = detections.at<float>(indxBx) * imgSize.width;
//			int By = detections.at<float>(indxBy) * imgSize.height;
//			int Bw = detections.at<float>(indxBw) * imgSize.width - Bx;
//			int Bh = detections.at<float>(indxBh) * imgSize.height - By;
//
//			// draw bounding box to image
//			Rect bbox(Bx, By, Bw, Bh);
//			Scalar color(255, 0, 255);
//			cv::rectangle(img, bbox, color, 1, 8, 0);
//			String text = classNames[cls] + ", conf: " + to_string(cnf * 100);
//			putText(img, text, Point(Bx, By), FONT_HERSHEY_SIMPLEX, 0.5, color);
//		}
//
//	}
//
//	return nrDog;
//}
//
//
//// count euclidean distance between two points
//float euclideanDist(Point& p, Point& q) {
//	Point diff = p - q;
//	return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
//}
//
//// Check the head position
//bool checkHeadPosition(const dlib::full_object_detection& landmarks)
//{
//	// maximum difference in the distances
//	float maxError = 0.3;
//
//	//distance between left eye and nose and right eye and nose should be nearly the same
//	float diffLeye = euclideanDist(cv::Point(landmarks.part(3).x(), landmarks.part(3).y()), cv::Point(landmarks.part(5).x(), landmarks.part(5).y()));
//	float diffReye = euclideanDist(cv::Point(landmarks.part(3).x(), landmarks.part(3).y()), cv::Point(landmarks.part(2).x(), landmarks.part(2).y()));
//	float meanEyeDist = (diffReye + diffLeye) / 2;
//	// if the distance difference is bigger than 20% of its value return false
//	if ((diffLeye - diffReye) > meanEyeDist * maxError || (diffLeye - diffReye) < -1 * meanEyeDist * maxError) {
//		return false;
//	}
//
//	//distance between left ear and nose and right ear and nose should be nearly the same
//	float diffLear = euclideanDist(cv::Point(landmarks.part(3).x(), landmarks.part(3).y()), cv::Point(landmarks.part(4).x(), landmarks.part(4).y()));
//	float diffRear = euclideanDist(cv::Point(landmarks.part(3).x(), landmarks.part(3).y()), cv::Point(landmarks.part(1).x(), landmarks.part(1).y()));
//	float meanEarDist = (diffLear + diffRear) / 2;
//	// if the distance difference is bigger than 20% of its value return false
//	if ((diffLear - diffRear) > meanEarDist * maxError || (diffLear - diffRear) < -1 * meanEarDist * maxError) {
//		return false;
//	}
//
//	// if all distances are equal return true
//	return true;
//
//}
//
//// Draw dog face
//void renderFace(cv::Mat &img, const dlib::full_object_detection& landmarks, cv::Scalar color)
//{
//	// save the point into vector
//	std::vector <cv::Point> points;
//	points.push_back(cv::Point(landmarks.part(0).x(), landmarks.part(0).y())); //top
//	points.push_back(cv::Point(landmarks.part(1).x(), landmarks.part(1).y())); //rear
//	points.push_back(cv::Point(landmarks.part(2).x(), landmarks.part(2).y())); //reye
//	points.push_back(cv::Point(landmarks.part(3).x(), landmarks.part(3).y())); //nose
//	points.push_back(cv::Point(landmarks.part(5).x(), landmarks.part(5).y())); //leye
//	points.push_back(cv::Point(landmarks.part(4).x(), landmarks.part(4).y())); //lear
//
//																			   // draw head
//	cv::polylines(img, points, true, color, 1, 16);
//}
//
////// function to play the sound
////void playRndSound(std::vector<LPCWSTR> sounds)
////{
////	// choose rondomly the sound and play it
////	std::srand(time(NULL));
////	int s = std::rand() % sounds.size();
////	PlaySound(sounds[s], NULL, SND_ASYNC);
////}
//
//
//
//
//// find dog's head and check dog smile
//int dogSmileDetector(Mat &img, net_type netHead, shape_predictor landmarkDetector) {
//
//	// convert OpenCV image to Dlib's cv_image object, then to Dlib's matrix object
//	Mat imRGB;
//	cv::cvtColor(img, imRGB, cv::COLOR_BGR2RGB);
//	dlib::matrix<dlib::rgb_pixel> imDlib(dlib::mat(dlib::cv_image<dlib::rgb_pixel>(imRGB)));
//
//	//detect dog heads in image
//	std::vector<dlib::mmod_rect>faceRects = netHead(imDlib);
//
//	// Loop over all detected face rectangles
//	int headPositionOk = 0;
//	for (int i = 0; i < faceRects.size(); i++)
//	{
//		// For every face rectangle, run landmarkDetector
//		full_object_detection landmarks = landmarkDetector(imDlib, faceRects[i].rect);
//
//		// Check head position
//		if (checkHeadPosition(landmarks)) {
//			// Draw landmarks on face (green)
//			renderFace(img, landmarks, cv::Scalar(0, 255, 0));
//			// The head position is OK, so end the loop and continue
//			headPositionOk = headPositionOk++;
//		}
//		else {
//			// Draw landmarks on face (red)
//			renderFace(img, landmarks, cv::Scalar(0, 0, 255));
//		}
//	}
//
//	return headPositionOk;
//
//}
//void detectdog()
//{
//	// set inputs
//	String pathNetDogTxt("MobileNetSSD_deploy.prototxt");
//	String pathNetDogBin("MobileNetSSD_deploy.caffemodel");
//	String pathNetHead("dogHeadDetector.dat");
//	String pathLandmarkDetector("landmarkDetector.dat");
//	std::vector<String> classNames = createClaseNames();
//	std::vector<LPCWSTR> sounds = createSoundNames();
//
//	//read all models
//	Net netDog;
//	net_type netHead;
//	shape_predictor landmarkDetector;
//	try {
//		//read caffe model
//		netDog = readNetFromCaffe(pathNetDogTxt, pathNetDogBin);
//		// Load the dog head detector
//		deserialize(pathNetHead) >> netHead;
//		// Load landmark model
//		deserialize(pathLandmarkDetector) >> landmarkDetector;
//	}
//	catch (cv::Exception& e) {
//		//cerr << "Exception: " << e.what() << std::endl;
//		//if (netDog.empty())
//		//{
//		//	cerr << "Can't load caffe model." << std::endl;
//		//	exit(-1);
//		//}
//	}
//
//	//// Create a VideoCapture object
//	//cv::VideoCapture cap(0);
//	//// Check if OpenCV is able to read feed from camera
//	//if (!cap.isOpened())
//	//{
//	//	cerr << "Unable to connect to camera" << endl;
//	//	cv::waitKey(0);
//	//}
//	//// define Mat 
//	Mat img = imread("dog.jpg");
//
//	//// grab first image and get its size
//	//cap >> img;
//	Size imgSize = img.size();
//
//	// Grab and process frames until the main window is closed by the user.
//	bool state = true;
//	int frameCount = 5;
//	int dogInFrameCount = 0;
//	int nrDogs, nrDogsPrev, headPositionOk;
//	while (state) {
//
//		//// grab frame
//		//cap >> img;
//
//		//// if the enought time elapsed and number of dogs did not changed in last 5 frames play sound
//		//if (frameCount > 70 && dogInFrameCount >= 4) {
//		//	// play sound
//		//	playRndSound(sounds);
//		//	// detect dogs in frame
//		nrDogs = dogDetector(img, imgSize, netDog, classNames);
//		//	// restart frame count
//		//	frameCount = 0;
//		//}
//		//// exactly 4 frames after playing the sound, check that the number of dogs in frame did not changed and take picture and analyze
//		//if (frameCount == 4 && dogInFrameCount >= 4) {
//		//create the copy of frame
//		//Mat imgCopy = img.clone();
//
//		//// detect dogs in frame
//		//nrDogs = dogDetector(img, imgSize, netDog, classNames);
//
//		// detect dogs head and the landmarks in frame
//		headPositionOk = dogSmileDetector(img, netHead, landmarkDetector);
//
//		// create window with output image with displayed dog rectangle and landmarks
//		String winName("Dog and Face detector output");
//		imshow(winName, img);
//
//		// print results
//		cout << "---------------------" << endl;
//		cout << "nrDogs: " << nrDogs << endl;
//		cout << "headPositionOk: " << headPositionOk << endl;
//
//		// check if is the number of dogs found and the number of head positions Ok the same
//		//if (nrDogs == headPositionOk) {
//
//		//	// display taken picture
//		//	String winName2("Taken foto OK");
//		//	imshow(winName2, imgCopy);
//		//	// save image
//		//	String fileName("output.jpg");
//		//	imwrite(fileName, imgCopy);
//		//	// wait for keypress
//		//	waitKey();
//		//	// break loop
//		//	state = false;
//		//}
//
//		// restart frameCount
//		//frameCount = 5;
//		//}
//		//else {
//
//		//	// detect dogs in frame
//		//	nrDogs = dogDetector(img, imgSize, netDog, classNames);
//
//		//}
//
//		//// increase frameCount
//		//frameCount = frameCount++;
//
//		//// chceck if the number of dogs detected in last and current frame changed
//		//if (nrDogsPrev != nrDogs) {
//		//	nrDogsPrev = nrDogs;
//		//	dogInFrameCount = 0;
//		//}
//		//else {
//		//	dogInFrameCount = dogInFrameCount++;
//		//}
//
//		// show image
//		/*std::String winName("image");*/
//		imshow("image", img);
//
//		// Wait for keypress
//		char key = cv::waitKey(1);
//		//if (key == 27) // ESC
//		//{
//		//	// If ESC is pressed, exit.
//		//	state = false;
//		//}
//
//	}
//
//	//destroy all windows and quit
//	destroyAllWindows();
//
//
//}
////
////int main2(int argc, char*argv[])
////{
////	ImageScanner scanner;
////	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
////	Mat image = imread("d:\\opencv_leane\\image\\barcode.png");
////	if (!image.data)
////	{
////		cout << "请确认图片" << endl;
////		system("pause");
////		return 0;
////		
////	}
////	Mat imageGray;
////	cvtColor(image, imageGray, CV_RGB2GRAY);
////	int width = imageGray.cols;
////	int height = imageGray.rows;
////	uchar *raw = (uchar *)imageGray.data;
////	Image imageZbar(width, height, "Y800", raw, width * height);
////	scanner.scan(imageZbar); //扫描条码      
////	Image::SymbolIterator symbol = imageZbar.symbol_begin();
////	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
////	{
////		cout << "查询条码失败，请检查图片！" << endl;
////	}
////	for (; symbol != imageZbar.symbol_end(); ++symbol)
////	{
////		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
////		cout << "条码：" << endl << symbol->get_data() << endl << endl;
////	}
////	imshow("Source Image", image);
////	waitKey();
////	imageZbar.set_data(NULL, 0);
////	return 0;
////}
////
////int main5() 
////{
////	Mat image, imageGray, imageGaussian;
////	Mat imageSobelX, imageSobelY, imageSobelOut;
////	image = imread("d:\\opencv_leane\\image\\2.jpg");
////	
////	/*resize(image, image, Size(300, 300));*/
////	imshow("images", image);
////	
////	cvtColor(image, imageGray, CV_BGR2GRAY);
////	
////	GaussianBlur(imageGray, imageGaussian, Size(3,3), 0);
////	
////	//get gredient in x, y direction
////	Mat imageX16S, imageY16S;
////	Sobel(imageGaussian, imageX16S, CV_16S, 1, 0, 3, 1, 0, 4);
////	Sobel(imageGaussian, imageY16S, CV_16S, 0, 1, 3, 1, 0, 4);
////	convertScaleAbs(imageX16S, imageSobelX, 1, 0);
////	convertScaleAbs(imageY16S, imageSobelY, 1, 0);
////	imageSobelOut = imageSobelX - imageSobelY;
////	imshow("4.X方向梯度", imageSobelX);
////	imshow("4.Y方向梯度", imageSobelY);
////	imshow("4.XY方向梯度差", imageSobelOut);
////
////	blur(imageSobelOut, imageSobelOut, Size(3, 3));
////	imshow("blur", imageSobelOut);
////	Mat imageSobleOutThreshold;
////	threshold(imageSobelOut, imageSobleOutThreshold, 180, 255, CV_THRESH_BINARY);
////	imshow("6.二值化", imageSobleOutThreshold);
////	
////	Mat element = getStructuringElement(0, Size(7, 7));
////	morphologyEx(imageSobleOutThreshold, imageSobleOutThreshold, MORPH_CLOSE, element);
////	imshow("7.闭运算", imageSobleOutThreshold);
////
////	//8. 腐蚀，去除孤立的点    
////	erode(imageSobleOutThreshold, imageSobleOutThreshold, element);
////	imshow("8.腐蚀", imageSobleOutThreshold);
////
////	//9. 膨胀，填充条形码间空隙，根据核的大小，有可能需要2~3次膨胀操作    
////	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
////	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
////	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
////	imshow("9.膨胀", imageSobleOutThreshold);
////	std::vector<std::vector<Point>> contours;
////	std::vector<cv::Vec4i> hiera;
////	//10.通过findContours找到条形码区域的矩形边界    
////	findContours(imageSobleOutThreshold, contours, hiera, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
////	for (int i = 0; i<contours.size(); i++)
////	{
////		Rect rect = boundingRect((Mat)contours[i]);
////		cv::rectangle(image, rect, Scalar(255), 2);
////	}
////	imshow("10.找出二维码矩形区域", image);
////	waitKey();
////	return 1;
////	
////}
////void houghfindcircle() {
////	Mat src = imread("d:\\opencv_leane\\image\\circle3.jpg");
////	GaussianBlur(src, src, Size(9, 9), 2, 2);
////	Mat src_gray;
////	cvtColor(src, src_gray, CV_BGR2GRAY);
////	std::vector<cv::Vec3f> circles;
////
////	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, src.rows / 8, 200, 100, 0, 0);
////	for (size_t i = 0; i < circles.size(); i++)
////	{
////		Point  center(cvRound(circles[i][0]), cvRound(circles[i][1]));
////		int radius = cvRound(circles[i][2]);
////		circle(src, center, 3, Scalar(255, 150, 156), -1, 8, 0);
////		circle(src, center, radius, Scalar(150, 150, 122), 4, 8, 0);
////
////	}
////	imshow("hough", src);
////	waitKey();
////}
////
////
////
////void findcircle() 
////{
////	Mat src = imread("d:\\opencv_leane\\image\\circle3.jpg");
////	GaussianBlur(src, src, Size(9, 9), 2, 2);
////	Mat gray,hsv,tmp,dst;
////	std::vector<Mat> hsvSplit;
////	cvtColor(src, hsv, CV_BGR2HSV);
////	split(hsv, hsvSplit);
////	threshold(hsvSplit[2], tmp, 100, 255, CV_THRESH_OTSU);
////	std::vector<std::vector<cv::Point>> contours;
////	
////	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5,5));
////	morphologyEx(tmp, tmp, CV_MOP_CLOSE, element);
////	//erode(tmp, tmp, element, Point(-1, -1), 3);
////	imshow("grag", tmp);
////	findContours(tmp, contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
////	int max = 0;
////	int max_sub = 0;
////
////	for (int i = 0; i < contours.size(); i++) 
////	{
////		int eara = contourArea(contours[i]);
////		if (max < eara) {
////			max = eara;
////			max_sub = i;		
////		}
////	}
////	Point2f center;
////	float radius = 0;
////	float fcompare = 0;
////	float dsum = 0;
////		
////	minEnclosingCircle((contours[max_sub]), center, radius);
////	for (int j = 0; j < contours[max_sub].size(); j++)
////	{
////		Point2f ptmp = contours[max_sub][j];
////		float fdistenct = sqrt((float)((ptmp.x - center.x)*(ptmp.x - center.x) + (ptmp.y - center.y)*(ptmp.y - center.y)));
////		float fdiff = abs(fdistenct - radius);
////			dsum += fdiff;
////	}
////	fcompare = dsum / contours[max_sub].size();
////
////	cvtColor(src, gray, CV_BGR2GRAY);
////	Mat paiter = Mat(tmp.size(), src.type());
////	drawContours(paiter, contours, max_sub, Scalar(250, 150, 156),4);
////	imshow("src", paiter);
////	waitKey();
////	
////}
////
////Mat mosaic() 
////{
////	Mat img;
////	return img;
////}
////
////int reg_pipe() {
////	Mat src = imread("d:\\opencv_leane\\image\\circle3.jpg");
////	std::vector<std::vector<Point2f>> contours;
////	std::vector<std::vector<Point2f>> select_contours;
////	Mat gray;
////	cvtColor(src, gray, CV_BayerRG2GRAY);
////	threshold(gray, gray, 100, 255, THRESH_BINARY);
////	bitwise_not(gray, gray);
////	findContours(gray, contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
////	for (int i = 0; i < contours.size(); i++)
////	{
////		if (contours[i].size() > 8, contours[i].size() < 88)
////		{
////			select_contours.push_back(contours[i]);
////
////		}
////	}
////	int size_contours = select_contours.size();
////	std::vector<cv::RotatedRect> minEllipse(size_contours);
////
////	for (int i = 0; i < select_contours.size(); i++)
////	{
////		
////	}
////	return 0;
////	
////}
//int main4() {
//	detectdog();
//	return 0;
//}
//
////#include <dlib/gui_widgets.h>
////#include <dlib/image_io.h>
////#include <dlib/image_transforms.h>
////#include <fstream>
////
////
////using namespace std;
////using namespace dlib;
////
//////  ----------------------------------------------------------------------------
////
////int main(int argc, char** argv)
////{
////	try
////	{
////
////		// 声明图像
////		array2d<rgb_pixel> img;
////
////		string img_path = "dog.jpg";
////		load_image(img, img_path);
////
////		// 高斯模糊
////		array2d<unsigned char> blurred_img;
////		gaussian_blur(img, blurred_img);
////
////		// sobel边缘检测
////		array2d<short> horz_gradient, vert_gradient;
////		array2d<unsigned char> edge_image;
////		sobel_edge_detector(blurred_img, horz_gradient, vert_gradient);
////
////		//非极大边缘抑制
////		suppress_non_maximum_edges(horz_gradient, vert_gradient, edge_image);
////
////		// 显示结果
////		image_window my_window(edge_image, "Normal Edge Image");
////
////		// We can also easily display the edge_image as a heatmap or using the jet color
////		// scheme like so.
////		image_window win_hot(heatmap(edge_image));
////		image_window win_jet(jet(edge_image));
////
////		// also make a window to display the original image
////		image_window my_window2(img, "Original Image");
////
////		// Sometimes you want to get input from the user about which pixels are important
////		// for some task.  You can do this easily by trapping user clicks as shown below.
////		// This loop executes every time the user double clicks on some image pixel and it
////		// will terminate once the user closes the window.
////		point p;
////		while (my_window.get_next_double_click(p))
////		{
////			cout << "User double clicked on pixel:         " << p << endl;
////			cout << "edge pixel value at this location is: " << (int)edge_image[p.y()][p.x()] << endl;
////		}
////
////		// wait until the user closes the windows before we let the program 
////		// terminate.
////		win_hot.wait_until_closed();
////		my_window2.wait_until_closed();
////
////
////		// Finally, note that you can access the elements of an image using the normal [row][column]
////		// operator like so:
////		cout << horz_gradient[0][3] << endl;
////		cout << "number of rows in image:    " << horz_gradient.nr() << endl;
////		cout << "number of columns in image: " << horz_gradient.nc() << endl;
////	}
////	catch (exception& e)
////	{
////		cout << "exception thrown: " << e.what() << endl;
////	}
////	getchar();
////}


