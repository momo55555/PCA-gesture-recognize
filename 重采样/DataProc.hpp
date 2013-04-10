#ifndef _DATAPROC_H
#define  _DATAPROC_H

#include <fstream>
#include <vector>
#include "3DVector.hpp"
#include "Kinect.hpp"

/*
    Time: 12-11-2012
    @Author: Rudy Snow
    Description: 姿态数据收集
*/

using namespace std;
using namespace Vector3;

const double eps1 = 0.025f;

// 上一帧和这一帧的骨骼数据
XnPoint3D nowSkel[4][24], preSkel[4][24];
XnMatrix3X3 nowOri[4][24], preOri[4][24];
// 是否触发
bool triggered[4];
// 是否开始记录姿态数据
bool start[4];
// 判定记录是否应该停止的计时器
int timer[4];
// 记录的姿态信息骨骼帧格式，这里只记录了右手的各种信息
struct Frame
{
	XnPoint3D skelPoints[24];
};
// 分层曲线算法用，记录每帧右手两根骨骼相对身体主骨架的夹角
struct BodyAngles
{
	float angles[12];
};

struct Velocity
{
	int frame;
	float vel[4];
};

// 用向量记录姿态的骨骼帧
vector<Frame> gesture[4];
vector<BodyAngles> movement[4];

// 触发函数
void trigger()
{
	for (int i = 0; i < 4; ++i)
	{
		if (currentUsers[i])
		{
			if(triggered[i] == 1) continue;
			XnVector3D rightHand, rightElbow, rightShoulder;
			XnFloat rightElbowOrientation;

			rightHand = skelPointsIn[i][XN_SKEL_LEFT_HAND-1];
			rightElbow = skelPointsIn[i][XN_SKEL_LEFT_ELBOW-1];
			rightShoulder = skelPointsIn[i][XN_SKEL_LEFT_SHOULDER-1];
			rightElbowOrientation = skelOrientation[i][XN_SKEL_LEFT_ELBOW-1].elements[3];

			// 右手不处于垂直状态时触发
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

// 打印收集的姿态骨骼帧信息，因为考虑到基本都是采集单人骨骼数据所以没有保存用户信息，处理时已单人数据为准
void printGes(int i)
{
	ofstream fout("C:/Users/rudysnow/Desktop/WaveGes1.txt");
	vector<Frame>::iterator vi;

	int n = gesture[i].size();
	int num = 0;

	for (vi = gesture[i].begin(); vi != gesture[i].end(); vi++)
	{
		// 去除每个姿态最后冗余的60帧
		++num;
		if (n - num < 60) break;
		for(int i = 0; i < 24; ++i)
		{
			fout << vi->skelPoints[i].X << " " << vi->skelPoints[i].Y << " " << vi->skelPoints[i].Z << endl;
		}
	}
	fout.close();
	gesture[i].clear();
}

// 打印收集的用于分层曲线算法的骨骼夹角信息，因为考虑到基本都是采集单人骨骼数据所以没有保存用户信息，处理时以单人数据为准
void printMove(int i)
{
	ofstream fout("C:/Users/rudysnow/Desktop/WaveMove1.txt");
	vector<BodyAngles>::iterator vi;
	
	int n = movement[i].size();
	int num = 0;

	for (vi = movement[i].begin(); vi != movement[i].end(); vi++)
	{
		// 去除每个姿态最后冗余的60帧
		++num;
		if(n - num < 60) break;
		for(int i = 0; i < 11; ++i)
		{
			fout << vi->angles[i] << " ";
		}
		fout << vi->angles[11] << endl;
	}
	fout.close();
	movement[i].clear();
}

void record(int i)
{
	// 记录身体当前姿态信息
	Frame f;
	memcpy(f.skelPoints, nowSkel[i], 24*sizeof(XnPoint3D));
	gesture[i].push_back(f);

	// 计算右手两根骨骼相对身体主骨干的夹角并保存
	//BodyAngles b;

	//Vector3f root(nowSkel[i][XN_SKEL_TORSO-1].X - nowSkel[i][XN_SKEL_HEAD-1].X,
	//	nowSkel[i][XN_SKEL_TORSO-1].Y - nowSkel[i][XN_SKEL_HEAD-1].Y,
	//	nowSkel[i][XN_SKEL_TORSO-1].Z - nowSkel[i][XN_SKEL_HEAD-1].Z);
	//Vector3f rightArmUp(nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].X - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].X,
	//	nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].Y - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Y,
	//	nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].Z - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Z);
	//Vector3f rightArmDown(nowSkel[i][XN_SKEL_LEFT_ELBOW-1].X - nowSkel[i][XN_SKEL_LEFT_HAND-1].X,
	//	nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Y - nowSkel[i][XN_SKEL_LEFT_HAND-1].Y,
	//	nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Z - nowSkel[i][XN_SKEL_LEFT_HAND-1].Z);

	//float a1 = angle(rightArmUp, root);
	//float a2 = angle(rightArmDown, root);
	//float a3 = angle(rightArmUp, rightArmDown);
	//
	//// 计算上臂与躯干的欧拉角以及上臂与下臂之间的欧拉角
	//Vector3f armUpShaft = crossProduct(rightArmUp, root);
	//armUpShaft.normalize();
	//double w1 = cos(a1 / 2);
	//double x1 = sin(a1 / 2) * armUpShaft.x;
	//double y1 = sin(a1 / 2) * armUpShaft.y;
	//double z1 = sin(a1 / 2) * armUpShaft.z;
	//double yaw1 = atan2f(2*(w1*x1+y1*z1), (1-2*(x1*x1+y1*y1))) * (180.0/3.14);
	//double pitch1 = asin(2*(w1*y1-z1*x1)) * (180.0/3.14);
	//double roll1 = atan2f(2*(w1*z1+x1*y1), (1-2*(y1*y1+z1*z1))) * (180.0/3.14);


	//Vector3f armDownShaft = crossProduct(rightArmDown, root);
	//armDownShaft.normalize();
	//double w2 = cos(a3 / 2);
	//double x2 = sin(a3 / 2) * armDownShaft.x;
	//double y2 = sin(a3 / 2) * armDownShaft.y;
	//double z2 = sin(a3 / 2) * armDownShaft.z;
	//double yaw2 = atan2f(2*(w2*x2+y2*z2), (1-2*(x2*x2+y2*y2))) * (180.0/3.14);
	//double pitch2 = asin(2*(w2*y2-z2*x2)) * (180.0/3.14);
	//double roll2 = atan2f(2*(w2*z2+x2*y2), (1-2*(y2*y2+z2*z2))) * (180.0/3.14);

	//b.rightArmUp = a1;
	//b.rightArmDown = a2;
	//b.betweenArms = a3;
	//b.eulerArmUp1 = yaw1;
	//b.eulerArmUp2 = pitch1;
	//b.eulerArmUp3 = roll1;
	//b.eulerArmDown1 = yaw2;
	//b.eulerArmDown2 = pitch2;
	//b.eulerArmDown3 = roll2;
	Vector3f rootXY(nowSkel[i][XN_SKEL_TORSO-1].X - nowSkel[i][XN_SKEL_HEAD-1].X, nowSkel[i][XN_SKEL_TORSO-1].Y - nowSkel[i][XN_SKEL_HEAD-1].Y, 0);
	Vector3f rootXZ(nowSkel[i][XN_SKEL_TORSO-1].X - nowSkel[i][XN_SKEL_HEAD-1].X, 0, nowSkel[i][XN_SKEL_TORSO-1].Z - nowSkel[i][XN_SKEL_HEAD-1].Z);
	Vector3f rootYZ(0, nowSkel[i][XN_SKEL_TORSO-1].Y - nowSkel[i][XN_SKEL_HEAD-1].Y, nowSkel[i][XN_SKEL_TORSO-1].Z - nowSkel[i][XN_SKEL_HEAD-1].Z);
	Vector3f rightArmUpXY(nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].X - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].X, nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].Y - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Y, 0);
	Vector3f rightArmUpXZ(nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].X - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].X, 0, nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].Z - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Z);
	Vector3f rightArmUpYZ(0, nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].Y - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Y, nowSkel[i][XN_SKEL_LEFT_SHOULDER-1].Z - nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Z);
	Vector3f rightArmDownXY(nowSkel[i][XN_SKEL_LEFT_ELBOW-1].X - nowSkel[i][XN_SKEL_LEFT_HAND-1].X, nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Y - nowSkel[i][XN_SKEL_LEFT_HAND-1].Y, 0);
	Vector3f rightArmDownXZ(nowSkel[i][XN_SKEL_LEFT_ELBOW-1].X - nowSkel[i][XN_SKEL_LEFT_HAND-1].X, 0, nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Z - nowSkel[i][XN_SKEL_LEFT_HAND-1].Z);
	Vector3f rightArmDownYZ(0, nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Y - nowSkel[i][XN_SKEL_LEFT_HAND-1].Y, nowSkel[i][XN_SKEL_LEFT_ELBOW-1].Z - nowSkel[i][XN_SKEL_LEFT_HAND-1].Z);
	Vector3f leftArmUpXY(nowSkel[i][XN_SKEL_RIGHT_SHOULDER-1].X - nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].X, nowSkel[i][XN_SKEL_RIGHT_SHOULDER-1].Y - nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].Y, 0);
	Vector3f leftArmUpXZ(nowSkel[i][XN_SKEL_RIGHT_SHOULDER-1].X - nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].X, 0, nowSkel[i][XN_SKEL_RIGHT_SHOULDER-1].Z - nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].Z);
	Vector3f leftArmUpYZ(0, nowSkel[i][XN_SKEL_RIGHT_SHOULDER-1].Y - nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].Y, nowSkel[i][XN_SKEL_RIGHT_SHOULDER-1].Z - nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].Z);
	Vector3f leftArmDownXY(nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].X - nowSkel[i][XN_SKEL_RIGHT_HAND-1].X, nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].Y - nowSkel[i][XN_SKEL_RIGHT_HAND-1].Y, 0);
	Vector3f leftArmDownXZ(nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].X - nowSkel[i][XN_SKEL_RIGHT_HAND-1].X, 0, nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].Z - nowSkel[i][XN_SKEL_RIGHT_HAND-1].Z);
	Vector3f leftArmDownYZ(0, nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].Y - nowSkel[i][XN_SKEL_RIGHT_HAND-1].Y, nowSkel[i][XN_SKEL_RIGHT_ELBOW-1].Z - nowSkel[i][XN_SKEL_RIGHT_HAND-1].Z);
	BodyAngles b;
	b.angles[0] = angle(rightArmUpXY, rootXY);
	b.angles[1] = angle(rightArmUpXZ, rootXZ);
	b.angles[2] = angle(rightArmUpYZ, rootYZ);
	b.angles[3] = angle(rightArmDownXY, rootXY);
	b.angles[4] = angle(rightArmDownXZ, rootXZ);
	b.angles[5] = angle(rightArmDownYZ, rootYZ);
	b.angles[6] = angle(leftArmUpXY, rootXY);
	b.angles[7] = angle(leftArmUpXZ, rootXZ);
	b.angles[8] = angle(leftArmUpYZ, rootYZ);
	b.angles[9] = angle(leftArmDownXY, rootXY);
	b.angles[10] = angle(leftArmDownXZ, rootXZ);
	b.angles[11] = angle(leftArmDownYZ, rootYZ);
	movement[i].push_back(b);

	memcpy(preSkel[i], nowSkel[i], 24*sizeof(XnPoint3D));
	memcpy(preOri[i], nowOri[i], 24*sizeof(XnMatrix3X3));
}

// 收集数据的主函数
void collectData()
{
	if (notify == 0) return;
	// 判定是否触发
	//cout << skelPointsIn[0][XN_SKEL_LEFT_HAND].X << endl;
	trigger();

	for (int i = 0; i < 4; ++i)
	{
		if (triggered[i])
		// 已触发
		{
			memcpy(nowSkel[i], skelPointsIn[i], 24*sizeof(XnPoint3D));
			memcpy(nowOri[i], skelOrientation[i], 24*sizeof(XnMatrix3X3));
			// 未开始记录
			if (!start[i])
			{
				// 判断是否仍在触发状态，以此为依据判定进入开始记录状态
				float rightElbowOrientation = nowOri[i][XN_SKEL_LEFT_ELBOW-1].elements[6];
				
				if (abs(rightElbowOrientation-1.0f) < eps1) continue;
				else 
				{
					start[i] = 1;
					// 记录姿态信息
					record(i);
				}
			}
			// 已经开始记录
			else
			{
				// 用计时器判断骨骼是否已经保持不动，如果保持不动停止姿态的记录
				float dx1, dx2, dx3;
				dx1 = nowSkel[i][XN_SKEL_LEFT_HAND-1].X - preSkel[i][XN_SKEL_LEFT_HAND-1].X;
				dx2 = nowSkel[i][XN_SKEL_LEFT_HAND-1].Y - preSkel[i][XN_SKEL_LEFT_HAND-1].Y;
				dx3 = nowSkel[i][XN_SKEL_LEFT_HAND-1].Z - preSkel[i][XN_SKEL_LEFT_HAND-1].Z;
				if(dx1 == 0.0f && dx2 == 0.0f && dx3 == 0.0f) continue;
				float eps = 5.0f;
				if (abs(dx1) < eps && abs(dx2) < eps && abs(dx3) < eps)
				{
					timer[i]++;
				}
				// 如果已保持不动60帧以上
				if (timer[i] > 60)
				{
					cout << "End of Capturing" << endl;
					timer[i] = 0;
					triggered[i] = 0;
					start[i] = 0;
					printGes(i);
					printMove(i);
					continue;
				}
				// 记录姿态信息
				record(i);
			}
		}
	}
	notify = 0;
}

#endif