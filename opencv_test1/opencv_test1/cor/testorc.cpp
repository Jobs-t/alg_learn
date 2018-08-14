//// 《炼数成金》课程 《结合Tesseract的OCR》
//// 综合实验
//// 2017年11月10日 by jsxyhelu
#include "stdafx.h"
#include "GoOcrHelper.h" 
//#pragma comment(lib, "public/lib/libtesseract302.lib")
//#pragma comment(lib, "../lib/liblept.lib")

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../public/include/tesseract/baseapi.h"
#include "../public/include/tesseract/basedir.h"
#include "../public/include/tesseract/strngs.h"


using namespace cv;
using namespace std;
using tesseract::TessBaseAPI;
#define IsDebug TRUE


//测试程序
int _tmain2(int argc, _TCHAR* argv[])
{
	//文件位置，这里在demo中被设定为绝对位置
	char *pth_account = "E:/template/template/tmp_account.jpg";  //保存账号roi
	char *pth_cny = "E:/template/template/tmp_cny.jpg";      //保存金额roi
	char *pth_no = "E:/template/template/tmp_no.jpg";            //保存序号roi
	char *pth_mingcheng = "E:/template/template/tmp_mingcheng.jpg";
	Mat matAccount;
	Mat matNo;
	Mat matCrash;
	//用于多次匹配
	int iconfAccountMax; int iindex = 0;
	int iconfCrashMax;
	Size sizeAccount[5];
	sizeAccount[0] = Size(620, 120);
	sizeAccount[1] = Size(600, 100);
	sizeAccount[2] = Size(500, 80);
	sizeAccount[3] = Size(800, 140);
	sizeAccount[4] = Size(450, 80);
	Size sizeCrash[5];
	sizeCrash[0] = Size(620, 120);
	sizeCrash[1] = Size(600, 100);
	sizeCrash[2] = Size(500, 80);
	sizeCrash[3] = Size(800, 140);
	sizeCrash[4] = Size(450, 80);
	//图片数据来源
	char *rootPath = "E://template//resorce";
	//用于保存读取文件的向量
	vector<pair<char*, Mat>> filevector;
	//匹配模板
	Mat img_roi_no = imread("E://template//template//no.jpg", 0);
	Mat img_roi_cny = imread("E://template//template//cny.jpg", 0);
	Mat img_roi_account = imread("E://template//template//account.jpg", 0);
	Mat img_mingcheng = imread("E://template//template//mingcheng.jpg", 0);
	Mat img_roi_mingcheng;
	Mat imagematch;
	Point noLoc;
	Point cnyLoc;
	Point accountLoc;
	Point mingchengLoc;
	Point minLoc1;
	Point maxLoc1;
	double minVal;
	double maxVal2;
	// 灰度图像 敏感色图片 彩色图片
	Mat imagetmp;
	Mat imagetmp2;
	Mat imagetmp3;
	//用于输出
	char txtout[10240] = "\0";
	char *pdata = NULL;
	char *pResult = NULL;
	char *strName = NULL; //银行名称
						  //////////////////////////////////程序正式部分////////////////////////////////////////
						  //生成tesseract的api调用
	tesseract::TessBaseAPI apiNo;
	tesseract::TessBaseAPI apiAccount;
	tesseract::TessBaseAPI apiCrash;
	float confMin = 0; //以最小的可信度作为最终结果的可信度
	float confNo = 0;
	float confAccount = 0;
	float confCrash = 0;
	apiNo.Init("F:\\gitbup\\lib\\tessdata", "eng", tesseract::OEM_DEFAULT);  //初始化，设置语言包，全部采用default
	apiCrash.Init("F:\\gitbup\\lib\\tessdata", "eng", tesseract::OEM_DEFAULT);  //初始化，设置语言包，全部采用default
	apiAccount.Init("F:\\gitbup\\lib\\tessdata", "eng", tesseract::OEM_DEFAULT);  //初始化，设置语言包，全部采用default
														   //根据不同情况设定不同的whitelist
	apiNo.SetVariable("tessedit_char_whitelist", "AZJ-0123456789");//序号为A Z J 和01234567890-
	apiAccount.SetVariable("tessedit_char_whitelist", "0123456789");//账号为0123456789
	apiCrash.SetVariable("tessedit_char_whitelist", "0123456789.,");//金额为0123456789
																	//输入或获得识别文件目录的地址
	filevector = read_img(rootPath);
	
	//pair<char*, Mat> ss;
	//ss = make_pair("E://template//resorce//a.jpg",3 );

	char imgstr[500];
	Mat mattmp;
	for (int i = 0; i<filevector.size(); i++)
	{
		//获取图像红色通道
		imagetmp = imread(filevector[i].first, 0);
		//imagetmp2 = imread(imgstr,0);
		imagetmp3 =  imread(filevector[i].first, 1);
		//定义一个Mat向量容器保存拆分后的数据  
		vector<Mat> channels;
		//通道的拆分  
		split(imagetmp3, channels);
		//提取红色通道的数据,红色通道中红章不明显，底纹不明显，印刷字明显
		imagetmp2 = channels.at(2);
		//取得用于识别名称的部分,灰度图片
		img_roi_mingcheng = imagetmp(Rect(246, 24, 334, 84));
		//模板匹配
		matchTemplate(img_roi_no, imagetmp, imagematch, 5); //2014年8月29日10:19:36 修改,匹配到no的位置
		normalize(imagematch, imagematch, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(imagematch, &minVal, &maxVal2, &minLoc1, &noLoc, Mat());

		matchTemplate(img_roi_cny, imagetmp, imagematch, 5); // cny
		normalize(imagematch, imagematch, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(imagematch, &minVal, &maxVal2, &minLoc1, &cnyLoc, Mat());

		matchTemplate(img_roi_account, imagetmp, imagematch, 5); //account
		normalize(imagematch, imagematch, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(imagematch, &minVal, &maxVal2, &minLoc1, &accountLoc, Mat());

		//反向匹配，通过标示判断银行
		matchTemplate(img_roi_mingcheng, img_mingcheng, imagematch, 5);
		normalize(imagematch, imagematch, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(imagematch, &minVal, &maxVal2, &minLoc1, &mingchengLoc, Mat());
		if (mingchengLoc.y< 50)
		{
			strName = "广州农村商业银行";
		}
		else
		{
			strName = "珠江村镇银行";
		}
		//获得roi区域
		int nox = noLoc.x;
		nox = nox + 16;
		int noy = noLoc.y;
		int nowidth = img_roi_no.cols;
		int noheight = img_roi_no.rows;
		//然后画出roi区域
		rectangle(imagetmp, cv::Point(nox + nowidth, noy), cv::Point(nox + nowidth + 316, noy + noheight), Scalar(0, 0, 0), 1);
		//最后将这个区域剪切出来，这里我剪切BGR中红通道的图片
		mattmp = imagetmp2(cv::Rect(nox + nowidth, noy, 316, noheight));
		mattmp.copyTo(matNo);
		//画出账户区域
		//首先获得当前accountLoc 的 x y width height
		int heightoffset = 25;//这个偏移是为了更多地取得账户区域而人为加上的
		int accountx = accountLoc.x;
		int accounty = accountLoc.y;
		accounty = accounty - 8 - heightoffset;//-8的偏移是为了去掉....，注意原始-8正好到底，这是表的本身决定的，和模板也有关系
		int accountwidth = img_roi_account.cols;
		int accountheight = img_roi_account.rows + heightoffset;
		accountheight = accountheight;
		//然后画出roi区域
		rectangle(imagetmp, cv::Point(accountx + accountwidth, accounty), cv::Point(accountx + accountwidth + 310, accounty + accountheight), Scalar(0, 0, 0), 1);
		//最后将这个区域剪切出来，这里我剪切BGR中红通道的图片
		mattmp = imagetmp2(cv::Rect(accountx + accountwidth, accounty, 310, accountheight));
		//进行初步去噪操作,通过投影算出roi区域，还是比较精确的
		Mat mattmp2 = FetchMaxContour(mattmp);
		//处理图像微调，提高识别准确率 
		resize(mattmp2, mattmp, Size(500 - 50, 80));
		mattmp.copyTo(matAccount);
		//首先获得当前accountLoc 的 x y width height
		int cnyx = cnyLoc.x;
		int cnyy = cnyLoc.y;
		cnyy = cnyy - 1;
		int cnywidth = img_roi_cny.cols;
		int cnyheight = img_roi_cny.rows*1.2;
		//然后画出roi区域
		rectangle(imagetmp, cv::Point(cnyx + cnywidth, cnyy), cv::Point(cnyx + cnywidth + 264, cnyy + cnyheight), Scalar(0, 0, 0), 1);
		//最后将这个区域剪切出来，这里我剪切BGR中红通道的图片
		mattmp = imagetmp2(cv::Rect(cnyx + cnywidth, cnyy, 264, cnyheight));
		mattmp.copyTo(matCrash);
		//////////////////////////////////////////////识别部分//////////////////////////////
		STRING text_out;  //TODO将图片转换为内联方式
		printf("本单银行为:%s\n", strName);
		sprintf(txtout, "%s 本单银行为:%s\n", txtout, strName);
		//识别账号
		apiAccount.SetImage((unsigned char*)(matAccount.data), matAccount.cols, matAccount.rows, matAccount.channels(), matAccount.step);//设置图像
		confAccount = apiAccount.MeanTextConf();//获得可信度
												//添加将多次测量的方法
		iconfAccountMax = 0; iindex = 0;
		if (confAccount<60)
		{
			iconfAccountMax = confAccount;
			for (int i = 0; i<5; i++)
			{
				resize(matAccount, matAccount, sizeAccount[i]);
				apiAccount.SetImage((unsigned char*)(matAccount.data), matAccount.cols, matAccount.rows, matAccount.channels(), matAccount.step);//设置图像
				confAccount = apiAccount.MeanTextConf();//获得可信度
				if (confAccount>iconfAccountMax)
				{
					iconfAccountMax = confAccount;
					iindex = i;
				}
			}
			resize(matAccount, matAccount, sizeAccount[iindex]);
			apiAccount.SetImage((unsigned char*)(matAccount.data), matAccount.cols, matAccount.rows, matAccount.channels(), matAccount.step);//设置图像
			confAccount = apiAccount.MeanTextConf();//获得可信度
		}

		pdata = apiAccount.GetUTF8Text();
		ConvertUtf8ToGBK(&pResult, pdata); //对结果转码
		ClearBlank(&pResult, 17);
		printf("本单账号为:%s,可信度为%.2f\n", pResult, confAccount);
		sprintf(txtout, "%s 本单账号为:%s,可信度为%.2f\n", txtout, pResult, confAccount);
		putText(imagetmp3, pResult, cv::Point(maxLoc1.x + 561, maxLoc1.y + 217 + 100), 3, 1, Scalar(0, 0, 0));  //打印上去
																												/////识别金额
		apiCrash.SetImage((unsigned char*)(matCrash.data), matCrash.cols, matCrash.rows, matCrash.channels(), matCrash.step);//设置图像
		confCrash = apiCrash.MeanTextConf();//获得可信度 //添加多次识别
		iconfCrashMax = 0; iindex = 0;
		bool bCrashLowquality = false;
		if (confCrash<60)
		{
			bCrashLowquality = true;
			iconfCrashMax = confCrash;
			for (int i = 0; i<5; i++)
			{
				resize(matCrash, matCrash, sizeCrash[i]);
				apiAccount.SetImage((unsigned char*)(matCrash.data), matCrash.cols, matCrash.rows, matCrash.channels(), matCrash.step);//设置图像
				confCrash = apiCrash.MeanTextConf();//获得可信度
				if (confCrash>iconfCrashMax)
				{
					iconfCrashMax = confCrash;
					iindex = i;
				}
			}
			resize(matCrash, matCrash, sizeCrash[iindex]);
			apiCrash.SetImage((unsigned char*)(matCrash.data), matCrash.cols, matCrash.rows, matCrash.channels(), matCrash.step);//设置图像
			confCrash = apiCrash.MeanTextConf();//获得可信度
		}
		pdata = apiCrash.GetUTF8Text();
		ConvertUtf8ToGBK(&pResult, pdata); //对结果转码
		ClearBlank(&pResult);
		if (bCrashLowquality) //如果处于这种情况，需要对结果进行特殊处理
		{
			FormatCrash(&pResult);
		}
		printf("本单金额为:%s,可信度为%.2f\n", pResult, confCrash);
		sprintf(txtout, "%s 本单金额为:%s,可信度为%.2f\n", txtout, pResult, confCrash);
		putText(imagetmp3, pResult, cv::Point(cnyLoc.x + img_roi_cny.cols, cnyLoc.y - 1), 3, 1, Scalar(0, 0, 0));  //打印上去
																												   //识别上面的那个号码
		apiNo.SetImage((unsigned char*)(matNo.data), matNo.cols, matNo.rows, matNo.channels(), matNo.step);//设置图像
		pdata = apiNo.GetUTF8Text();
		confNo = apiNo.MeanTextConf();//获得可信度
		ConvertUtf8ToGBK(&pResult, pdata); //对结果转码
		FormatNo(&pResult);
		printf("本单机打序号号为:%s，可信度为%.2f\n", pResult, confNo);
		ClearBlank(&pResult);
		sprintf(txtout, "%s 本单机打序号为:%s,可信度为%.2f\n", txtout, pResult, confNo);
		putText(imagetmp3, pResult, cv::Point(noLoc.x - 10, noLoc.y), 3, 1, Scalar(0, 0, 0));  //打印上去
		printf(" **%s***********\n", filevector[i].first);
		sprintf(txtout, "%s**%s************\n\n", txtout, filevector[i].first);
		//循环往复
	}
	//销毁
	delete pResult;
	apiAccount.Clear();
	apiNo.Clear();
	apiCrash.Clear();
	apiAccount.End();
	apiNo.End();
	apiCrash.End();
	//打印相关信息
	//写入文件
	FILE *fp;//建立一个文件操作指针
	fp = fopen("d:/reslut.txt", "w+");//以追加的方式建立或打开1.txt，默认位置在你程序的目录下面
	fprintf(fp, "%s", txtout);//同输出printf一样，以格式方式输出到文本中
	fclose(fp);//关闭流
	
	return 0;
}




//
//#include <cv.h>
//#include <cxcore.h>
//#include <highgui.h>
//#include <stdio.h>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "../public/include/tesseract/baseapi.h"
//#include "../public/include/tesseract/strngs.h"
//#include "leptonica/allheaders.h"
//
////#pragma comment(lib, "public/lib/libtesseract302.lib")
////#pragma comment(lib, "lib/liblept.lib")
//using namespace cv;
//using namespace std;
//using tesseract::TessBaseAPI;
//#define TOPHEIGHT 304
//#define ROWNHEIGHT 56
////到谷歌官网下载tesseract-3.02.02-win32-lib-include-dirs.zip
////以及leptonica-1.68-win32-lib-include-dirs.zip
//
////void ConvertUtf8ToGBK(char **amp, char *strUtf8)
////{
////	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL, 0);
////	unsigned short * wszGBK = new unsigned short[len + 1];
////	memset(wszGBK, 0, len * 2 + 2);
////	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, (LPWSTR)wszGBK, len);
////	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
////	//char *szGBK=new char[len + 1]; 
////	*amp = new char[len + 1];
////	memset(*amp, 0, len + 1);
////	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, *amp, len, NULL, NULL);
////	//amp=szGBK; 
////}
//
////返回识别结果
//int RecognizeMat(Mat src, int &d)
//{
//	//系统初始化
//	TessBaseAPI api;
//	char *pdata = NULL;
//	char *pResult = NULL;
//	int nRet = api.Init("F:\\gitbup\\lib\\tessdata", "eng", tesseract::OEM_DEFAULT);
//	if (nRet != 0)
//	{
//		printf("初始化字库失败！");
//		return 0;
//	}
//	api.SetVariable("tessedit_char_whitelist", ".0123456789");
//	api.SetImage((unsigned char*)(src.data), src.cols, src.rows, src.channels(), src.step);//设置图像  
//	d = api.MeanTextConf();//平均准确值
//	pdata = api.GetUTF8Text();
//	//ConvertUtf8ToGBK(&pResult, pdata); //对结果转码
//	int iret = atoi(pdata);
//	return iret;
//}
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	//opencv处理部分
//	Mat src = imread("E://template//resorce//kdqu.png", 0);
//	int d = 0;
//	if (src.empty())
//	{
//		printf("图片读取错误!");
//		return 0;
//	}
//	int iRet = RecognizeMat(src, d);
//	printf("识别结果%d,准确率%d", iRet, d);
//
//	cvWaitKey();
//	getchar();
//	return 0;
//}
