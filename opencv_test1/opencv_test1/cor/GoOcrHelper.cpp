//�ļ�����:GoOcrHelper.cpp
//��    ��: ocrʶ����������
//��    �ģ�
//2014��12��7��12:33:30 ����
//2014��12��14��8:29:27 �ع�ͶӰ��������������min����Ϊ0��ʱ��Ч���ܺã�Ϊʲô����������Ҫ��tesseractocr�Ĵ�����з���
#include "stdafx.h"
#include "GoOcrHelper.h" 
#include <iostream>
#include <fstream>
#include <sstream>
using namespace cv;
using namespace std;
#define IsDebug TRUE 

//��������:ConvertUtf8ToGBK 
//��������:��utf8��ʽת��ΪGBK�ĸ�ʽ
//��    ���� amp     [ot]   ���
//           strUtf8 [in]   ����
void ConvertUtf8ToGBK(char **amp,char *strUtf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL,0)-3; 
	if (len <0) //2014��12��18��19:16:16�޸�
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

 
//��������:read_img 
//��������:������ȡͼƬ
//��    ���� dir     [in]   Ŀ¼
//��    �أ� vector<pair<char*,Mat>>������firstΪ����·����secondΪmat
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

//��������:read_csv 
//��������:ͨ����ȡcsv(txt)�ļ�����������ļ������ƺ����Ӧ��no/crash/account
//��    ���� filename                   [in]   csv(txt)�ļ����ڵ�λ��
//          vector<string>& VectorPath  [ot]   ͼƬ�ļ�����·��
//          vector<string>& VectorNumber [ot]   no
//          vector<string>& VectorAccount [ot]  account
//          vector<string>& VectorCrash    [ot]  crash
//          vchar separator       [in]    �ָ����
//��    �أ� void
void read_csv(const string& filename, vector<string>& VectorPath, vector<string>& VectorNumber,vector<int>& CheckingNumber, vector<string>& VectorAccount,vector<int>& CheckingAccount,vector<string>& VectorCrash,vector<int>& CheckingCrash,char separator)
{
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message ="csv�ļ������ô���!";
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

//��������:ClearBlank 
//��������:ȥ��char�еĿո�
//��    ���� **pResult     [in ot]   ���봮,�����
//          len            [in]   �׶γ���
//��    �أ� void
void ClearBlank(char **pResult,int len )
{
	char *p =*pResult;
	char *pResult2 = new char[50];//�������
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
	*p2 ='\0';//��ӽ�β
	*pResult = pResult2;
}

//��������:FormatCrash 
//��������:����Ǯ�ĸ�ʽ����Ǯ
//��    ���� **pResult     [in ot]   ���봮,�����
//          len            [in]   �׶γ���
//��    �أ� void
void FormatCrash(char **pResult )
{
	char *p =*pResult;
	char *pResult2 = new char[50];//�������
	char *pResult3 = new char[50];//�������
	char *p2 = pResult2;
	char *p3 = pResult3;
	int ilen = 0;
	//����������� . �� ,
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
	p2 = pResult2;//��ͷ��ʼ��
	//ͷ����
	for (int i=0;i<ifirst;i++)
	{
		*p3 = *p2;
		p3++;
		p2++;
	}
	for (int i=0;i<imode;i++)
	{
		//���������Ҫ����
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
	//2014��12��24��22:38:17 hack
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
	*p3 ='\0';//��ӽ�β
	*pResult = pResult3;
}

//��������:FormatNo 
//��������:����no�ĸ�ʽ���д���
//��    ���� **pResult     [in ot]   ���봮,�����
//��    �أ� void
void FormatNo(char **pResult)
{
	char *p =*pResult;
 
	//���� - ������
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

//�������ƣ�FetchMaxContour
//��������: ���˻��������Ԥ��������������������
//��    ��: src [in] ����mat
//��    �أ�������mat
Mat FetchMaxContour(Mat src)
{
	//��ȡͼ��
	Mat testmat = src.clone();
	Mat testclone = src.clone();
	Mat matcanny;
	//����Ѱ������
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int imax = 0;int maxsize = 0;
	RotatedRect theMinRect;
	RotatedRect theMinEllipse;

	//imshow("ԭʼͼ��",testmat);
	//����ҵ���������
	threshold(testmat,testmat,0,255,cv::THRESH_OTSU); 
	//imshow("���",testmat);
	//��������4��
	erode(testmat,testmat,cv::Mat());
	erode(testmat,testmat,cv::Mat());
	erode(testmat,testmat,cv::Mat());
	erode(testmat,testmat,cv::Mat());
	//imshow("����",testmat);
	//ΪʲôҪת������Ϊ��ɫ�������ݵ�����,������Χ�ư�ɫ�����
	threshold(testmat,testmat,0,255,THRESH_BINARY_INV);
	//imshow("��ֵ",testmat);
	
	//�������ͶӰ,����Ķ��ڵ��� row,����Ķ��ڵ���col
	vector<int> vrow;int itmp =0;
	for (int i=0;i<testmat.rows;i++)
	{
		for (int j=0;j<testmat.cols;j++)
		{
			if (testmat.at<uchar>(i,j))//��������ǲ��ܶ����ˣ�ֻҪ����0�ͽ�����
			{
				itmp = itmp +1;
			}
		}
		vrow.push_back(itmp);
		itmp = 0;
	}
	//��������ͶӰ
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
	////�Եõ���ͶӰ������ֵ����
	int iRowMin = 0;int iRowMax = 0;
	int iColMin = 0;int iColMax = 0;
	//��Ϊ�ں����У�����200�ľ���roi����
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
 //   //�ҳ��������
	//std::vector<std::vector<cv::Point>>contours;
	//cv::findContours(testmat,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
	////�ҳ�����contours
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
	//������Ķ�������
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

//���ڻ���ģ������
void MyPolygon( Mat img )
{
	int lineType = 8;
	int w = img.rows;
	/** ����һЩ�� */
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

//usm��
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


