#ifndef _DATAPROC_H
#define  _DATAPROC_H

#include "3DVector.hpp"
#include "Kinect.hpp"

/*
    Time: 12-11-2012
    @Author: Rudy Snow
    Description: Collect gesture data
*/

using namespace std;
using namespace Vector3;

const double eps1 = 0.025f;		// Threshold to determine if right hand has been raised

// Skeleton data from last and this frame
XnPoint3D nowSkel[4][24], preSkel[4][24];
XnMatrix3X3 nowOri[4][24], preOri[4][24];

bool triggered[4];		// Record if user[i] is been triggered
bool start[4];		// Record if user[i]'s data collection has been started
int timer[4];		// Timer to judge is gesture has been done

// Skeleton Frame Form
struct Frame
{
	XnPoint3D skelPoints[24];
};

// Data structure for calculating key frames
struct BodyAngles
{
	float angles[12];	// Angles in three axis between arms and body
};

// Data structure for calculating velocity
struct Velocity
{
	int frame;
	float vel[4];
};

vector<Frame> gesture[4];		// Save gestures in a vector container

// Trigger function
void trigger()
{
	for (int i = 0; i < 4; ++i)
	{
		if (currentUsers[i])
		{
			if (triggered[i] == 1) continue;

			XnVector3D rightHand, rightElbow, rightShoulder;
			XnFloat rightElbowOrientation;
			rightHand = skelPointsIn[i][XN_SKEL_LEFT_HAND-1];
			rightElbow = skelPointsIn[i][XN_SKEL_LEFT_ELBOW-1];
			rightShoulder = skelPointsIn[i][XN_SKEL_LEFT_SHOULDER-1];
			rightElbowOrientation = skelOrientation[i][XN_SKEL_LEFT_ELBOW-1].elements[3];

			// Trigger when right arm was raised up
			if (rightElbowOrientation < 0.6f && !triggered[i])
			{
				triggered[i] = 1;
				start[i] = 0;
				memcpy(preSkel[i], skelPointsIn[i], 24*sizeof(XnPoint3D));
				memcpy(preOri[i], skelOrientation[i], 24*sizeof(XnMatrix3X3));
			}
		}
	}
}

// Print gesture skeleton information to file.(Single-user-skeleton only)
void printGes(int i)
{
	ofstream fout("C:/Users/rudysnow/Desktop/ILoveYouTianyuan2.txt");
	vector<Frame>::iterator vi;

	int n = gesture[i].size();
	int num = 0;

	for (vi = gesture[i].begin(); vi != gesture[i].end(); vi++)
	{
		// Get rid of the last 60 redundant frames
		++num;
		if (n - num < 60) break;

		for (int i = 0; i < 24; ++i)
		{
			fout << vi->skelPoints[i].X << " " << vi->skelPoints[i].Y << " " << vi->skelPoints[i].Z << endl;
		}
	}
	fout.close();
	gesture[i].clear();
}

// Record Function
void record(int i)
{
	Frame f;
	memcpy(f.skelPoints, nowSkel[i], 24*sizeof(XnPoint3D));
	gesture[i].push_back(f);

	// Copy data from this frame to last frame 
	memcpy(preSkel[i], nowSkel[i], 24*sizeof(XnPoint3D));
	memcpy(preOri[i], nowOri[i], 24*sizeof(XnMatrix3X3));
}

// Collect Function
void collectData()
{
	if (notify == 0) return;

	// Call triggeer function
	trigger();

	for (int i = 0; i < 4; ++i)
	{
		if (triggered[i])
		{
			memcpy(nowSkel[i], skelPointsIn[i], 24*sizeof(XnPoint3D));
			memcpy(nowOri[i], skelOrientation[i], 24*sizeof(XnMatrix3X3));

			if (!start[i])
			{
				// Determine if recording process should begin
				float rightElbowOrientation = nowOri[i][XN_SKEL_LEFT_ELBOW-1].elements[6];
				if (abs(rightElbowOrientation-1.0f) < eps1) continue;
				else 
				{
					start[i] = 1;
					record(i);
				}
			}
			// If recording process had begun
			else
			{
				// Judging if the body has stopped via timer, if stopped, then stop recording gesture data
				float dx1, dx2, dx3;
				dx1 = nowSkel[i][XN_SKEL_LEFT_HAND-1].X - preSkel[i][XN_SKEL_LEFT_HAND-1].X;
				dx2 = nowSkel[i][XN_SKEL_LEFT_HAND-1].Y - preSkel[i][XN_SKEL_LEFT_HAND-1].Y;
				dx3 = nowSkel[i][XN_SKEL_LEFT_HAND-1].Z - preSkel[i][XN_SKEL_LEFT_HAND-1].Z;

				// This statement is important here, But I forgot the reason... Never mind, the program worked fine with it...
				if (dx1 == 0.0f && dx2 == 0.0f && dx3 == 0.0f) continue;

				float eps = 5.0f;
				if (abs(dx1) < eps && abs(dx2) < eps && abs(dx3) < eps)
				{
					timer[i]++;
				}

				// One gesture has been done
				if (timer[i] > 60)
				{
					cout << "End of Capturing" << endl;
					timer[i] = 0;
					triggered[i] = 0;
					start[i] = 0;
					printGes(i);
					continue;
				}

				// Record gesture data
				record(i);
			}
		}
	}
	notify = 0;
}

#endif