#include <iostream>
#include <random>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>
#include "DrawSkeleton.hpp"
#include "Recognize.hpp"
#include "RecICA.h"

/*
    Time: 12-05-2012
    @Author: Rudy Snow
    Description: Main Program
*/

using namespace std;

// Thread callback function
//void threadFunc()
//{
//	notify = 0;
//	cout << "Entering data receiving thread" << endl;
//	while (true)
//	{
//		dataReceive();
//	}
//}
//
// This is the Kinect data receive main program
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
//	for (int i = 0; i < 4; ++i)
//	{
//		gesture[i].clear();
//	}
//
//	// Init Kinect environment
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
//	// Create new thread
//	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadFunc, NULL, 0, &threadID);
//
//	// Init OpenGL environment
//	glInit(&argc, argv);
//
//	// Release Kinect resources
//	context.StopGeneratingAll();
//	context.Shutdown();
//
//	return 0;
//}

//// This main program contains the flow of calculating key frames and doing PCA recognition
//int main()
//{
//	//srand(time(0));
//
//	//cout << "**********************************************" << endl;
//	//cout << "Calculating Key Frames..." << endl << endl;
//
//	//char fileName[47][256] = {
//	//	"C:/Users/rudysnow/Desktop/CarrierGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/CarrierGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/CarrierGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulTechnicalGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulTechnicalGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulTechnicalGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/HandHowAreYouGes1.txt",
//	//	"C:/Users/rudysnow/Desktop/HandHowAreYouGes2.txt",
//	//	"C:/Users/rudysnow/Desktop/HandHowAreYouGes3.txt",
//	//	"C:/Users/rudysnow/Desktop/HandPleaseLookAfter1.txt",
//	//	"C:/Users/rudysnow/Desktop/HandPleaseLookAfter2.txt",
//	//	"C:/Users/rudysnow/Desktop/HandPleaseLookAfter3.txt",
//	//	"C:/Users/rudysnow/Desktop/HandILoveYou1.txt",
//	//	"C:/Users/rudysnow/Desktop/HandILoveYou2.txt",
//	//	"C:/Users/rudysnow/Desktop/HandILoveYou3.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumTianyuan1.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumTianyuan2.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingTianyuan1.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingTianyuan2.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushTianyuan1.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushTianyuan2.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardTianyuan1.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardTianyuan2.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopTianyuan1.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopTianyuan2.txt",
//	//	"C:/Users/rudysnow/Desktop/HowAreYouTianyuan1.txt",
//	//	"C:/Users/rudysnow/Desktop/HowAreYouTianyuan2.txt",
//	//	"C:/Users/rudysnow/Desktop/ILoveYouTianyuan1.txt",
//	//	"C:/Users/rudysnow/Desktop/ILoveYouTianyuan2.txt"
//	//};
//
//	//char fileNameKeyFrame[47][256] = {
//	//	"C:/Users/rudysnow/Desktop/CarrierGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/CarrierGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/CarrierGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulTechnicalGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulTechnicalGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulTechnicalGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandHowAreYouGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandHowAreYouGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandHowAreYouGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandPleaseLookAfterGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandPleaseLookAfterGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandPleaseLookAfterGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandILoveYouGes1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandILoveYouGes2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HandILoveYouGes3KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumTianyuan1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/GanumTianyuan2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingTianyuan1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulWalkingTianyuan2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushTianyuan1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/FoulPushTianyuan2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardTianyuan1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceFowardTianyuan2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopTianyuan1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/PoliceStopTianyuan2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HowAreYouTianyuan1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/HowAreYouTianyuan2KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/ILoveYouTianyuan1KeyFrame.txt",
//	//	"C:/Users/rudysnow/Desktop/ILoveYouTianyuan2KeyFrame.txt"
//	//};
//	//
//	//// Calculate each gesture's key frame and save into files
//	//for (int i = 0; i < 47; ++i)
//	//{
//	//	oriMove.clear();
//	//	subset.clear();
//	//	velVec.clear();
//	//	keyFrames.clear();
//	//	realKeyFrames.clear();
//	//	numOfFrames = 0;
//	//	numC = 0;
//	//	aveVel[0] = aveVel[1] = aveVel[2] = aveVel[3] = 0;
//	//	readOriginalFile(fileName[i]);
//	//	curveMain();	
//	//	ofstream fout(fileNameKeyFrame[i]);
//	//	set<int>::iterator si;
//	//	for (si = realKeyFrames.begin(); si != realKeyFrames.end(); si++)
//	//	{
//	//		fout << *si << " ";
//	//		for (int j = 0; j < 11; ++j)
//	//		{
//	//			fout << oriMove[*si].angles[j] << " ";
//	//		}
//	//		fout << oriMove[*si].angles[11] << endl;
//	//	}
//	//	delete[] c_num;
//	//	delete[] pairs;
//	//	delete[] cNumTrue;
//	//	cout << "Having calculated the " << i + 1 << "-th gesture's key frames..." << endl;
//	//}
//	//cout << endl;
//
//    // PCA process
//	doPCA();
//    recogGes();
//
//    system("pause");
//
//    return 0;
//}

int main()
{
	doICA();
	recogGes1();

	system("pause");
	return 0;
}