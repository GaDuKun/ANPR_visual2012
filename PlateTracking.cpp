#include "PlateTracking.h"
const CvScalar RED =CV_RGB(255,0,0);
const CvScalar GREEN =CV_RGB(0,255,0);
const CvScalar BLUE =CV_RGB(0,0,255);
PlateTracking::PlateTracking(void)
{
	S1 = cvCreateStructuringElementEx(3, 1, 1, 0, CV_SHAPE_CROSS, NULL);			// tạo ma trận 3 cột , 1 hàng cho S1  kernel mask for filtering
	S2 = cvCreateStructuringElementEx(6, 1, 3, 0, CV_SHAPE_CROSS, NULL);
	plate = NULL;



}


PlateTracking::~PlateTracking()
{
	if (plate)			// khi đã tìm được plate thì hủy các đầu vào đi để bắt đầu một nhận dạng mới
	{
		S1 = NULL;
		S2 = NULL;
		plate = NULL;
	}
}
void PlateTracking::ImageRestoration(IplImage *Img)					//tiền xử lí
{
	int w = Img->width;				// BIẾN LƯU LẠI KÍCH THƯỚC ẢNH
	int h = Img->height;
	IplImage *mImg = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);			//ẢNH SỬ DỤNG CHO BIẾN ĐỔI HÌNH THÁI HỌC,CHỌN BITS_DEPTH = 8 BITS ĐỂ GIẢM DUNG LƯỢNG, 256 COLOR
	IplImage *Img_pyrdown = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);		//THU NHỎ ẢNH LẠI, PYRAMID THEORY
	IplImage *tmp = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);					//ẢNH TẠM THỜI
	IplImage *Img_threshold = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *mini_threshold = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *dst = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);				// ẢNH ĐÃ LÀM RÕ VÙNG BIỂN SỐ (DESTINATION)

	cvPyrDown(Img, Img_pyrdown);
	cvMorphologyEx(Img_pyrdown, mImg, tmp, S2, CV_MOP_BLACKHAT);
	cvNormalize(mImg, mImg, 0, 255, CV_MINMAX);

	// nhị phân hóa ảnh mImg
	cvThreshold(mImg, Img_threshold, (int)10*cvAvg(mImg).val[0], 255, CV_THRESH_BINARY );
	cvZero(dst);
	cvCopy(Img_threshold, mini_threshold);

	// su dung mot mat na hinh chu nhat 8x16 quet toan bo anh
	int cnt;			//bIẾN ĐẾM
	int po1, po2, po3, po4;		//Position CÓ THỂ CHỨA BIỂN SỐ
	CvRect rect; 

	// loop
	for( int i = 0; i< mini_threshold->width-32; i+=4)			// Moi FRAME DUOC GHEP BANG 4 MAT NA
	{
		for(int j = 0; j< mini_threshold ->height-16; j+=4)
		{
			{
				rect = cvRect(i,j,16,8);			//TAO MOT  MAT NA HINH CHU NHAT CO TOP-LEFT POIN(X,Y)
				cvSetImageROI(mini_threshold, rect);						// region of image- khoanh vung tren anh
				po1= cvCountNonZero(mini_threshold);						//DEM SO PIXEL KHONG PHAI DIEM TRANG
				cvResetImageROI(mini_threshold);							//reset vung vua lay

				rect = cvRect(i+16, j, 16, 8);			//TAO MOT  MAT NA HINH CHU NHAT
				cvSetImageROI(mini_threshold, rect);						// region of image- khoanh vung tren anh
				po2= cvCountNonZero(mini_threshold);						//DEM SO PIXEL KHONG PHAI DIEM TRANG
				cvResetImageROI(mini_threshold);	

				rect = cvRect(i, j+8, 16, 8);			//TAO MOT  MAT NA HINH CHU NHAT
				cvSetImageROI(mini_threshold, rect);						// region of image- khoanh vung tren anh
				po3= cvCountNonZero(mini_threshold);						//DEM SO PIXEL KHONG PHAI DIEM TRANG
				cvResetImageROI(mini_threshold);	

				rect = cvRect(i+16, j+8, 16, 8);			//TAO MOT  MAT NA HINH CHU NHAT
				cvSetImageROI(mini_threshold, rect);						// region of image- khoanh vung tren anh
				po4= cvCountNonZero(mini_threshold);						//DEM SO PIXEL KHONG PHAI DIEM TRANG
				cvResetImageROI(mini_threshold);	

				cnt = 0;
				if(po1>15)	cnt++;		//>20 diem mau trang
				if(po2>15)	cnt++;
				if(po3>15)	cnt++;
				if(po4>15)	cnt++;

				if(cnt >2)

					{

						rect = cvRect(i,j,32,16);
						cvSetImageROI(dst, rect);
						cvSetImageROI(mini_threshold, rect);
						cvCopy(mini_threshold, dst);
						cvResetImageROI(dst);
						cvResetImageROI(mini_threshold);
					}

			}
		}
	}
IplImage *dst_clone = cvCloneImage(dst);		//sao chep anh dst truoc khi bi lam nhoe
// LAM NHOE PHAN UNG CU VIEN
cvDilate(dst,dst,NULL, 2);	//Gian ra 2 pixel
cvErode(dst,dst, NULL, 2);	//Nen lai 2 pixel
cvDilate(dst,dst,S1, 9);
cvErode(dst,dst, S1, 10);	
cvDilate(dst,dst);
// SHOW ANH LEN XEM choi
/*
cvShowImage("source", Img);
cvShowImage("mImg", mImg);
cvShowImage("img threshold", mini_threshold);
cvShowImage("image destination", dst);
cvShowImage("image destination cloned", dst_clone);
*/

//Tra ve anh goc
cvPyrUp(dst,Img);

//giai phong bo nho
//cvReleaseImage(&Img);
cvReleaseImage(&mini_threshold);
cvReleaseImage(&dst);
cvReleaseImage(&dst_clone);
cvReleaseImage(&mImg);
cvReleaseImage(&tmp);
cvReleaseImage(&Img_pyrdown);
cvReleaseImage(&Img_threshold);


}
IplImage*PlateTracking::TrackPlate(IplImage *Img)
{
	IplImage *plate;			
	IplImage* grayImg1 = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U,1);		//anh grey
	IplImage* rectImg =   cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U,3);		// anh bien so duoc them rectangle o bien so
	IplImage* contourImg = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U,1);		//TAO ANH CHUA DUONG BAO
	cvCvtColor(Img, grayImg1, CV_RGB2GRAY);
	IplImage* cloneImg =   cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U,3);	//anh copy co mau(kenh 3)
	cloneImg = cvCloneImage(Img);
	cvShowImage("clone Imge tracking",cloneImg);

	//tien xu li
	cvCopy(grayImg1,contourImg);
	cvNormalize(contourImg,contourImg,0,255,CV_MINMAX); //khu nhieu tren buc anh
	ImageRestoration(contourImg);
	cvShowImage("contour Image1", contourImg);
	cvMerge(contourImg, contourImg, contourImg, NULL, rectImg);		//tron anh rectImg chua cac rect vao anh chua cac phan nhoe
	//tim contour
	CvMemStorage *storagePlate = cvCreateMemStorage(0);		//luu lai vung chua bien so
	CvSeq *contours = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq),sizeof(CvPoint),storagePlate);			//luu lai anh co contour
	cvFindContours(contourImg,storagePlate, &contours, sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

	//cvShowImage("contour Image", contourImg);        Ảnh biển số đã được bôi trắng phần biển, background màu đen

	//tim cac vung contours  
	CvRect rectPlate;
	int xmin,ymin,xmax,ymax, w,h,s,r;		//r: rario of contour/s
	int count;
	double ratio;							// w/h

	//luu lai cac anh da cat duoc co kha nang la bien so
	IplImage** plateArr = new IplImage *[4];	//luu  vung thoi vi neu co qua nhieu nhieu thi moi sinh ra nhieu vung chua bien so nhu vay
	int j =0;
	for(int i =0; i < 4; i++)
	{
		plateArr[i] = NULL;
	}

	while(contours){
		count =contours -> total;
		CvPoint *PointArray =new CvPoint[count];
		cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);
		for(int i = 0; i<count ;i++)
		{
			if( i==0 ){
				xmin = xmax = PointArray[i].x;	
				ymin = ymax = PointArray[i].y;
			}
			if(PointArray[i].x > xmax){
				xmax = PointArray[i].x;

			} 
			if(PointArray[i].x < xmin){
				xmin = PointArray[i].x;

			} 
			

			if(PointArray[i].y > ymax){
				ymax = PointArray[i].y;

			}
			if(PointArray[i].y < ymin){
				ymin = PointArray[i].y;

			}


		}

		w = xmax - xmin;
		h =ymax - ymin;
		s = w*h; 
		cvRectangle(rectImg, cvPoint(xmin,ymin), cvPoint(xmax,ymax),RED);

		//loại bỏ những contour có tỉ lệ không đúng

		if(s != 0)
		{
			r =((contourImg->height* contourImg->width)/s);
		}
		else r = 1000;		//giai thich sau
		if(w==0 && h==0){
			ratio = 0;			//truong hop nay khong lam viec vi khong co anh
		}
		else 
			ratio = (double)w/h;

		if(r>30 && r<270){					//con số này phải thống kê diện tích của biển số so với xe, nên dùng matlab kiểm tra lại album
			cvRectangle(rectImg, cvPoint(xmin,ymin), cvPoint(xmax,ymax),GREEN);
			if(ratio >2.6 && ratio <7){
			 cvRectangle(rectImg, cvPoint(xmin,ymin), cvPoint(xmax,ymax),BLUE);
				if (w > 80 && w<250 &&h>25 && h<150){
				  rectPlate = cvRect(xmin,ymin,w,h);
				  //them rectangle bien so vao anh clone cua anh goc
				  cvRectangle(cloneImg, cvPoint(rectPlate.x,rectPlate.y),cvPoint(rectPlate.x + rectPlate.width, rectPlate.y + rectPlate.height), BLUE, 3);
				 
				  //cat roi bien so tren anh resize
				  plate = cvCreateImage(cvSize(rectPlate.width, rectPlate.height), IPL_DEPTH_8U, 3);
				  cvSetImageROI(Img, rectPlate);
				  cvCopy(Img, plate, NULL);
				  cvResetImageROI(Img);

				  //luu vao mang anh cac bien so
				  int cnt = CountCharacter(plate);
				  if(cnt >= 6)
				  {
					  plateArr[j] = cvCloneImage(plate);
					  j++;
				  }


				}
			
			}

		}
		delete []PointArray;
		contours = contours->h_next;
	}

	//sap xep lai mang nao nhieu len tren
	if(plateArr[0]){
		int w = plateArr[0]->width;
		int flag;
		for(int i=1; i<3; i++)
		{
			if(plateArr[i] && plateArr[i] ->width <w)
			{
					flag = i;
			}
		}
			plateArr[0] = plateArr[flag];

	}

	cvShowImage("rectImg",rectImg);
	cvShowImage("anh bien so da duoc set rectangle", cloneImg);
	cvShowImage("anh bien so duoc cat roi", plate); 

	cvReleaseImage(&contourImg);
	cvReleaseImage(&rectImg);
	cvReleaseImage(&plate);
	return plateArr[0];
}

int PlateTracking::CountCharacter(IplImage *plate){
	int cnt = 0;
	IplImage *resizeImg, *binaryImg;
	resizeImg = cvCreateImage(cvSize(400,80), IPL_DEPTH_8U, 3);
	binaryImg = cvCreateImage(cvSize(400,80), IPL_DEPTH_8U, 1);
	cvResize(plate, resizeImg);
	cvCvtColor(resizeImg, binaryImg, CV_RGB2GRAY);
	cvAdaptiveThreshold(binaryImg, binaryImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);
	cvShowImage("anh binary anh bien so", binaryImg);

	//tim contour cua tung chu so , de cat roi tung chu so
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours =cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq),sizeof(CvPoint),storage);		//Cvseq *contour =0
	cvFindContours(binaryImg, storage, &contours);	//cvfindcontour(binary,storage,&contours)
	cvShowImage("contours", binaryImg);
	while(contours){
		CvRect rect = cvBoundingRect(contours);			//ve duong bao quanh contour, co san trong opencv
		if(rect.width > 10 && rect.width < 65 && rect.height > 50 &&rect.height < 65 && rect.width * rect.height >800)
		{
			cvRectangle(resizeImg, cvPoint(rect.x,rect.y), cvPoint(rect.x + rect.width,rect.y + rect.height),GREEN, 2);
			cnt++;
		}
		contours = contours ->h_next;

	}

	cvShowImage("anh bien so da duoc them duong bao vao cac con so",resizeImg);

	return cnt;
}

