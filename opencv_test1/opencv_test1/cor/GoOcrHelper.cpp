//文件名称:GoOcrHelper.cpp
//作    用: ocr识别辅助函数体
//修    改：
//2014年12月7日12:33:30 创建
//2014年12月14日8:29:27 重构投影函数，调整发现min设置为0的时候效果很好，为什么会这样，需要对tesseractocr的代码进行分析
#include "stdafx.h"
#include "GoOcrHelper.h" 
#include <iostream>
#include <fstream>
#include <sstream>
using namespace cv;
using namespace std;
#define IsDebug TRUE 

//函数名称:ConvertUtf8ToGBK 
//函数作用:将utf8格式转换为GBK的格式
//参    数： amp     [ot]   输出
//           strUtf8 [in]   输入
void ConvertUtf8ToGBK(char **amp,char *strUtf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL,0)-3; 
	if (len <0) //2014年12月18日19:16:16修改
	{
		len = 0;
	}
	unsigned short * wszGBK = new unsigned short[len+1]; 
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	//char *szGBK=new char[len + 1]; 
	*amp = new char[len+1];
	memset(*amp, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, *amp, len, NULL,NULL); 
	//amp=szGBK; 
}

 
//函数名称:read_img 
//函数作用:批量读取图片
//参    数： dir     [in]   目录
//返    回： vector<pair<char*,Mat>>，其中first为绝对路径，second为mat
vector<pair<char*,Mat>>  read_img(const string& dir)
{
	CStatDir statdir;
	pair<char*,Mat> pfi;
	vector<pair<char*,Mat>> Vp;
	if (!statdir.SetInitDir(dir.c_str()))
	{
		return Vp;
	}
	int cls_id = dir[dir.length()-1]-'0';
	vector<char*>file_vec = statdir.BeginBrowseFilenames("*.jpg");
	int i,s = file_vec.size();
	for (i=0;i<s;i++)
	{
		pfi.first = file_vec[i];
		pfi.second = imread(file_vec[i]);
		Vp.push_back(pfi);
	}
	return Vp;
}

//函数名称:read_csv 
//函数作用:通过读取csv(txt)文件，批量获得文件的名称和其对应的no/crash/account
//参    数： filename                   [in]   csv(txt)文件所在的位置
//          vector<string>& VectorPath  [ot]   图片文件绝对路径
//          vector<string>& VectorNumber [ot]   no
//          vector<string>& VectorAccount [ot]  account
//          vector<string>& VectorCrash    [ot]  crash
//          vchar separator       [in]    分割符号
//返    回： void
void read_csv(const string& filename, vector<string>& VectorPath, vector<string>& VectorNumber,vector<int>& CheckingNumber, vector<string>& VectorAccount,vector<int>& CheckingAccount,vector<string>& VectorCrash,vector<int>& CheckingCrash,char separator)
{
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message ="csv文件名设置错误!";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, number,account,crash;

	while (getline(file, line)) {

		stringstream liness(line);

		getline(liness, path, separator); //C:\test\4.jpg;
		getline(liness, number,separator); //  A000-90032589;  
		getline(liness, account,separator); // 00101245000000368;
		getline(liness, crash); //  15,000,000.00

		if(!path.empty()&&!number.empty()&&!account.empty()&&!crash.empty()) {
			VectorPath.push_back(path);
			VectorNumber.push_back(number);
			VectorAccount.push_back(account);
			VectorCrash.push_back(crash);
			CheckingAccount.push_back(0);
			CheckingNumber.push_back(0);
			CheckingCrash.push_back(0);
		}
	}
}

//函数名称:ClearBlank 
//函数作用:去除char中的空格
//参    数： **pResult     [in ot]   输入串,输出串
//          len            [in]   阶段长度
//返    回： void
void ClearBlank(char **pResult,int len )
{
	char *p =*pResult;
	char *pResult2 = new char[50];//结果保存
	char *p2 = pResult2;
	int ilen = 0;
	for (int i=0;i<strlen(*pResult);i++)
	{
		if (*p != ' ')
		{
			*p2 = *p;
			ilen = ilen+1;
			p2++;
		}
		p++;
		if (ilen == len && len!=0)
		{
			break;
		}
	}
	*p2 ='\0';//添加结尾
	*pResult = pResult2;
}

//函数名称:FormatCrash 
//函数作用:按照钱的格式处理钱
//参    数： **pResult     [in ot]   输入串,输出串
//          len            [in]   阶段长度
//返    回： void
void FormatCrash(char **pResult )
{
	char *p =*pResult;
	char *pResult2 = new char[50];//结果保存
	char *pResult3 = new char[50];//结果保存
	char *p2 = pResult2;
	char *p3 = pResult3;
	int ilen = 0;
	//首先清除所有 . 和 ,
	for (int i=0;i<strlen(*pResult);i++)
	{
		if (*p == '.' || *p == ',' )
		{
			
		}
		else
		{
			*p2 = *p;
			ilen = ilen+1;
			p2++;
		}
		
		p++;
	
	}
	int ifirst = (ilen-2) % 3; 
	int imode =(ilen-2)/3;
	p2 = pResult2;//从头开始算
	//头两个
	for (int i=0;i<ifirst;i++)
	{
		*p3 = *p2;
		p3++;
		p2++;
	}
	for (int i=0;i<imode;i++)
	{
		//这种情况下要不加
		if (ifirst==0 && i == 0)
		{
			
		}
		else
		{
			*p3 =',';
			p3++;
		}
	

		for (int j=0;j<3;j++)
		{
			*p3 = *p2;
			p3++;
			p2++;
		}
	}
	*p3 = '.';
	p3++;
	*p3 = *p2;
	p3++;
	p2++;
	//*p3 = *p2;
	//2014年12月24日22:38:17 hack
	*p3 = '0';
	p3++;
	p2++;
 
	//p2 -- ;
	//char temp2 = *p2;
	//p2--;
	//char temp1 = *p2;
	//*p2 ='.';
	//p2++;
	//*p2 = temp1;
	//p2++;
	//*p2 = temp2;
	//p2++;
	*p3 ='\0';//添加结尾
	*pResult = pResult3;
}

//函数名称:FormatNo 
//函数作用:按照no的格式进行处理
//参    数： **pResult     [in ot]   输入串,输出串
//返    回： void
void FormatNo(char **pResult)
{
	char *p =*pResult;
 
	//进行 - 的修正
	for (int i=0;i<strlen(*pResult);i++)
	{
		 if (i == 4 && *p != '-')
		 {
			 *p = '-';
			 break;;
		 }
		 p++;
	
	}
	 
}

//函数名称：FetchMaxContour
//函数作用: 对账户区域进行预处理，返回最大的连续区域
//参    数: src [in] 输入mat
//返    回：处理后的mat
Mat FetchMaxContour(Mat src)
{
	//读取图像
	Mat testmat = src.clone();
	Mat testclone = src.clone();
	Mat matcanny;
	//用于寻找轮廓
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int imax = 0;int maxsize = 0;
	RotatedRect theMinRect;
	RotatedRect theMinEllipse;

	//imshow("原始图像",testmat);
	//大津法找到敏感区域
	threshold(testmat,testmat,0,255,cv::THRESH_OTSU); 
	//imshow("大津法",testmat);
	//连续膨胀4次
	erode(testmat,testmat,cv::Mat());
	erode(testmat,testmat,cv::Mat());
	erode(testmat,testmat,cv::Mat());
	erode(testmat,testmat,cv::Mat());
	//imshow("膨胀",testmat);
	//为什么要转换，因为白色是有数据的区域,轮廓是围绕白色区域的
	threshold(testmat,testmat,0,255,THRESH_BINARY_INV);
	//imshow("二值",testmat);
	
	//计算横向投影,横向的对于的是 row,纵向的对于的是col
	vector<int> vrow;int itmp =0;
	for (int i=0;i<testmat.rows;i++)
	{
		for (int j=0;j<testmat.cols;j++)
		{
			if (testmat.at<uchar>(i,j))//我这里就是不管多少了，只要大于0就进入了
			{
				itmp = itmp +1;
			}
		}
		vrow.push_back(itmp);
		itmp = 0;
	}
	//计算纵向投影
	vector<int> vcol;itmp = 0;
	for (int i=0;i<testmat.cols;i++)
	{
		for (int j=0;j<testmat.rows;j++)
		{
			if (testmat.at<uchar>(j,i))
			{
				itmp = itmp +1;
			}
		}
		vcol.push_back(itmp);
		itmp = 0;
	}
	////对得到的投影进行阈值处理
	int iRowMin = 0;int iRowMax = 0;
	int iColMin = 0;int iColMax = 0;
	//认为在横向中，大于200的就是roi区域
	for (int i=0;i<vrow.size();i++)
	{
		if (vrow[i] >200)
		{
			 iRowMin = i;
			 break;
		}
	}
	for (int i=vrow.size()-1;i>=0;i--)
	{
		if (vrow[i] >200)
		{
			iRowMax = i;
			break;
		}
	}
	if (iRowMin - 3 >=0)
	{
		iRowMin =iRowMin -  3;
	}
	else
	{
		iRowMin = 0;
	}
	if (iRowMax+3<=testmat.rows)
	{
		iRowMax = iRowMax+3;
	}
	else
	{
		iRowMax = testmat.rows;
	};
	
	//for (int i=0;i<vcol.size();i++)
	//{
	//	if (vcol[i] >0)
	//	{
	//		jmin = i;
	//		break;
	//	}
	//}
	for (int i=vcol.size()-1;i>=0;i--)
	{
		if (vcol[i] >18)
		{
			iColMax = i;
			break;
		}
	}
	//if (jmin - 3 >=18)
	//{
	//	jmin =jmin -  3;
	//}
	//else
	//{
		iColMin = 0;
	//}
	if (iColMax+7<=testmat.cols)
	{
		iColMax = iColMax+7;
	}
	else
	{
		iColMax = testmat.cols;
	}
	Mat MatRet = testclone(Rect(iColMin,iRowMin,iColMax-iColMin,iRowMax-iRowMin));
	 
	//resize(MatRet,MatRet,Size(MatRet.cols*2,MatRet.rows*2));

	//imshow("MatRec",MatRet);
 //   //找出最大轮廓
	//std::vector<std::vector<cv::Point>>contours;
	//cv::findContours(testmat,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
	////找出最大的contours
	//int icontour = 0; int sizemax = 0;
	//for (int i=0;i<contours.size();i++)
	//{
	//	vector<cv::Point> contourtmp = contours[i];
	//	if (contourtmp.size()>sizemax)
	//	{
	//		sizemax = contourtmp.size();
	//		icontour = i;
	//	}
	//}
	// drawContours(src,contours,icontour,Scalar(0 ),1);
	// imshow("src",src);
	//将其余的洞补起来
	//for (int i=0;i<contours.size();i++)
	//{
	//	if (i!=icontour)
	//	{
	//		vector<cv::Point> contourtmp = contours[i];
	//		Point rook_points[1][100000];
	//		for (int j=0;j<contourtmp.size();j++)
	//		{
	//			rook_points[0][j] = contourtmp[j];
	//		}
	//		const Point* ppt[1] = { rook_points[0] };
	//		int npt[] = { contourtmp.size() };

	//		fillPoly( testclone,
	//			ppt,
	//			npt,
	//			1,
	//			Scalar( 255, 255, 255 ),
	//			8 );

	//	}
	//}
	return MatRet;
}

//用于绘制模板区域
void MyPolygon( Mat img )
{
	int lineType = 8;
	int w = img.rows;
	/** 创建一些点 */
	Point rook_points[1][20];
	rook_points[0][0] = Point( w/4.0, 7*w/8.0 );
	rook_points[0][1] = Point( 3*w/4.0, 7*w/8.0 );
	rook_points[0][2] = Point( 3*w/4.0, 13*w/16.0 );
	rook_points[0][3] = Point( 11*w/16.0, 13*w/16.0 );
	rook_points[0][4] = Point( 19*w/32.0, 3*w/8.0 );
	rook_points[0][5] = Point( 3*w/4.0, 3*w/8.0 );
	rook_points[0][6] = Point( 3*w/4.0, w/8.0 );
	rook_points[0][7] = Point( 26*w/40.0, w/8.0 );
	rook_points[0][8] = Point( 26*w/40.0, w/4.0 );
	rook_points[0][9] = Point( 22*w/40.0, w/4.0 );
	rook_points[0][10] = Point( 22*w/40.0, w/8.0 );
	rook_points[0][11] = Point( 18*w/40.0, w/8.0 );
	rook_points[0][12] = Point( 18*w/40.0, w/4.0 );
	rook_points[0][13] = Point( 14*w/40.0, w/4.0 );
	rook_points[0][14] = Point( 14*w/40.0, w/8.0 );
	rook_points[0][15] = Point( w/4.0, w/8.0 );
	rook_points[0][16] = Point( w/4.0, 3*w/8.0 );
	rook_points[0][17] = Point( 13*w/32.0, 3*w/8.0 );
	rook_points[0][18] = Point( 5*w/16.0, 13*w/16.0 );
	rook_points[0][19] = Point( w/4.0, 13*w/16.0) ;

	const Point* ppt[1] = { rook_points[0] };
	int npt[] = { 20 };

	fillPoly( img,
		ppt,
		npt,
		1,
		Scalar( 255, 255, 255 ),
		lineType );
}

//usm锐化
Mat unsharpmask(Mat src)
{
	Mat blurred;
	double sigma = 1, threshold = 5, amount = 1;

	GaussianBlur(src, blurred, Size(), sigma, sigma);

	Mat lowConstrastMask = abs(src - blurred) < threshold;
	Mat sharpened = src*(1+amount) + blurred*(-amount);
	src.copyTo(sharpened, lowConstrastMask);

	return sharpened;
}


