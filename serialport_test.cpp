// main.cpp : Defines the entry point for the console application.  
// 【第二部分完成】 
//2016年5月28日20:04:11
//不知为啥就好了，一个下午啊，天哪噜，简直要疯了我都！！！！！！！！！！！！！！！！！！！！！！！！！！
#include "stdafx.h"  
#include "serialport1.h"  
#include <iostream>  

int _tmain(int argc, _TCHAR* argv[])
{

	CSerialPort mySerialPort;

			if (!mySerialPort.InitPort(1))
		{
			std::cout << "initPort fail !" << std::endl;
		}
		else
		{
			std::cout << "initPort success !" << std::endl;
		}

		if (!mySerialPort.OpenListenThread())
		{
			std::cout << "OpenListenThread fail !" << std::endl;
		}
		else
		{
			std::cout << "OpenListenThread success !" << std::endl;
		}
	
		int temp;
		std::cin >> temp;
	
	return 0;
}