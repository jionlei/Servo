// main.cpp : Defines the entry point for the console application.  
// ���ڶ�������ɡ� 
//2016��5��28��20:04:11
//��֪Ϊɶ�ͺ��ˣ�һ�����簡�������࣬��ֱҪ�����Ҷ�����������������������������������������������������
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