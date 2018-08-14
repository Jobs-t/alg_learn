// face.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <dlib/opencv.h>  
#include <opencv2/opencv.hpp>  
#include <dlib/image_processing/frontal_face_detector.h>  
#include <dlib/image_processing/render_face_detections.h>  
#include <dlib/image_processing.h>  
#include <dlib/gui_widgets.h>  

using namespace dlib;
using namespace std;

int main()
{

	frontal_face_detector detector = get_frontal_face_detector();
	shape_predictor pose_model;
	deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;


	// Grab a frame  
	cv::Mat temp = imread("D:\\opencv_leane\\image\\lena.jpg");


	cv_image<bgr_pixel> cimg(temp);
	// Detect faces   
	std::vector<rectangle> faces = detector(cimg);
	// Find the pose of each face.  
	std::vector<full_object_detection> shapes;
	for (unsigned long i = 0; i < faces.size(); ++i)
		shapes.push_back(pose_model(cimg, faces[i]));

	if (!shapes.empty()) {
		for (int i = 0; i < 68; i++) {
			circle(temp, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 3, cv::Scalar(0, 0, 255), -1);
			//  shapes[0].part(i).x();//68个  
		}
	}
	//Display it all on the screen  
	imshow("Dlib特征点", temp);



}
