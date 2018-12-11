#pragma once
#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>
#include<iostream>
const CvScalar RED =CV_RGB(255,0,0);
const CvScalar GREEN =CV_RGB(0,255,0);
const CvScalar BLUE =CV_RGB(0,0,255);
using namespace std;
class Recognize 
{ 
public:
	Recognize();
	vector<IplImage*>FindCharacter(IplImage *plate);
	

};