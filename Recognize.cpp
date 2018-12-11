#include "recognize.h"
//#include<stdio.h>
#include<stdlib.h>
Recognize::Recognize(){

}
vector<IplImage *> Recognize::FindCharacter (IplImage *plate){
	vector<IplImage *> charImgVector;
	vector<CvRect> rect;

	IplImage *resizeImg, *binaryImg, *cloneImg;
	resizeImg = cvCreateImage(cvSize(400,80), IPL_DEPTH_8U, 3);
	binaryImg = cvCreateImage(cvSize(400,80), IPL_DEPTH_8U, 1);
	//cloneImg = cvCreateImage(cvSize(400,80), IPL_DEPTH_8U, 3);
	cvResize(plate, resizeImg);
	cvCvtColor(resizeImg, binaryImg, CV_RGB2GRAY);
	cvAdaptiveThreshold(binaryImg, binaryImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);
	cloneImg = cvCloneImage(resizeImg);
	//tim contour cua tung chu so , de cat roi tung chu so
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours =cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq),sizeof(CvPoint),storage);		//Cvseq *contour =0
	cvFindContours(binaryImg, storage, &contours);	//cvfindcontour(binary,storage,&contours)
	double ratio, ratioWhite;
	int s, white;
	while(contours){
		CvRect r =cvContourBoundingRect(contours,0);
		ratio = (double)r.width/r.height;
		s = r.width*r.height;
		cvSetImageROI(binaryImg, r);
		white = cvCountNonZero(binaryImg);		//dem nhung diem khong phai diem den 
		cvResetImageROI(binaryImg);
		ratioWhite = (double)white/s;

		if(ratio> 0.5 && ratio < 1 && s >500 && r.width >10 && r.width <65 && 
			r.height >50 && r.height <65 && 
			ratioWhite > 0.3 &&ratioWhite <0.75 &&r.x >2)
		{
			cvRectangle(cloneImg, cvPoint(r.x, r.y), cvPoint(r.x + r.width, r.y +r.height), RED,2);
			rect.push_back(r);
		}

		contours = contours ->h_next;
	}
	//sap xep cac anh ki tu
	for(int i = 0; i < rect.size() -1; i++)
	{
		for(int j = i+1 ; j < rect.size() ; j++)
		{
			if(rect[i].x > rect[j].x)
			{
				CvRect swp;
				 swp = rect[i];
				 rect[i] = rect[j];
				 rect[j] = swp;
			}

		}

	}



	//cat ki tu
	IplImage *charImg;
	IplImage *saveImg;
	for (int i = 0; i < rect.size(); i++)
	{
		charImg = cvCreateImage(cvSize(rect[i].width, rect[i].height), IPL_DEPTH_8U, 3);
		cvSetImageROI(resizeImg, rect[i]);
		cvCopy(resizeImg, charImg, NULL);
		cvResetImageROI(resizeImg);

		//add anh vao vector ki tu
		charImgVector.push_back(charImg);

		//show anh
		char name[8];			//chong show de cac bien len nhau
		sprintf(name, "anh %d",i+1);
		cvShowImage(name, charImg);

		// luu ki tu lam mau de training trong mang
		saveImg = cvCreateImage(cvSize(rect[i].width, rect[i].height), IPL_DEPTH_8U, 1);
		cvCvtColor(charImgVector[i], saveImg, CV_RGB2GRAY);
		cvAdaptiveThreshold(saveImg, saveImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);
		
		//show anh
		char sname[8];			//chong show de cac bien len nhau
		sprintf(sname, "Data/ data%d.jpg",i+1);			//luu vao thu muc da ta, sau nay dung de traning
		//cvShowImage(sname, saveImg);
		cvSaveImage(sname, saveImg);
	}


	cvShowImage("character", resizeImg);
	return charImgVector;
}
