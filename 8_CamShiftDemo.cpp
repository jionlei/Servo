//--------------------------------------������˵����-------------------------------------------
//	
//------------------------------------------------------------------------------------------------


#include "fps.h"
#include "stdafx.h"  
#include "serialport1.h"  
#include <iostream>  
#include "jinzhi.h"


//����ȫ�ֱ���

cv::Mat image;
bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
cv::Point origin;
cv::Rect selection;
int vmin = 10, vmax = 256, smin = 30;
cv::VideoCapture cap;



//�������ص�

static void onMouse( int event, int x, int y, int, void* )
{
	if( selectObject )
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);

		selection &= cv::Rect(0, 0, image.cols, image.rows);
	}

	switch( event )
	{
	//�˾�����OpenCV2��Ϊ��
	//case CV_EVENT_LBUTTONDOWN:
	//�˾�����OpenCV3��Ϊ��
	case cv::EVENT_LBUTTONDOWN:
		origin = cv::Point(x,y);
		selection = cv::Rect(x,y,0,0);
		selectObject = true;
		break;
	//�˾�����OpenCV2��Ϊ��
	//case CV_EVENT_LBUTTONUP:
	//�˾�����OpenCV3��Ϊ��
	case cv::EVENT_LBUTTONUP:
		selectObject = false;
		if( selection.width > 0 && selection.height > 0 )
			trackObject = -1;
		break;
	}
}



const char* keys =
{
	"{1|  | 0 | camera number}"
};


//-----------------------------------��main( )������--------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-------------------------------------------------------------------------------------------------
int main( int argc, const char** argv )
{



	CSerialPort mySerialPort;
	unsigned char Degree_x =  0, Degree_y = 0, flag_x = 2, flag_y = 3;

	if (!mySerialPort.InitPort(1))
	{
		std::cout << "initPort fail !" << std::endl;
		return 1;
	}
	else
	{
		std::cout << "initPort success !" << std::endl;
	}

	if (!mySerialPort.OpenListenThread())
	{
		std::cout << "OpenListenThread fail !" << std::endl;
		return 2;
	}
	else
	{
		std::cout << "OpenListenThread success !" << std::endl;
	}
	mySerialPort.WriteData(&Degree_x,1);
	mySerialPort.WriteData(&Degree_y,1);

	

	//VideoCapture cap;
	
	cv::Rect trackWindow;//������
	int hsize = 16;
	float hranges[] = {0,180};
	const float* phranges = hranges;
	unsigned char offset_x = 0, offset_y = 0;

	cap.open(0);//opencv the camera

	if( !cap.isOpened() )
	{
		cout << "���ܳ�ʼ������ͷ\n";
	}// if is open

	

	//using namespace cv;
	//using namespace std;
	cv::namedWindow( "Histogram", 0 );
	cv::namedWindow( "CamShift Demo", 0 );
	
	

	cv::setMouseCallback( "CamShift Demo", onMouse, 0 );
	cv::createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
	cv::createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
	cv::createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );

	cv::Mat frame, hsv, hue, mask, hist, histimg = cv::Mat::zeros(200, 320, CV_8UC3), backproj;
	bool paused = false;
	
	for(;;)
	{
		if( !paused )
		{
			cap >> frame;//����cap��Mat frame
			if( frame.empty() )
				break;//�ж��Ƿ�Ϊ��
			//fps(cap);
		}

		frame.copyTo(image);//clone to image
		const int length = image.cols;//����
		const int heigth = image.rows;//���
		printf("\n\n length = %d,heigth= %d", length, heigth);
		if( !paused )
		{
			
			cvtColor(image, hsv, cv::COLOR_BGR2HSV);//

			if( trackObject )
			{
				int _vmin = vmin, _vmax = vmax;

				inRange(hsv, cv::Scalar(0, smin, MIN(_vmin,_vmax)),
					cv::Scalar(180, 256, MAX(_vmin, _vmax)), mask);//���ÿһ�������ǲ�������ѡ��Χ������mask ��ֵͼ��
				int ch[] = {0, 0};
				hue.create(hsv.size(), hsv.depth());//2ά����ĳߴ�Size(cols, rows) .��Size()���캯���������������ڴ����ϸպ÷�ת�����ˡ�
				mixChannels(&hsv, 1, &hue, 1, ch, 1);//mixChannels��Ҫ���ǰ�����ľ��󣨻�������飩��ĳЩͨ����ָ��Ƹ���Ӧ��������󣨻�������飩��ĳЩͨ���У����еĶ�Ӧ��ϵ����fromTo�����ƶ�.
				//ͨ��0 �� 1 ��ͨ��ת��
				
				if( trackObject < 0 )
				{
					cv::Mat roi(hue, selection), maskroi(mask, selection);
					calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);//??
					//�˾�����OpenCV3��Ϊ��
					normalize(hist, hist, 0, 255, cv::NORM_MINMAX);//��һ��
					//�˾�����OpenCV2��Ϊ��
					//normalize(hist, hist, 0, 255, CV_MINMAX);

					trackWindow = selection;//����ѡ����rio
					trackObject = 1;
					//���»���ͼ��
					histimg = cv::Scalar::all(0);
					int binW = histimg.cols / hsize;//lie / ֱ��ͼ�ĸ���
					cv::Mat buf(1, hsize, CV_8UC3);
					for( int i = 0; i < hsize; i++ )
						buf.at<cv::Vec3b>(i) = cv::Vec3b(cv::saturate_cast<uchar>(i*180./hsize), 255, 255);

					//�˾�����OpenCV3��Ϊ��
					cvtColor(buf, buf, cv::COLOR_HSV2BGR);
					//�˾�����OpenCV2��Ϊ��
					//cvtColor(buf, buf, CV_HSV2BGR);

					for( int i = 0; i < hsize; i++ )
					{
						int val = cv::saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
						rectangle( histimg, cv::Point(i*binW,histimg.rows),
							cv::Point((i+1)*binW,histimg.rows - val),
							cv::Scalar(buf.at<cv::Vec3b>(i)), -1, 8 );
					}
				}

				calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
				backproj &= mask;
				cv::RotatedRect trackBox = CamShift(backproj, trackWindow,

				//�˾�����OpenCV3��Ϊ��
					cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1 ));
				//�˾�����OpenCV2��Ϊ��
				//TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));

				if( trackWindow.area() <= 1 )
				{
					int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
					trackWindow = cv::Rect(trackWindow.x - r, trackWindow.y - r,
						trackWindow.x + r, trackWindow.y + r) &
						cv::Rect(0, 0, cols, rows);
				}

				if( backprojMode )
					cvtColor( backproj, image, cv::COLOR_GRAY2BGR );

				//�˾�����OpenCV3��Ϊ��
				ellipse( image, trackBox, cv::Scalar(0,0,255), 3, cv::LINE_AA );//������Բ
				//�˾�����OpenCV2��Ϊ��
				//ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
				int x = trackBox.center.x;
				int y = trackBox.center.y;
				
			//	offset_x = length - x;
			//	offset_y = heigth - y;
				printf("\n x = %d, y = %d\n", x,y);
			//	printf("\n offset_x = %d,offset_y = %d\n", offset_x, offset_y);
				delay_ms();
			//	cvWaitKey(50);
				if ((x>=10) && (x<=540))
				{
					Degree_x = (540 - x) / 3;
					mySerialPort.WriteData(&flag_x,1);
				//	cvWaitKey(50);
					delay_ms();
					mySerialPort.WriteData(&Degree_x,1);
					delay_ms();

				//	mySerialPort.WriteData(&flag_y, 1);
				//	cvWaitKey(50);
				}
			//	cvWaitKey(100);
				if ((y >= 10) && (y <= 480))
				{
					delay_ms();
					Degree_y = y / 2.7;
					mySerialPort.WriteData(&flag_y, 1);
					delay_ms();
			//		cvWaitKey(50);
					mySerialPort.WriteData(&Degree_y, 1);
				//	cvWaitKey(50);
				}


				
			
			//	mySerialPort.WriteData(POFF_Y, sizeof(offset_y));  do not
				// = trackBox.cols / 2;
				//const int heigth = trackBox.rows / 2;
				//int offset_x = 0, offset_y = 0;

			}
		}
		else if( trackObject < 0 )
			paused = false;

		if( selectObject && selection.width > 0 && selection.height > 0 )
		{
			cv::Mat roi(image, selection);
			cv::bitwise_not(roi, roi);
		}
		//������TEXT_help����
		imshow( "CamShift Demo", image );
		imshow( "Histogram", histimg );

		char c = (char)cv::waitKey(10);
		if( c == 27 )
			break;
		switch(c)
		{
		case 'b':
			backprojMode = !backprojMode;
			break;
		case 'c':
			trackObject = 0;
			histimg = cv::Scalar::all(0);
			break;
		case 'h':
			showHist = !showHist;
			if( !showHist )
				cv::destroyWindow( "Histogram" );
			else
				cv::namedWindow( "Histogram", 1 );
			break;
		case 'p':
			paused = !paused;
			break;
		default:
			;
		}
	}
	
	return 0;
}
