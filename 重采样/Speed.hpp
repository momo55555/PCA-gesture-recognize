#ifndef _SPEED_H
#define _SPEED_H

#include <time.h>
#include <set>
#include "DataProc.hpp"

/*
    Time: 01-03-2013
    @Author: Rudy Snow
    Description: 基于分层曲线简化的关键帧提取算法
*/

using namespace std;

// 要提取的关键帧帧数
#define KEY_FRAMES 60

vector<BodyAngles> oriMove;			// 姿态源数据
vector<int> subset;			// 曲线两点之间的子集点
vector<Velocity> velVec;
set<int> keyFrames;			// 迭代所得关键帧
set<int> realKeyFrames;		// 对迭代所得的“关键帧”进行进一步插值
int numOfFrames = 0;			// 当前姿态数据的帧数
int* pairs;			// 记录分层曲线中各点的连接关系 
int* c_num;			// 记录已提取作为关键帧的帧信息
int* cNumTrue;		// 用于迭代时保存原始姿态曲线极值点信息
float delta = 1.0f;		// 分层曲线算法中判断是否将点加入曲线集合的阈值
float dDeltaU = 0.1f;	// 阈值的增减幅度
float dDeltaD = 0.1f;
int numC;
float aveVel[4] = {0};

// 从保存了骨骼夹角数据的文件中读取
//void readFromFile()
//{
//	ifstream fin("C:/Users/rudysnow/Desktop/WaveMove2.txt");
//	numOfFrames = 0;
//	XnFloat a1, a2, a3, x1, y1, z1, x2, y2, z2;
//	while (fin >> a1 >> a2 >> a3 >> x1 >> y1 >> z1 >> x2 >> y2 >> z2)
//	{
//		BodyAngles b;
//
//		b.rightArmUp = a1;
//		b.rightArmDown = a2;
//		b.betweenArms = a3;
//		b.eulerArmUp1 = x1;
//		b.eulerArmUp2 = y1;
//		b.eulerArmUp3 = z1;
//		b.eulerArmDown1 = x2;
//		b.eulerArmDown2 = y2;
//		b.eulerArmDown3 = z2;
//
//		oriMove.push_back(b);
//		++numOfFrames;
//	}
//	fin.close();
//	pairs = new int[numOfFrames];			// 记得要delete！
//	c_num = new int[numOfFrames];
//	cNumTrue = new int[numOfFrames];
//}

// 从保存原始姿态数据的文件中读取并转化为骨骼夹角
void readOriginalFile()
{
	ifstream fin("C:/Users/rudysnow/Desktop/HorizontalGes1.txt");
	numOfFrames = 0;
	XnPoint3D torso, head, righthand, rightelbow, rightshoulder, lefthand, leftelbow, leftshoulder;
	int flag = 0;
	int flagCalVec = 0;
	int timer = 0;
	XnFloat x, y, z;
	XnFloat px[4] = {0}, py[4] = {0}, pz[4] = {0};
	velVec.clear();
	float totalVel[4] = {0};
	int numVel = 0;
	while (true)
	{
		++timer;
		for (int i = 0; i < 24; ++i)
		{
			if(!(fin >> x >> y >> z))
			{
				flag = 1;
				break;
			}
			// 取得当前帧几个关键点的坐标
			if (i+1 == XN_SKEL_TORSO)
			{
				torso.X = x;
				torso.Y = y;
				torso.Z = z;
			}
			else if (i+1 == XN_SKEL_HEAD)
			{
				head.X = x;
				head.Y = y;
				head.Z = z;
			}
			else if (i+1 == XN_SKEL_LEFT_HAND)
			{
				righthand.X = x;
				righthand.Y = y;
				righthand.Z = z;
			}
			else if (i+1 == XN_SKEL_LEFT_ELBOW)
			{
				rightelbow.X = x;
				rightelbow.Y = y;
				rightelbow.Z = z;
			}
			else if (i+1 == XN_SKEL_LEFT_SHOULDER)
			{
				rightshoulder.X = x;
				rightshoulder.Y = y;
				rightshoulder.Z = z;
			}
			else if (i+1 == XN_SKEL_RIGHT_HAND)
			{
				lefthand.X = x;
				lefthand.Y = y;
				lefthand.Z = z;
			}
			else if (i+1 == XN_SKEL_RIGHT_ELBOW)
			{
				leftelbow.X = x;
				leftelbow.Y = y;
				leftelbow.Z = z;
			}
			else if (i+1 == XN_SKEL_RIGHT_SHOULDER)
			{
				leftshoulder.X = x;
				leftshoulder.Y = y;
				leftshoulder.Z = z;
			}

		}
		if(flag == 1) break;
		if (flagCalVec == 0)
		{
			flagCalVec = 1;
			px[0] = lefthand.X;
			py[0] = lefthand.Y;
			pz[0] = lefthand.Z;
			px[1] = leftelbow.X;
			py[1] = leftelbow.Y;
			pz[1] = leftelbow.Z;
			px[2] = righthand.X;
			py[2] = righthand.Y;
			pz[2] = righthand.Z;
			px[3] = rightelbow.X;
			py[3] = rightelbow.Y;
			pz[3] = rightelbow.Z;
		}
		if(timer == 6)
		{
			timer = 0;
			Velocity vel;
			vel.frame = numOfFrames - rand() % 5;
			vel.vel[0] = Vector3::distance(Vector3f(lefthand.X, lefthand.Y, lefthand.Z), Vector3f(px[0], py[0], pz[0]));
			vel.vel[1] = Vector3::distance(Vector3f(leftelbow.X, leftelbow.Y, leftelbow.Z), Vector3f(px[1], py[1], pz[1]));
			vel.vel[2] = Vector3::distance(Vector3f(righthand.X, righthand.Y, righthand.Z), Vector3f(px[2], py[2], pz[2]));
			vel.vel[3] = Vector3::distance(Vector3f(rightelbow.X, rightelbow.Y, rightelbow.Z), Vector3f(px[3], py[3], pz[3]));
			px[0] = lefthand.X;
			py[0] = lefthand.Y;
			pz[0] = lefthand.Z;
			px[1] = leftelbow.X;
			py[1] = leftelbow.Y;
			pz[1] = leftelbow.Z;
			px[2] = righthand.X;
			py[2] = righthand.Y;
			pz[2] = righthand.Z;
			px[3] = rightelbow.X;
			py[3] = rightelbow.Y;
			pz[3] = rightelbow.Z;
			velVec.push_back(vel);
			++numVel;
			totalVel[0] += vel.vel[0];
			totalVel[1] += vel.vel[1];
			totalVel[2] += vel.vel[2];
			totalVel[3] += vel.vel[3];
		}

		// 通过源数据计算右手与身体的夹角
		Vector3f rootXY(torso.X - head.X, torso.Y - head.Y, 0);
		Vector3f rootXZ(torso.X - head.X, 0, torso.Z - head.Z);
		Vector3f rootYZ(0, torso.Y - head.Y, torso.Z - head.Z);
		Vector3f rightArmUpXY(rightshoulder.X - rightelbow.X, rightshoulder.Y - rightelbow.Y, 0);
		Vector3f rightArmUpXZ(rightshoulder.X - rightelbow.X, 0, rightshoulder.Z - rightelbow.Z);
		Vector3f rightArmUpYZ(0, rightshoulder.Y - rightelbow.Y, rightshoulder.Z - rightelbow.Z);
		Vector3f rightArmDownXY(rightelbow.X - righthand.X, rightelbow.Y - righthand.Y, 0);
		Vector3f rightArmDownXZ(rightelbow.X - righthand.X, 0, rightelbow.Z - righthand.Z);
		Vector3f rightArmDownYZ(0, rightelbow.Y - righthand.Y, rightelbow.Z - righthand.Z);
		Vector3f leftArmUpXY(leftshoulder.X - leftelbow.X, leftshoulder.Y - leftelbow.Y, 0);
		Vector3f leftArmUpXZ(leftshoulder.X - leftelbow.X, 0, leftshoulder.Z - leftelbow.Z);
		Vector3f leftArmUpYZ(0, leftshoulder.Y - leftelbow.Y, leftshoulder.Z - leftelbow.Z);
		Vector3f leftArmDownXY(leftelbow.X - lefthand.X, leftelbow.Y - lefthand.Y, 0);
		Vector3f leftArmDownXZ(leftelbow.X - lefthand.X, 0, leftelbow.Z - lefthand.Z);
		Vector3f leftArmDownYZ(0, leftelbow.Y - lefthand.Y, leftelbow.Z - lefthand.Z);
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
		oriMove.push_back(b);

		numOfFrames++;
	}
	if(timer != 0)
	{
		Velocity vel;
		vel.frame = numOfFrames - 1;
		vel.vel[0] = Vector3::distance(Vector3f(lefthand.X, lefthand.Y, lefthand.Z), Vector3f(px[0], py[0], pz[0]));
		vel.vel[1] = Vector3::distance(Vector3f(leftelbow.X, leftelbow.Y, leftelbow.Z), Vector3f(px[1], py[1], pz[1]));
		vel.vel[2] = Vector3::distance(Vector3f(righthand.X, righthand.Y, righthand.Z), Vector3f(px[2], py[2], pz[2]));
		vel.vel[3] = Vector3::distance(Vector3f(rightelbow.X, rightelbow.Y, rightelbow.Z), Vector3f(px[3], py[3], pz[3]));
		velVec.push_back(vel);
		++numVel;
		totalVel[0] += vel.vel[0];
		totalVel[1] += vel.vel[1];
		totalVel[2] += vel.vel[2];
		totalVel[3] += vel.vel[3];
	}
	
	aveVel[0] = totalVel[0] / float(numVel);
	aveVel[1] = totalVel[1] / float(numVel);
	aveVel[2] = totalVel[2] / float(numVel);
	aveVel[3] = totalVel[3] / float(numVel);

	fin.close();
	pairs = new int[numOfFrames];			// 记得要delete！
	c_num = new int[numOfFrames];
	cNumTrue = new int[numOfFrames];
}

void speedyPreProcess()
{
	memset(cNumTrue, 0, numOfFrames * sizeof(int));
	numC = 0;
	vector<Velocity>::iterator vi;
	for(vi = velVec.begin(); vi != velVec.end(); ++vi)
	{
		for(int i = 0; i < 4; ++i)
		{
			if(vi->vel[i] < aveVel[i])
			{
				++numC;
				cNumTrue[vi->frame] = 1;
				break;
			}
		}
	}
}

#endif