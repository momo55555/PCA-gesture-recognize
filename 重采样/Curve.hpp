#ifndef _CURVE_H
#define _CURVE_H

// 这个头文件只能处理单人采集的骨骼数据，目前没有多人同时采集的需求，所以不添加此功能

#include "Speed.hpp"

/*
    Time: 01-03-2013
    @Author: Rudy Snow
    Description: 基于分层曲线简化的关键帧提取算法
*/

using namespace std;

// 预处理计算出包含冗余的关键帧集合
void preProcess()
{
	int i = 0;
	cNumTrue[0] = 1;
	cNumTrue[numOfFrames-1] = 1;
	numC +=2;

	// 计算曲线在各个分量上的极值点，将这些点组成关键帧
	for (i = 1; i < numOfFrames-1; ++i)
	{
		//if ((oriMove[i-1].rightArmUp < oriMove[i].rightArmUp && oriMove[i+1].rightArmUp < oriMove[i].rightArmUp)
		//	|| (oriMove[i-1].rightArmUp > oriMove[i].rightArmUp && oriMove[i+1].rightArmUp > oriMove[i].rightArmUp)
		//	|| (oriMove[i-1].rightArmDown < oriMove[i].rightArmDown && oriMove[i+1].rightArmDown < oriMove[i].rightArmDown)
		//	|| (oriMove[i-1].rightArmDown > oriMove[i].rightArmDown && oriMove[i+1].rightArmDown > oriMove[i].rightArmDown)
		//	|| (oriMove[i-1].rightArmUp == oriMove[i].rightArmUp && oriMove[i+1].rightArmUp < oriMove[i].rightArmUp)
		//	|| (oriMove[i-1].rightArmUp == oriMove[i].rightArmUp && oriMove[i+1].rightArmUp > oriMove[i].rightArmUp)
		//	|| (oriMove[i-1].rightArmUp < oriMove[i].rightArmUp && oriMove[i+1].rightArmUp == oriMove[i].rightArmUp)
		//	|| (oriMove[i-1].rightArmUp > oriMove[i].rightArmUp && oriMove[i+1].rightArmUp == oriMove[i].rightArmUp)
		//	|| (oriMove[i-1].rightArmDown == oriMove[i].rightArmDown && oriMove[i+1].rightArmDown < oriMove[i].rightArmDown)
		//	|| (oriMove[i-1].rightArmDown == oriMove[i].rightArmDown && oriMove[i+1].rightArmDown > oriMove[i].rightArmDown)
		//	|| (oriMove[i-1].rightArmDown > oriMove[i].rightArmDown && oriMove[i+1].rightArmDown == oriMove[i].rightArmDown)
		//	|| (oriMove[i-1].rightArmDown < oriMove[i].rightArmDown && oriMove[i+1].rightArmDown == oriMove[i].rightArmDown)
		//	|| (oriMove[i-1].betweenArms < oriMove[i].betweenArms && oriMove[i+1].betweenArms < oriMove[i].betweenArms)
		//	|| (oriMove[i-1].betweenArms > oriMove[i].betweenArms && oriMove[i+1].betweenArms > oriMove[i].betweenArms)
		//	|| (oriMove[i-1].betweenArms < oriMove[i].betweenArms && oriMove[i+1].betweenArms == oriMove[i].betweenArms)
		//	|| (oriMove[i-1].betweenArms > oriMove[i].betweenArms && oriMove[i+1].betweenArms == oriMove[i].betweenArms)
		//	|| (oriMove[i-1].betweenArms == oriMove[i].betweenArms && oriMove[i+1].betweenArms < oriMove[i].betweenArms)
		//	|| (oriMove[i-1].betweenArms == oriMove[i].betweenArms && oriMove[i+1].betweenArms > oriMove[i].betweenArms)
		//	|| (oriMove[i-1].eulerArmUp1 < oriMove[i].eulerArmUp1 && oriMove[i+1].eulerArmUp1 < oriMove[i].eulerArmUp1)
		//	|| (oriMove[i-1].eulerArmUp1 > oriMove[i].eulerArmUp1 && oriMove[i+1].eulerArmUp1 > oriMove[i].eulerArmUp1)
		//	|| (oriMove[i-1].eulerArmUp1 < oriMove[i].eulerArmUp1 && oriMove[i+1].eulerArmUp1 == oriMove[i].eulerArmUp1)
		//	|| (oriMove[i-1].eulerArmUp1 > oriMove[i].eulerArmUp1 && oriMove[i+1].eulerArmUp1 == oriMove[i].eulerArmUp1)
		//	|| (oriMove[i-1].eulerArmUp1 == oriMove[i].eulerArmUp1 && oriMove[i+1].eulerArmUp1 < oriMove[i].eulerArmUp1)
		//	|| (oriMove[i-1].eulerArmUp1 == oriMove[i].eulerArmUp1 && oriMove[i+1].eulerArmUp1 > oriMove[i].eulerArmUp1)
		//	|| (oriMove[i-1].eulerArmUp2 < oriMove[i].eulerArmUp2 && oriMove[i+1].eulerArmUp2 < oriMove[i].eulerArmUp2)
		//	|| (oriMove[i-1].eulerArmUp2 > oriMove[i].eulerArmUp2 && oriMove[i+1].eulerArmUp2 > oriMove[i].eulerArmUp2)
		//	|| (oriMove[i-1].eulerArmUp2 < oriMove[i].eulerArmUp2 && oriMove[i+1].eulerArmUp2 == oriMove[i].eulerArmUp2)
		//	|| (oriMove[i-1].eulerArmUp2 > oriMove[i].eulerArmUp2 && oriMove[i+1].eulerArmUp2 == oriMove[i].eulerArmUp2)
		//	|| (oriMove[i-1].eulerArmUp2 == oriMove[i].eulerArmUp2 && oriMove[i+1].eulerArmUp2 < oriMove[i].eulerArmUp2)
		//	|| (oriMove[i-1].eulerArmUp2 == oriMove[i].eulerArmUp2 && oriMove[i+1].eulerArmUp2 > oriMove[i].eulerArmUp2)
		//	|| (oriMove[i-1].eulerArmUp3 < oriMove[i].eulerArmUp3 && oriMove[i+1].eulerArmUp3 < oriMove[i].eulerArmUp3)
		//	|| (oriMove[i-1].eulerArmUp3 > oriMove[i].eulerArmUp3 && oriMove[i+1].eulerArmUp3 > oriMove[i].eulerArmUp3)
		//	|| (oriMove[i-1].eulerArmUp3 < oriMove[i].eulerArmUp3 && oriMove[i+1].eulerArmUp3 == oriMove[i].eulerArmUp3)
		//	|| (oriMove[i-1].eulerArmUp3 > oriMove[i].eulerArmUp3 && oriMove[i+1].eulerArmUp3 == oriMove[i].eulerArmUp3)
		//	|| (oriMove[i-1].eulerArmUp3 == oriMove[i].eulerArmUp3 && oriMove[i+1].eulerArmUp3 < oriMove[i].eulerArmUp3)
		//	|| (oriMove[i-1].eulerArmUp3 == oriMove[i].eulerArmUp3 && oriMove[i+1].eulerArmUp3 > oriMove[i].eulerArmUp3)
		//	|| (oriMove[i-1].eulerArmDown1 < oriMove[i].eulerArmDown1 && oriMove[i+1].eulerArmDown1 < oriMove[i].eulerArmDown1)
		//	|| (oriMove[i-1].eulerArmDown1 > oriMove[i].eulerArmDown1 && oriMove[i+1].eulerArmDown1 > oriMove[i].eulerArmDown1)
		//	|| (oriMove[i-1].eulerArmDown1 < oriMove[i].eulerArmDown1 && oriMove[i+1].eulerArmDown1 == oriMove[i].eulerArmDown1)
		//	|| (oriMove[i-1].eulerArmDown1 > oriMove[i].eulerArmDown1 && oriMove[i+1].eulerArmDown1 == oriMove[i].eulerArmDown1)
		//	|| (oriMove[i-1].eulerArmDown1 == oriMove[i].eulerArmDown1 && oriMove[i+1].eulerArmDown1 < oriMove[i].eulerArmDown1)
		//	|| (oriMove[i-1].eulerArmDown1 == oriMove[i].eulerArmDown1 && oriMove[i+1].eulerArmDown1 > oriMove[i].eulerArmDown1)
		//	|| (oriMove[i-1].eulerArmDown2 < oriMove[i].eulerArmDown2 && oriMove[i+1].eulerArmDown2 < oriMove[i].eulerArmDown2)
		//	|| (oriMove[i-1].eulerArmDown2 > oriMove[i].eulerArmDown2 && oriMove[i+1].eulerArmDown2 > oriMove[i].eulerArmDown2)
		//	|| (oriMove[i-1].eulerArmDown2 < oriMove[i].eulerArmDown2 && oriMove[i+1].eulerArmDown2 == oriMove[i].eulerArmDown2)
		//	|| (oriMove[i-1].eulerArmDown2 > oriMove[i].eulerArmDown2 && oriMove[i+1].eulerArmDown2 == oriMove[i].eulerArmDown2)
		//	|| (oriMove[i-1].eulerArmDown2 == oriMove[i].eulerArmDown2 && oriMove[i+1].eulerArmDown2 < oriMove[i].eulerArmDown2)
		//	|| (oriMove[i-1].eulerArmDown2 == oriMove[i].eulerArmDown2 && oriMove[i+1].eulerArmDown2 > oriMove[i].eulerArmDown2)
		//	|| (oriMove[i-1].eulerArmDown3 < oriMove[i].eulerArmDown3 && oriMove[i+1].eulerArmDown3 < oriMove[i].eulerArmDown3)
		//	|| (oriMove[i-1].eulerArmDown3 > oriMove[i].eulerArmDown3 && oriMove[i+1].eulerArmDown3 > oriMove[i].eulerArmDown3)
		//	|| (oriMove[i-1].eulerArmDown3 < oriMove[i].eulerArmDown3 && oriMove[i+1].eulerArmDown3 == oriMove[i].eulerArmDown3)
		//	|| (oriMove[i-1].eulerArmDown3 > oriMove[i].eulerArmDown3 && oriMove[i+1].eulerArmDown3 == oriMove[i].eulerArmDown3)
		//	|| (oriMove[i-1].eulerArmDown3 == oriMove[i].eulerArmDown3 && oriMove[i+1].eulerArmDown3 < oriMove[i].eulerArmDown3)
		//	|| (oriMove[i-1].eulerArmDown3 == oriMove[i].eulerArmDown3 && oriMove[i+1].eulerArmDown3 > oriMove[i].eulerArmDown3))
		//{
		//	++numC;
		//	cNumTrue[i] = 1;
		//}
		for (int j = 0; j < 12; ++j)
		{
			if((oriMove[i-1].angles[j] < oriMove[i].angles[j] && oriMove[i+1].angles[j] < oriMove[i].angles[j])
			|| (oriMove[i-1].angles[j] > oriMove[i].angles[j] && oriMove[i+1].angles[j] > oriMove[i].angles[j])
			|| (oriMove[i-1].angles[j] < oriMove[i].angles[j] && oriMove[i+1].angles[j] == oriMove[i].angles[j])
			|| (oriMove[i-1].angles[j] > oriMove[i].angles[j] && oriMove[i+1].angles[j] == oriMove[i].angles[j])
			|| (oriMove[i-1].angles[j] == oriMove[i].angles[j] && oriMove[i+1].angles[j] < oriMove[i].angles[j])
			|| (oriMove[i-1].angles[j] == oriMove[i].angles[j] && oriMove[i+1].angles[j] > oriMove[i].angles[j]))
			{
				++numC;
				cNumTrue[i] = 1;
				break;
			}
		}
	}
}

float power2(float n)
{
	return n * n;
}

// 计算一点到另外两点连成直线的距离
float getDis(BodyAngles start, BodyAngles end, BodyAngles p) 
{
	float term1 = 0;
	for (int i = 0; i < 12; ++i)
	{
		term1 += power2(p.angles[i] - start.angles[i]);
	}

	float term2u = 0;
	for (int i = 0; i < 12; ++i)
	{
		term2u += (end.angles[i]-start.angles[i]) * (p.angles[i]-start.angles[i]);
	}
	term2u = power2(term2u);

	float term2d = 0;
	for (int i = 0; i < 12; ++i)
	{
		term2d += power2(end.angles[i]-start.angles[i]);
	}

	float result = sqrt(term1 - term2u/term2d);
	return result;
}

float realDis(int s, int e)
{
	float res = 0;
	for(int i = 0; i < 12; ++i)
	{
		res += (oriMove[s].angles[i]-oriMove[e].angles[i]) * (oriMove[s].angles[i]-oriMove[e].angles[i]);
	}
	return sqrt(res);
}

// 上层曲线简化
int curveUp(int s, int e)
{
	vector<int>::iterator vi;
	float maxDis = -1.0f;
	int maxInd = 0;

	// 计算子集中距离起始点和终止点距离最远的点
	for (vi = subset.begin(); vi != subset.end(); ++vi)
	{
		float dis = getDis(oriMove[s], oriMove[e], oriMove[*vi]);
		if (dis > maxDis)
		{
			maxDis = dis;
			maxInd = *vi;
		}
	}
	
	// 若该点满足阈值，将该点加入曲线
	float ratio = maxDis / (realDis(s,e));
	if (ratio > delta)
	{
		pairs[maxInd] = e;
		pairs[s] = maxInd;
		return 0;
	}
	else return 1;
}

// 下层曲线简化
int curveDown(int s, int e)
{
	vector<int>::iterator vi;
	float maxDis = -1.0f;
	int maxInd = 0;

	// 计算子集中距离起始和终止点距离最远的点
	for (vi = subset.begin(); vi != subset.end(); ++vi)
	{
		float dis = getDis(oriMove[s], oriMove[e], oriMove[*vi]);
		if (dis > maxDis)
		{
			maxDis = dis;
			maxInd = *vi;
		}
	}
	// 若该点满足阈值，将该点加入曲线，并将该点加入预设的关键帧集合
	float ratio = maxDis / (realDis(s,e));
	if (ratio > delta)
	{
		c_num[maxInd] = 1;
		pairs[maxInd] = e;
		pairs[s] = maxInd;
		return 0;
	}
	else return 1;
}

void curveMain()
{
	speedyPreProcess();
	preProcess();

	int dir = 1;			// dir = 1代表帧数多， 0代表帧数少
	int vibNum = 0;			// 迭代震荡次数，用于控制迭代停止
	int restrainNum = 0;
	int deltaFlag = 1;
	float pDelta = delta;
	int counting = 0;
	int last = 0;
	int vibTimes[10000];
	int bestAns = 10000;
	memset(vibTimes, 0, 10000*sizeof(int));

	// 迭代
	while (true)
	{
		for(int i = 0; i < numOfFrames; ++i)
		{
			pairs[i] = -1;
		}
		// 初始化
		pairs[0] = numOfFrames - 1;
		pairs[numOfFrames-1] = 0;
		int trackMark = 0;
		memcpy(c_num, cNumTrue, numOfFrames*sizeof(int));

		while (pairs[trackMark] != 0)
		{
			subset.clear();
			int flag = 0;
			// 计算当前曲线两点之间有无关键帧
			for (int i = trackMark + 1; i < pairs[trackMark]; ++i)
			{
				if (c_num[i] == 1)
				{
					flag = 1;
					subset.push_back(i);
				}
			}
			// 存在关键帧，对关键帧所在的上层曲线进行简化
			if (flag == 1)
			{
				// 若这段曲线无法再简化，则进入下一段曲线
				int f = curveUp(trackMark, pairs[trackMark]);
				if (f == 1) trackMark = pairs[trackMark];
			}
			// 不存在关键帧，进入所有帧所在的下层曲线寻找合适的帧加入预设关键帧集合
			else 
			{
				// 取当前曲线两端点在所有骨骼帧位置中间的所有帧作为子集
				for(int i = trackMark; i <= pairs[trackMark]; ++i)
				{
					subset.push_back(i);
				}
				// 若该段曲线找不到可加入预设关键帧的点，进入下一段曲线
				int f = curveDown(trackMark, pairs[trackMark]);
				if (f == 1) trackMark = pairs[trackMark];
			}
		}

		// 计算本次迭代的关键帧数目
		keyFrames.clear();
		int numOfKeyFrames = 0;
		for (int i = 0; i < numOfFrames; ++i)
		{
			if(pairs[i] != -1)
			{
				++numOfKeyFrames;
				keyFrames.insert(i);
			}
		}

		// 若当前计算所得关键帧数目刚好等于所求关键帧数目，结束迭代
		if (numOfKeyFrames == KEY_FRAMES)
		{
			set<int>::iterator si;
			realKeyFrames.clear();
			for(si = keyFrames.begin(); si != keyFrames.end(); si++)
			{
				realKeyFrames.insert(*si);
			}
			cout << numOfKeyFrames << endl;
			cout << numOfFrames << endl;
			break;
		}

		// 针对迭代过程中出现的各种不同情况动态调整delta阈值
		if (numOfKeyFrames >  KEY_FRAMES)
		{
			if (last != 0)
			{
				if(last == 1)
				{
					dDeltaU = sqrt(1-power2(dDeltaU-1));
				}
				else dDeltaU = 1-sqrt(1-dDeltaU*dDeltaU);
			}
			delta = delta * (1+dDeltaU);
			last = 1;
		}
		else
		{
			if (last != 0)
			{
				if(last == -1)
				{
					dDeltaU = sqrt(1-power2(dDeltaD-1));
				}
				else dDeltaU = 1-sqrt(1-dDeltaU*dDeltaD);
			}
			delta = delta * (1-dDeltaD);
			last = -1;
		}
		
		// 始终保存迭代过程中离所需关键帧数目最接近的一次迭代结果
		if(abs(numOfKeyFrames-KEY_FRAMES) < abs(bestAns-KEY_FRAMES))
		{
			bestAns = numOfKeyFrames;
			realKeyFrames.clear();
			set<int>::iterator si;
			for(si = keyFrames.begin(); si != keyFrames.end(); si++)
			{
				realKeyFrames.insert(*si);
			}
		}
		cout << bestAns << endl;
		
		// 迭代无法正常终止
		vibTimes[numOfKeyFrames]++;
		if(vibTimes[numOfKeyFrames] > 30)
		{
			srand(time(0));
			int tmp = bestAns;
			// 若当前最佳结果比所需关键帧帧数多,随机选几个点进行线性插值
			if(bestAns > KEY_FRAMES)
			{
				int delNum = 0;
				for(int i = 0; i < tmp - KEY_FRAMES; ++i)
				{
					delNum = rand() % (bestAns-3) + 1;
					set<int>::iterator si;
					int j;
					for(j = 0, si = realKeyFrames.begin(); si != realKeyFrames.end(); si++, ++j)
					{
						if(j == delNum)	
						{
							break;
						}
					}
					set<int>::iterator si1 = si;
					set<int>::iterator si2 = ++si;
					si--;
					for(int k = 0; k < 12; ++k)
					{
						oriMove[*si1].angles[k] = (oriMove[*si1].angles[k] + oriMove[*si2].angles[k]) / 2.0f;
					}
					//oriMove[*si1].rightArmDown = (oriMove[*si1].rightArmDown + oriMove[*si2].rightArmDown) / 2.0f;
					//oriMove[*si1].rightArmUp = (oriMove[*si1].rightArmUp + oriMove[*si2].rightArmUp) / 2.0f;
					//oriMove[*si1].betweenArms = (oriMove[*si1].betweenArms + oriMove[*si2].betweenArms) / 2.0f;
					//oriMove[*si1].eulerArmUp1 = (oriMove[*si1].eulerArmUp1 + oriMove[*si2].eulerArmUp1) / 2.0f;
					//oriMove[*si1].eulerArmUp2 = (oriMove[*si1].eulerArmUp2 + oriMove[*si2].eulerArmUp2) / 2.0f;
					//oriMove[*si1].eulerArmUp3 = (oriMove[*si1].eulerArmUp3 + oriMove[*si2].eulerArmUp3) / 2.0f;
					//oriMove[*si1].eulerArmDown1 = (oriMove[*si1].eulerArmDown1 + oriMove[*si2].eulerArmDown1) / 2.0f;
					//oriMove[*si1].eulerArmDown2 = (oriMove[*si1].eulerArmDown2 + oriMove[*si2].eulerArmDown2) / 2.0f;
					//oriMove[*si1].eulerArmDown3 = (oriMove[*si1].eulerArmDown3 + oriMove[*si2].eulerArmDown3) / 2.0f;

					bestAns--;
					realKeyFrames.erase(si2);
				}

			}
			// 若当前最佳结果中的帧数比要求的关键帧帧数少，随机添加线性插值后的帧作为关键帧
			else
			{
				int addNum = 0;
				for(int i = 0; i < KEY_FRAMES - tmp;)
				{
					addNum = rand() % (bestAns-1);
					set<int>::iterator si;
					int j;
					for(j = 0, si = realKeyFrames.begin(); si != realKeyFrames.end(); si++, ++j)
					{
						if(j == addNum)	
						{
							break;
						}
					}
					set<int>::iterator si1 = si;
					set<int>::iterator si2 = ++si;
					si--;
					if((*si2) - (*si1) > 1)
					{
						int inc = (rand() % ((*si2)-(*si1))-1) + 1 + (*si1);
						if(realKeyFrames.find(inc) != realKeyFrames.end()) continue;
						//float tmp = (oriMove[*si1].rightArmDown + oriMove[*si2].rightArmDown) / 2.0f;
						//oriMove[inc].rightArmUp = oriMove[*si1].rightArmUp 
						//	+ (tmp - oriMove[*si1].rightArmDown) * ((oriMove[*si2].rightArmUp - oriMove[*si1].rightArmUp)/(oriMove[*si2].rightArmDown - oriMove[*si1].rightArmDown));
						//oriMove[inc].rightArmDown = tmp;
						realKeyFrames.insert(inc);
						bestAns++;
						++i;
					}
				}
			}
			cout << bestAns << endl;
			cout << numOfKeyFrames << endl;
			break;
		}
	}
}

//int main()
//{
//	srand(time(0));
//	readOriginalFile();
//	curveMain();
//	freopen("C:/Users/rudysnow/Desktop/ClockGesKeyFrame.txt", "w", stdout);
//	for(int i = 0; i < KEY_FRAMES; ++i)
//	{
//		cout << KFs[i] << " " << oriMove[KFs[i]].rightArmUp << " " << oriMove[KFs[i]].rightArmDown << endl;
//	}	
//	delete[] c_num;
//	delete[] pairs;
//	delete[] cNumTrue;
//	delete[] KFs;
//	return 0;
//}

#endif