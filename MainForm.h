#pragma once
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\ml.h>
#include <opencv\cxcore.h>
#include <stdio.h>
#include "PlateTracking.h"
#include "recognize.h"

namespace ANPRproject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;


	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	private:
			// ẢNH ĐỌC ĐƯỢC SHOW LÊN MÀN HÌNH

	public:
		IplImage *Img ;		
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
			if (Img)
			{
				Img = NULL  ;		// HỦY ẢNH VỪA ĐỌC 
			}
		}
	private: System::Windows::Forms::PictureBox^  pbImg;
	private: System::Windows::Forms::Button^  btBrowse;
	private: System::Windows::Forms::Button^  btRecognize;
	private: System::Windows::Forms::TextBox^  tbPlate;
	private: System::Windows::Forms::PictureBox^  pbPlate;

	//private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;			// moi them vao


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pbImg = (gcnew System::Windows::Forms::PictureBox());
			this->btBrowse = (gcnew System::Windows::Forms::Button());
			this->btRecognize = (gcnew System::Windows::Forms::Button());
			this->tbPlate = (gcnew System::Windows::Forms::TextBox());
			this->pbPlate = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbImg))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPlate))->BeginInit();
			this->SuspendLayout();
			// 
			// pbImg
			// 
			this->pbImg->Location = System::Drawing::Point(36, 12);
			this->pbImg->Name = L"pbImg";
			this->pbImg->Size = System::Drawing::Size(384, 302);
			this->pbImg->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbImg->TabIndex = 0;
			this->pbImg->TabStop = false;
			this->pbImg->Click += gcnew System::EventHandler(this, &MainForm::pictureBox1_Click);
			// 
			// Browse
			// 
			this->btBrowse->Location = System::Drawing::Point(328, 377);
			this->btBrowse->Name = L"Browse";
			this->btBrowse->Size = System::Drawing::Size(75, 23);
			this->btBrowse->TabIndex = 1;
			this->btBrowse->Text = L"Browse";
			this->btBrowse->UseVisualStyleBackColor = true;
			this->btBrowse->Click += gcnew System::EventHandler(this, &MainForm::Browse_Click);
			// 
			// Recognize
			// 
			this->btRecognize->Location = System::Drawing::Point(328, 406);
			this->btRecognize->Name = L"Recognize";
			this->btRecognize->Size = System::Drawing::Size(75, 23);
			this->btRecognize->TabIndex = 2;
			this->btRecognize->Text = L"Recognize";
			this->btRecognize->UseVisualStyleBackColor = true;
			this->btRecognize->Click += gcnew System::EventHandler(this, &MainForm::Recognize_Click);
			// 
			// tbPlate
			// 
			this->tbPlate->Location = System::Drawing::Point(36, 390);
			this->tbPlate->Multiline = true;
			this->tbPlate->Name = L"tbPlate";
			this->tbPlate->Size = System::Drawing::Size(161, 47);
			this->tbPlate->TabIndex = 3;
			this->tbPlate->TextChanged += gcnew System::EventHandler(this, &MainForm::textBox1_TextChanged);
			// 
			// pbPlate
			// 
			this->pbPlate->Location = System::Drawing::Point(36, 334);
			this->pbPlate->Name = L"pbPlate";
			this->pbPlate->Size = System::Drawing::Size(161, 50);
			this->pbPlate->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pbPlate->TabIndex = 4;
			this->pbPlate->TabStop = false;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(461, 449);
			this->Controls->Add(this->pbPlate);
			this->Controls->Add(this->tbPlate);
			this->Controls->Add(this->btRecognize);
			this->Controls->Add(this->btBrowse);
			this->Controls->Add(this->pbImg);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbImg))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPlate))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: char* ConvertStringtoChar(System::String ^str)					//ÉP KIỂU TRONG KHI LOAD  IMG
		{
			char *str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
			return str2;
		}
	private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
			
	private: System::Void Browse_Click(System::Object^  sender, System::EventArgs^  e) {
		OpenFileDialog^ dlg = gcnew OpenFileDialog;				//MỞ TỆP TRONG MÁY TÍNH
		dlg->Filter = "Image (*.bmp; *.jpg; *.jpeg; *.png) |*.bmp; *.jpg; *.jpeg; *.png|All files (*.*)|*.*||";				//	chọn các kiểu ảnh	
		if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::Cancel)
		{
			
			Img = cvLoadImage(ConvertStringtoChar(dlg->FileName));		//LOAD IMAGE VÀO IMG
		Bitmap ^bmp = gcnew Bitmap(dlg->FileName);					//LOAD IMG LÊN PIXTUREBOX
		pbImg->Image = bmp;
		pbImg->Refresh();
		}
	}
	private: System::Void Recognize_Click(System::Object^  sender, System::EventArgs^  e) {
		PlateTracking pt;

		if (!Img) 
		{
			MessageBox::Show("NO IMAGE LOADED", "ERROR", MessageBoxButtons::OK);
			return;
		}
		IplImage *resizeImg = cvCreateImage(cvSize(800, 600), Img->depth, Img->nChannels);	// Anh resize
		cvResize(Img, resizeImg);		//ẢNH ĐÃ ĐƯỢC RESIZE GIẢM KÍCH THƯỚC ĐỂ SPEED UP
		
			//cvShowImage("resize",resizeImg);
			// CHUYỂN SANG ẢNH XÁM
	
			IplImage *grayImg = cvCreateImage(cvGetSize(resizeImg), IPL_DEPTH_8U,1);

			cvCvtColor(resizeImg, grayImg, CV_RGB2GRAY);
			//cvShowImage("grayimg", grayImg);
			cvNormalize(grayImg, grayImg, 0, 255, CV_MINMAX);
	//		pt.ImageRestoration(grayImg);
	
	      // pt.TrackPlate(resizeImg);
		   IplImage * plate = pt.TrackPlate(resizeImg);
		   if(plate)
		   {
			   Recognize rc ;
			   vector<IplImage *>  cVector =rc.FindCharacter(plate); 

		   }
		   else MessageBox::Show(" Not Found Plate", L"error", MessageBoxButtons::OK, MessageBoxIcon::Error);

	}
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
}
};
}
