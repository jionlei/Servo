


#include "fps.h"


//using namespace cv;
using namespace std;

void fps(cv::VideoCapture cap)
{
	cv::Mat frame_fps;
	double fps;
	char* str;
	double t = 0;
	
		t = (double)cv::getTickCount();//��ʱ�����ۼ�
		//waitKey(50);
		

		cap >> frame_fps;
		// do something ...
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();//��ʱ��ʱ��
		fps = 1.0 / t;//fps��ȡ
		printf("\nfps = %5f",fps);

		//sprintf(str, "%4f", fps);
		//string fpsStr("FPS:");
		//fpsStr += str;
		//putText(frame_fps, fpsStr, Point(0,30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255));//opencv3.1��opencv2/opencv.hpp��
///
		//imshow("CamShift Demo", frame_fps);
	
	
}


void delay_ms(void)
{
	int i = 10000;
	while (i--);
	return;

}