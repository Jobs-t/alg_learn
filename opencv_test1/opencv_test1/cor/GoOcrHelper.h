//文件名称:GoOcrHelper.cpp
//作    用: ocr识别辅助函数体
//修    改：
//2014年12月7日12:33:30 创建
#include "../stdafx.h"
 
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../public/include/tesseract/baseapi.h"
#include "../public/include/tesseract/basedir.h"
#include "../public/include/tesseract/strngs.h"
 
#include "BrowseDir.h"
#include "StatDir.h"
using namespace cv;
using namespace std;

void MyPolygon( Mat img );
//函数名称:ConvertUtf8ToGBK 
//函数作用:将utf8格式转换为GBK的格式
//参    数： amp     [ot]   输出
//           strUtf8 [in]   输入
void ConvertUtf8ToGBK(char **amp,char *strUtf8);


//函数名称:read_img 
//函数作用:批量读取图片
//参    数： dir     [in]   目录
//返    回： vector<pair<char*,Mat>>，其中first为绝对路径，second为mat
vector<pair<char*,Mat>>  read_img(const string& dir);

//函数名称:read_csv 
//函数作用:通过读取csv(txt)文件，批量获得文件的名称和其对应的no/crash/account
//参    数： filename                   [in]   csv(txt)文件所在的位置
//          vector<string>& VectorPath  [ot]   图片文件绝对路径
//          vector<string>& VectorNumber [ot]   no
//          vector<string>& VectorAccount [ot]  account
//          vector<string>& VectorCrash    [ot]  crash
//          vchar separator =';'         [in]    分割符号
//返    回： void
void read_csv(const string& filename, vector<string>& VectorPath, vector<string>& VectorNumber,vector<int>& CheckingNumber, vector<string>& VectorAccount,vector<int>& CheckingAccount,vector<string>& VectorCrash,vector<int>& CheckingCrash,char separator=';');

//函数名称:ClearBlank 
//函数作用:去除char中的空格
//参    数： **pResult     [in ot]   输入串,输出串
//          len            [in]   阶段长度
//返    回： void
void ClearBlank(char **pResult,int len = 0);

//函数名称：FetchMaxContour
//函数作用: 返回最大的连续区域
//参    数: src [in] 输入mat
//返    回：处理后的mat
Mat FetchMaxContour(Mat src);

//函数名称:FormatCrash 
//函数作用:按照钱的格式处理钱
//参    数： **pResult     [in ot]   输入串,输出串
//          len            [in]   阶段长度
//返    回： void
void FormatCrash(char **pResult);

void FormatNo(char **pResult);
