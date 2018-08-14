//�ļ�����:GoOcrHelper.cpp
//��    ��: ocrʶ����������
//��    �ģ�
//2014��12��7��12:33:30 ����
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
//��������:ConvertUtf8ToGBK 
//��������:��utf8��ʽת��ΪGBK�ĸ�ʽ
//��    ���� amp     [ot]   ���
//           strUtf8 [in]   ����
void ConvertUtf8ToGBK(char **amp,char *strUtf8);


//��������:read_img 
//��������:������ȡͼƬ
//��    ���� dir     [in]   Ŀ¼
//��    �أ� vector<pair<char*,Mat>>������firstΪ����·����secondΪmat
vector<pair<char*,Mat>>  read_img(const string& dir);

//��������:read_csv 
//��������:ͨ����ȡcsv(txt)�ļ�����������ļ������ƺ����Ӧ��no/crash/account
//��    ���� filename                   [in]   csv(txt)�ļ����ڵ�λ��
//          vector<string>& VectorPath  [ot]   ͼƬ�ļ�����·��
//          vector<string>& VectorNumber [ot]   no
//          vector<string>& VectorAccount [ot]  account
//          vector<string>& VectorCrash    [ot]  crash
//          vchar separator =';'         [in]    �ָ����
//��    �أ� void
void read_csv(const string& filename, vector<string>& VectorPath, vector<string>& VectorNumber,vector<int>& CheckingNumber, vector<string>& VectorAccount,vector<int>& CheckingAccount,vector<string>& VectorCrash,vector<int>& CheckingCrash,char separator=';');

//��������:ClearBlank 
//��������:ȥ��char�еĿո�
//��    ���� **pResult     [in ot]   ���봮,�����
//          len            [in]   �׶γ���
//��    �أ� void
void ClearBlank(char **pResult,int len = 0);

//�������ƣ�FetchMaxContour
//��������: ����������������
//��    ��: src [in] ����mat
//��    �أ�������mat
Mat FetchMaxContour(Mat src);

//��������:FormatCrash 
//��������:����Ǯ�ĸ�ʽ����Ǯ
//��    ���� **pResult     [in ot]   ���봮,�����
//          len            [in]   �׶γ���
//��    �أ� void
void FormatCrash(char **pResult);

void FormatNo(char **pResult);
