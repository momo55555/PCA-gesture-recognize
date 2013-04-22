#ifndef _KINECT_H
#define _KINECT_H

#include <XnCppWrapper.h>
#include <XnModuleCppInterface.h>
#include <XnTypes.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

/*
    Time: 12-04-2012
    @Author: Rudy Snow
    Description: Initialize Kinect Skeleton Data with OpenNI
*/

/*
	Array index of each joints(Reversed left and right mark, and minus 1 each time use them)
    XN_SKEL_HEAD = 1, XN_SKEL_NECK = 2,
    XN_SKEL_TORSO = 3, XN_SKEL_WAIST = 4,
    XN_SKEL_LEFT_COLLAR = 5, XN_SKEL_LEFT_SHOULDER = 6,
    XN_SKEL_LEFT_ELBOW = 7, XN_SKEL_LEFT_WRIST = 8,
    XN_SKEL_LEFT_HAND = 9, XN_SKEL_LEFT_FINGERTIP =10,
    XN_SKEL_RIGHT_COLLAR =11, XN_SKEL_RIGHT_SHOULDER =12,
    XN_SKEL_RIGHT_ELBOW =13, XN_SKEL_RIGHT_WRIST =14,
    XN_SKEL_RIGHT_HAND =15, XN_SKEL_RIGHT_FINGERTIP =16,
    XN_SKEL_LEFT_HIP =17, XN_SKEL_LEFT_KNEE =18,
    XN_SKEL_LEFT_ANKLE =19, XN_SKEL_LEFT_FOOT =20,
    XN_SKEL_RIGHT_HIP =21, XN_SKEL_RIGHT_KNEE =22,
    XN_SKEL_RIGHT_ANKLE =23, XN_SKEL_RIGHT_FOOT =24
*/

// Some global variables
xn::Context context;
xn::UserGenerator userGenerator;
XnCallbackHandle userCBHandle;
XnCallbackHandle calibCBHandle;
xn::SkeletonCapability skeletonCap = NULL;
XnUInt16 userCounts;
XnUserID* userID;
XnPoint3D skelPointsIn[4][24];
XnMatrix3X3 skelOrientation[4][24];
XnConfidence skelConfidence[4][24];
bool currentUsers[4];
bool newData;
bool closeFlag;
int notify = 0;

// Callback function of user generator: new user
void XN_CALLBACK_TYPE NewUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	if (generator.GetNumberOfUsers() >= 2) return;
	cout << "New user identified: " << user << endl;
	generator.GetSkeletonCap().RequestCalibration( user, FALSE );
}

// Callback function of user generator: lost user
void XN_CALLBACK_TYPE LostUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	cout << "User " << user << " lost" << endl;
	currentUsers[user] = 0;
}

// Callback function of skeleton: calibration start
void XN_CALLBACK_TYPE CalibrationStart(xn::SkeletonCapability& skeleton, XnUserID user, void* pCookie)
{
	cout << "Calibration start for user " << user << endl;
}

// Callback function of skeleton: calibration end
void XN_CALLBACK_TYPE CalibrationEnd(xn::SkeletonCapability& skeleton, XnUserID user, XnCalibrationStatus calibrationError, void* pCookie)
{
	cout << "Calibration complete for user " << user << ", ";
	if (calibrationError == XN_CALIBRATION_STATUS_OK)
	{
		cout << "Success" << endl;
		skeleton.StartTracking(user);
	}
	else
	{
		cout << "Failure" << endl;
		//For the current version of OpenNI, only Psi pose is available
		((xn::UserGenerator*)pCookie)->GetPoseDetectionCap().StartPoseDetection( "Psi", user );
	}
}

int setDataEnvironment()
{
	context.Init();

	if (userGenerator.Create(context) != XN_STATUS_OK) return 0;
	// Register callback functions of user generator
	userGenerator.RegisterUserCallbacks(NewUser, LostUser, NULL, userCBHandle);

	// Register callback functions of skeleton capability
	skeletonCap = userGenerator.GetSkeletonCap();
	skeletonCap.SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
	skeletonCap.SetSmoothing(0.5f);
	skeletonCap.RegisterToCalibrationStart(CalibrationStart, &userGenerator, calibCBHandle);
	skeletonCap.RegisterToCalibrationComplete(CalibrationEnd, &userGenerator, calibCBHandle);

	context.StartGeneratingAll();

	return 1;
}

void dataReceive()
{
	notify = 0;
	while (true)
	{
		if (closeFlag == 1)
		{
			return;
		}
		context.WaitAndUpdateAll();
		memset(currentUsers, 0, sizeof(currentUsers));

		// Get users
		userCounts = userGenerator.GetNumberOfUsers();
		if (userCounts > 0)
		{
			if(notify == 1) continue;

			// Collecting new data
			newData = 1;
			XnUserID* userID = new XnUserID[userCounts];
			userGenerator.GetUsers(userID, userCounts);
			for (int i = 0; i < userCounts; ++i)
			{
				//if is tracking user[i]'s skeleton
				if (skeletonCap.IsTracking(userID[i]))
				{
					currentUsers[userID[i]] = 1;
					XnSkeletonJointTransformation mJointTran;
					XnSkeletonJointOrientation mJointOrien; 
					for (int iter = 0; iter < 24; ++iter)
					{
						//XnSkeletonJoint from 1 to 24
						skeletonCap.GetSkeletonJoint(userID[i], XnSkeletonJoint(iter+1), mJointTran);
						skelPointsIn[userID[i]][iter] = mJointTran.position.position;
						skeletonCap.GetSkeletonJointOrientation(userID[i], XnSkeletonJoint(iter+1), mJointOrien);
						skelOrientation[userID[i]][iter] = mJointOrien.orientation;
						skelConfidence[userID[i]][iter] = mJointOrien.fConfidence;
						notify = 1;
					}
				}
			}
			delete []userID;
		}
	}
}

#endif