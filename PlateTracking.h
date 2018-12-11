#pragma once
#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>
#include<iostream>

class PlateTracking
{
private:
	IplConvKernel *S1;			//MA TRÂN để dung kernel
	IplConvKernel *S2;
	IplImage *plate;			//MA TRẬN CHỨA ẢNH BIỂN SỐ ĐƯỢC CẮT RỜI

public:
	PlateTracking(void);
	virtual ~PlateTracking(void);
	void ImageRestoration(IplImage *Img);		// XỬ LÍ ẢNH GỐC DẠNG GRAYSCALE
	IplImage* TrackPlate(IplImage *Img);		// TIM VA CAT BIEN SO, TRA VE TRACKPLATE
	int CountCharacter(IplImage *plate);		//dem so vung co kha nag la ki tu
};

