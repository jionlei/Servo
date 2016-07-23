#ifndef __FPS_H_
#define __FPS_H_
#include "opencv2/opencv.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <ctype.h>
#include <opencv2/core/core.hpp>
//#include <Windows.h>  


//#include<opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
//#include <iostream>

//using namespace cv;
using namespace std;

//VideoCapture cap;
void fps(cv::VideoCapture );
void delay_ms(void);

#endif // !__FPS_H
