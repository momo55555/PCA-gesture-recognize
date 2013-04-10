#include <iostream>
#include <random>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>
#include "DrawSkeleton.hpp"
#include "Curve.hpp"
#include "pca.hpp"
#include "Recognize.hpp"

/*
    Time: 12-05-2012
    @Author: Rudy Snow
    Description: Main Program
*/

using namespace std;

 //线程函数
//void threadFunc()
//{
//	notify = 0;
//	cout << "Entering data receiving thread" << endl;
//	// 循环接收数据
//	while(true)
//	{
//		dataReceive();
//	}
//}
//
//int main(int argc, char* argv[])
//{
//	HANDLE hThread;
//	DWORD threadID;
//	
//	newData = 0;
//	closeFlag = 0;
//	memset(triggered, 0, sizeof(triggered));
//	memset(start, 0 ,sizeof(start));
//	memset(timer, 0, sizeof(timer));
//	for(int i = 0; i < 4; ++i)
//	{
//		gesture[i].clear();
//	}
//
//	//// 初始化Kinect环境
//	while (true)
//	{
//		if (!setDataEnvironment())
//		{
//			cout << "Kinect not connected!" << endl;
//			Sleep(1000);
//		}
//		else break;
//	}
//
//	// 创建新线程，在线程中接收数据
//	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadFunc, NULL, 0, &threadID);
//
//	glInit(&argc, argv);
//
//	// 释放Kinect资源
//	context.StopGeneratingAll();
//	context.Shutdown();
//
//	return 0;
//}

//int main()
//{
//	srand(time(0));
//	readOriginalFile();
//	curveMain();	
//	freopen("C:/Users/rudysnow/Desktop/WaveGes2KeyFrame.txt", "w", stdout);
//	set<int>::iterator si;
//	for(si = realKeyFrames.begin(); si != realKeyFrames.end(); si++)
//	{
//		cout << *si << " ";
//		for(int i = 0; i < 11; ++i)
//		{
//			cout << oriMove[*si].angles[i] << " ";
//		}
//		cout << oriMove[*si].angles[11] << endl;
//	}
//	delete[] c_num;
//	delete[] pairs;
//	delete[] cNumTrue;
//	system("pause");
//	return 0;
//}

//int main()
//{
//	doPCA();
//	recogGes();
//	system("pause");
//	return 0;
//}