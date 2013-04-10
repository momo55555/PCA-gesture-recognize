#ifndef _CURVE_H
#define _CURVE_H

// ���ͷ�ļ�ֻ�ܴ����˲ɼ��Ĺ������ݣ�Ŀǰû�ж���ͬʱ�ɼ����������Բ���Ӵ˹���

#include "Speed.hpp"

/*
    Time: 01-03-2013
    @Author: Rudy Snow
    Description: ���ڷֲ����߼򻯵Ĺؼ�֡��ȡ�㷨
*/

using namespace std;

// Ԥ����������������Ĺؼ�֡����
void preProcess()
{
	int i = 0;
	cNumTrue[0] = 1;
	cNumTrue[numOfFrames-1] = 1;
	numC +=2;

	// ���������ڸ��������ϵļ�ֵ�㣬����Щ����ɹؼ�֡
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

// ����һ�㵽������������ֱ�ߵľ���
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

// �ϲ����߼�
int curveUp(int s, int e)
{
	vector<int>::iterator vi;
	float maxDis = -1.0f;
	int maxInd = 0;

	// �����Ӽ��о�����ʼ�����ֹ�������Զ�ĵ�
	for (vi = subset.begin(); vi != subset.end(); ++vi)
	{
		float dis = getDis(oriMove[s], oriMove[e], oriMove[*vi]);
		if (dis > maxDis)
		{
			maxDis = dis;
			maxInd = *vi;
		}
	}
	
	// ���õ�������ֵ�����õ��������
	float ratio = maxDis / (realDis(s,e));
	if (ratio > delta)
	{
		pairs[maxInd] = e;
		pairs[s] = maxInd;
		return 0;
	}
	else return 1;
}

// �²����߼�
int curveDown(int s, int e)
{
	vector<int>::iterator vi;
	float maxDis = -1.0f;
	int maxInd = 0;

	// �����Ӽ��о�����ʼ����ֹ�������Զ�ĵ�
	for (vi = subset.begin(); vi != subset.end(); ++vi)
	{
		float dis = getDis(oriMove[s], oriMove[e], oriMove[*vi]);
		if (dis > maxDis)
		{
			maxDis = dis;
			maxInd = *vi;
		}
	}
	// ���õ�������ֵ�����õ�������ߣ������õ����Ԥ��Ĺؼ�֡����
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

	int dir = 1;			// dir = 1����֡���࣬ 0����֡����
	int vibNum = 0;			// �����𵴴��������ڿ��Ƶ���ֹͣ
	int restrainNum = 0;
	int deltaFlag = 1;
	float pDelta = delta;
	int counting = 0;
	int last = 0;
	int vibTimes[10000];
	int bestAns = 10000;
	memset(vibTimes, 0, 10000*sizeof(int));

	// ����
	while (true)
	{
		for(int i = 0; i < numOfFrames; ++i)
		{
			pairs[i] = -1;
		}
		// ��ʼ��
		pairs[0] = numOfFrames - 1;
		pairs[numOfFrames-1] = 0;
		int trackMark = 0;
		memcpy(c_num, cNumTrue, numOfFrames*sizeof(int));

		while (pairs[trackMark] != 0)
		{
			subset.clear();
			int flag = 0;
			// ���㵱ǰ��������֮�����޹ؼ�֡
			for (int i = trackMark + 1; i < pairs[trackMark]; ++i)
			{
				if (c_num[i] == 1)
				{
					flag = 1;
					subset.push_back(i);
				}
			}
			// ���ڹؼ�֡���Թؼ�֡���ڵ��ϲ����߽��м�
			if (flag == 1)
			{
				// ����������޷��ټ򻯣��������һ������
				int f = curveUp(trackMark, pairs[trackMark]);
				if (f == 1) trackMark = pairs[trackMark];
			}
			// �����ڹؼ�֡����������֡���ڵ��²�����Ѱ�Һ��ʵ�֡����Ԥ��ؼ�֡����
			else 
			{
				// ȡ��ǰ�������˵������й���֡λ���м������֡��Ϊ�Ӽ�
				for(int i = trackMark; i <= pairs[trackMark]; ++i)
				{
					subset.push_back(i);
				}
				// ���ö������Ҳ����ɼ���Ԥ��ؼ�֡�ĵ㣬������һ������
				int f = curveDown(trackMark, pairs[trackMark]);
				if (f == 1) trackMark = pairs[trackMark];
			}
		}

		// ���㱾�ε����Ĺؼ�֡��Ŀ
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

		// ����ǰ�������ùؼ�֡��Ŀ�պõ�������ؼ�֡��Ŀ����������
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

		// ��Ե��������г��ֵĸ��ֲ�ͬ�����̬����delta��ֵ
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
		
		// ʼ�ձ������������������ؼ�֡��Ŀ��ӽ���һ�ε������
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
		
		// �����޷�������ֹ
		vibTimes[numOfKeyFrames]++;
		if(vibTimes[numOfKeyFrames] > 30)
		{
			srand(time(0));
			int tmp = bestAns;
			// ����ǰ��ѽ��������ؼ�֡֡����,���ѡ������������Բ�ֵ
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
			// ����ǰ��ѽ���е�֡����Ҫ��Ĺؼ�֡֡���٣����������Բ�ֵ���֡��Ϊ�ؼ�֡
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