//// �������ɽ𡷿γ� �����Tesseract��OCR��
//// �ۺ�ʵ��
//// 2017��11��10�� by jsxyhelu
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


//���Գ���
int _tmain2(int argc, _TCHAR* argv[])
{
	//�ļ�λ�ã�������demo�б��趨Ϊ����λ��
	char *pth_account = "E:/template/template/tmp_account.jpg";  //�����˺�roi
	char *pth_cny = "E:/template/template/tmp_cny.jpg";      //������roi
	char *pth_no = "E:/template/template/tmp_no.jpg";            //�������roi
	char *pth_mingcheng = "E:/template/template/tmp_mingcheng.jpg";
	Mat matAccount;
	Mat matNo;
	Mat matCrash;
	//���ڶ��ƥ��
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
	//ͼƬ������Դ
	char *rootPath = "E://template//resorce";
	//���ڱ����ȡ�ļ�������
	vector<pair<char*, Mat>> filevector;
	//ƥ��ģ��
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
	// �Ҷ�ͼ�� ����ɫͼƬ ��ɫͼƬ
	Mat imagetmp;
	Mat imagetmp2;
	Mat imagetmp3;
	//�������
	char txtout[10240] = "\0";
	char *pdata = NULL;
	char *pResult = NULL;
	char *strName = NULL; //��������
						  //////////////////////////////////������ʽ����////////////////////////////////////////
						  //����tesseract��api����
	tesseract::TessBaseAPI apiNo;
	tesseract::TessBaseAPI apiAccount;
	tesseract::TessBaseAPI apiCrash;
	float confMin = 0; //����С�Ŀ��Ŷ���Ϊ���ս���Ŀ��Ŷ�
	float confNo = 0;
	float confAccount = 0;
	float confCrash = 0;
	apiNo.Init("F:\\gitbup\\lib\\tessdata", "eng", tesseract::OEM_DEFAULT);  //��ʼ�����������԰���ȫ������default
	apiCrash.Init("F:\\gitbup\\lib\\tessdata", "eng", tesseract::OEM_DEFAULT);  //��ʼ�����������԰���ȫ������default
	apiAccount.Init("F:\\gitbup\\lib\\tessdata", "eng", tesseract::OEM_DEFAULT);  //��ʼ�����������԰���ȫ������default
														   //���ݲ�ͬ����趨��ͬ��whitelist
	apiNo.SetVariable("tessedit_char_whitelist", "AZJ-0123456789");//���ΪA Z J ��01234567890-
	apiAccount.SetVariable("tessedit_char_whitelist", "0123456789");//�˺�Ϊ0123456789
	apiCrash.SetVariable("tessedit_char_whitelist", "0123456789.,");//���Ϊ0123456789
																	//�������ʶ���ļ�Ŀ¼�ĵ�ַ
	filevector = read_img(rootPath);
	
	//pair<char*, Mat> ss;
	//ss = make_pair("E://template//resorce//a.jpg",3 );

	char imgstr[500];
	Mat mattmp;
	for (int i = 0; i<filevector.size(); i++)
	{
		//��ȡͼ���ɫͨ��
		imagetmp = imread(filevector[i].first, 0);
		//imagetmp2 = imread(imgstr,0);
		imagetmp3 =  imread(filevector[i].first, 1);
		//����һ��Mat�������������ֺ������  
		vector<Mat> channels;
		//ͨ���Ĳ��  
		split(imagetmp3, channels);
		//��ȡ��ɫͨ��������,��ɫͨ���к��²����ԣ����Ʋ����ԣ�ӡˢ������
		imagetmp2 = channels.at(2);
		//ȡ������ʶ�����ƵĲ���,�Ҷ�ͼƬ
		img_roi_mingcheng = imagetmp(Rect(246, 24, 334, 84));
		//ģ��ƥ��
		matchTemplate(img_roi_no, imagetmp, imagematch, 5); //2014��8��29��10:19:36 �޸�,ƥ�䵽no��λ��
		normalize(imagematch, imagematch, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(imagematch, &minVal, &maxVal2, &minLoc1, &noLoc, Mat());

		matchTemplate(img_roi_cny, imagetmp, imagematch, 5); // cny
		normalize(imagematch, imagematch, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(imagematch, &minVal, &maxVal2, &minLoc1, &cnyLoc, Mat());

		matchTemplate(img_roi_account, imagetmp, imagematch, 5); //account
		normalize(imagematch, imagematch, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(imagematch, &minVal, &maxVal2, &minLoc1, &accountLoc, Mat());

		//����ƥ�䣬ͨ����ʾ�ж�����
		matchTemplate(img_roi_mingcheng, img_mingcheng, imagematch, 5);
		normalize(imagematch, imagematch, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(imagematch, &minVal, &maxVal2, &minLoc1, &mingchengLoc, Mat());
		if (mingchengLoc.y< 50)
		{
			strName = "����ũ����ҵ����";
		}
		else
		{
			strName = "�齭��������";
		}
		//���roi����
		int nox = noLoc.x;
		nox = nox + 16;
		int noy = noLoc.y;
		int nowidth = img_roi_no.cols;
		int noheight = img_roi_no.rows;
		//Ȼ�󻭳�roi����
		rectangle(imagetmp, cv::Point(nox + nowidth, noy), cv::Point(nox + nowidth + 316, noy + noheight), Scalar(0, 0, 0), 1);
		//������������г����������Ҽ���BGR�к�ͨ����ͼƬ
		mattmp = imagetmp2(cv::Rect(nox + nowidth, noy, 316, noheight));
		mattmp.copyTo(matNo);
		//�����˻�����
		//���Ȼ�õ�ǰaccountLoc �� x y width height
		int heightoffset = 25;//���ƫ����Ϊ�˸����ȡ���˻��������Ϊ���ϵ�
		int accountx = accountLoc.x;
		int accounty = accountLoc.y;
		accounty = accounty - 8 - heightoffset;//-8��ƫ����Ϊ��ȥ��....��ע��ԭʼ-8���õ��ף����Ǳ�ı�������ģ���ģ��Ҳ�й�ϵ
		int accountwidth = img_roi_account.cols;
		int accountheight = img_roi_account.rows + heightoffset;
		accountheight = accountheight;
		//Ȼ�󻭳�roi����
		rectangle(imagetmp, cv::Point(accountx + accountwidth, accounty), cv::Point(accountx + accountwidth + 310, accounty + accountheight), Scalar(0, 0, 0), 1);
		//������������г����������Ҽ���BGR�к�ͨ����ͼƬ
		mattmp = imagetmp2(cv::Rect(accountx + accountwidth, accounty, 310, accountheight));
		//���г���ȥ�����,ͨ��ͶӰ���roi���򣬻��ǱȽϾ�ȷ��
		Mat mattmp2 = FetchMaxContour(mattmp);
		//����ͼ��΢�������ʶ��׼ȷ�� 
		resize(mattmp2, mattmp, Size(500 - 50, 80));
		mattmp.copyTo(matAccount);
		//���Ȼ�õ�ǰaccountLoc �� x y width height
		int cnyx = cnyLoc.x;
		int cnyy = cnyLoc.y;
		cnyy = cnyy - 1;
		int cnywidth = img_roi_cny.cols;
		int cnyheight = img_roi_cny.rows*1.2;
		//Ȼ�󻭳�roi����
		rectangle(imagetmp, cv::Point(cnyx + cnywidth, cnyy), cv::Point(cnyx + cnywidth + 264, cnyy + cnyheight), Scalar(0, 0, 0), 1);
		//������������г����������Ҽ���BGR�к�ͨ����ͼƬ
		mattmp = imagetmp2(cv::Rect(cnyx + cnywidth, cnyy, 264, cnyheight));
		mattmp.copyTo(matCrash);
		//////////////////////////////////////////////ʶ�𲿷�//////////////////////////////
		STRING text_out;  //TODO��ͼƬת��Ϊ������ʽ
		printf("��������Ϊ:%s\n", strName);
		sprintf(txtout, "%s ��������Ϊ:%s\n", txtout, strName);
		//ʶ���˺�
		apiAccount.SetImage((unsigned char*)(matAccount.data), matAccount.cols, matAccount.rows, matAccount.channels(), matAccount.step);//����ͼ��
		confAccount = apiAccount.MeanTextConf();//��ÿ��Ŷ�
												//��ӽ���β����ķ���
		iconfAccountMax = 0; iindex = 0;
		if (confAccount<60)
		{
			iconfAccountMax = confAccount;
			for (int i = 0; i<5; i++)
			{
				resize(matAccount, matAccount, sizeAccount[i]);
				apiAccount.SetImage((unsigned char*)(matAccount.data), matAccount.cols, matAccount.rows, matAccount.channels(), matAccount.step);//����ͼ��
				confAccount = apiAccount.MeanTextConf();//��ÿ��Ŷ�
				if (confAccount>iconfAccountMax)
				{
					iconfAccountMax = confAccount;
					iindex = i;
				}
			}
			resize(matAccount, matAccount, sizeAccount[iindex]);
			apiAccount.SetImage((unsigned char*)(matAccount.data), matAccount.cols, matAccount.rows, matAccount.channels(), matAccount.step);//����ͼ��
			confAccount = apiAccount.MeanTextConf();//��ÿ��Ŷ�
		}

		pdata = apiAccount.GetUTF8Text();
		ConvertUtf8ToGBK(&pResult, pdata); //�Խ��ת��
		ClearBlank(&pResult, 17);
		printf("�����˺�Ϊ:%s,���Ŷ�Ϊ%.2f\n", pResult, confAccount);
		sprintf(txtout, "%s �����˺�Ϊ:%s,���Ŷ�Ϊ%.2f\n", txtout, pResult, confAccount);
		putText(imagetmp3, pResult, cv::Point(maxLoc1.x + 561, maxLoc1.y + 217 + 100), 3, 1, Scalar(0, 0, 0));  //��ӡ��ȥ
																												/////ʶ����
		apiCrash.SetImage((unsigned char*)(matCrash.data), matCrash.cols, matCrash.rows, matCrash.channels(), matCrash.step);//����ͼ��
		confCrash = apiCrash.MeanTextConf();//��ÿ��Ŷ� //��Ӷ��ʶ��
		iconfCrashMax = 0; iindex = 0;
		bool bCrashLowquality = false;
		if (confCrash<60)
		{
			bCrashLowquality = true;
			iconfCrashMax = confCrash;
			for (int i = 0; i<5; i++)
			{
				resize(matCrash, matCrash, sizeCrash[i]);
				apiAccount.SetImage((unsigned char*)(matCrash.data), matCrash.cols, matCrash.rows, matCrash.channels(), matCrash.step);//����ͼ��
				confCrash = apiCrash.MeanTextConf();//��ÿ��Ŷ�
				if (confCrash>iconfCrashMax)
				{
					iconfCrashMax = confCrash;
					iindex = i;
				}
			}
			resize(matCrash, matCrash, sizeCrash[iindex]);
			apiCrash.SetImage((unsigned char*)(matCrash.data), matCrash.cols, matCrash.rows, matCrash.channels(), matCrash.step);//����ͼ��
			confCrash = apiCrash.MeanTextConf();//��ÿ��Ŷ�
		}
		pdata = apiCrash.GetUTF8Text();
		ConvertUtf8ToGBK(&pResult, pdata); //�Խ��ת��
		ClearBlank(&pResult);
		if (bCrashLowquality) //������������������Ҫ�Խ���������⴦��
		{
			FormatCrash(&pResult);
		}
		printf("�������Ϊ:%s,���Ŷ�Ϊ%.2f\n", pResult, confCrash);
		sprintf(txtout, "%s �������Ϊ:%s,���Ŷ�Ϊ%.2f\n", txtout, pResult, confCrash);
		putText(imagetmp3, pResult, cv::Point(cnyLoc.x + img_roi_cny.cols, cnyLoc.y - 1), 3, 1, Scalar(0, 0, 0));  //��ӡ��ȥ
																												   //ʶ��������Ǹ�����
		apiNo.SetImage((unsigned char*)(matNo.data), matNo.cols, matNo.rows, matNo.channels(), matNo.step);//����ͼ��
		pdata = apiNo.GetUTF8Text();
		confNo = apiNo.MeanTextConf();//��ÿ��Ŷ�
		ConvertUtf8ToGBK(&pResult, pdata); //�Խ��ת��
		FormatNo(&pResult);
		printf("����������ź�Ϊ:%s�����Ŷ�Ϊ%.2f\n", pResult, confNo);
		ClearBlank(&pResult);
		sprintf(txtout, "%s �����������Ϊ:%s,���Ŷ�Ϊ%.2f\n", txtout, pResult, confNo);
		putText(imagetmp3, pResult, cv::Point(noLoc.x - 10, noLoc.y), 3, 1, Scalar(0, 0, 0));  //��ӡ��ȥ
		printf(" **%s***********\n", filevector[i].first);
		sprintf(txtout, "%s**%s************\n\n", txtout, filevector[i].first);
		//ѭ������
	}
	//����
	delete pResult;
	apiAccount.Clear();
	apiNo.Clear();
	apiCrash.Clear();
	apiAccount.End();
	apiNo.End();
	apiCrash.End();
	//��ӡ�����Ϣ
	//д���ļ�
	FILE *fp;//����һ���ļ�����ָ��
	fp = fopen("d:/reslut.txt", "w+");//��׷�ӵķ�ʽ�������1.txt��Ĭ��λ����������Ŀ¼����
	fprintf(fp, "%s", txtout);//ͬ���printfһ�����Ը�ʽ��ʽ������ı���
	fclose(fp);//�ر���
	
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
////���ȸ��������tesseract-3.02.02-win32-lib-include-dirs.zip
////�Լ�leptonica-1.68-win32-lib-include-dirs.zip
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
////����ʶ����
//int RecognizeMat(Mat src, int &d)
//{
//	//ϵͳ��ʼ��
//	TessBaseAPI api;
//	char *pdata = NULL;
//	char *pResult = NULL;
//	int nRet = api.Init("F:\\gitbup\\lib\\tessdata", "eng", tesseract::OEM_DEFAULT);
//	if (nRet != 0)
//	{
//		printf("��ʼ���ֿ�ʧ�ܣ�");
//		return 0;
//	}
//	api.SetVariable("tessedit_char_whitelist", ".0123456789");
//	api.SetImage((unsigned char*)(src.data), src.cols, src.rows, src.channels(), src.step);//����ͼ��  
//	d = api.MeanTextConf();//ƽ��׼ȷֵ
//	pdata = api.GetUTF8Text();
//	//ConvertUtf8ToGBK(&pResult, pdata); //�Խ��ת��
//	int iret = atoi(pdata);
//	return iret;
//}
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	//opencv������
//	Mat src = imread("E://template//resorce//kdqu.png", 0);
//	int d = 0;
//	if (src.empty())
//	{
//		printf("ͼƬ��ȡ����!");
//		return 0;
//	}
//	int iRet = RecognizeMat(src, d);
//	printf("ʶ����%d,׼ȷ��%d", iRet, d);
//
//	cvWaitKey();
//	getchar();
//	return 0;
//}
