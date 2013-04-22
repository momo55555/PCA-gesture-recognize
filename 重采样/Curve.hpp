#ifndef _CURVE_H
#define _CURVE_H

// This Header File can only handle single-skeleton data, now there's no obvious need for multi-skeleton data processing, so I did not add that function

#include "Speed.hpp"

/*
    Time: 01-03-2013
    @Author: Rudy Snow
    Description: Key frames extraction algorithm based on hierarchical curve simplification method
*/

using namespace std;

// Preprocess function, in order to calculated a key frame set with much redundancy
void preProcess()
{
	int i = 0;

	// add the first and the last frame to the set
	cNumTrue[0] = 1;
	cNumTrue[numOfFrames-1] = 1;
	numC +=2;

	// Calculate extreme points on each axis of the curve, and these points make up the Pre-Key Frame set
	for (i = 1; i < numOfFrames-1; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			if ((oriMove[i-1].angles[j] < oriMove[i].angles[j] && oriMove[i+1].angles[j] < oriMove[i].angles[j])
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

// Just a function to calculate square...
float power2(float n)
{
	return n * n;
}

// Calculate the distance from a point to a line in high-dimensional space
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

// Calculate the distance between two points in high-dimensional space
float realDis(int s, int e)
{
	float res = 0;
	for (int i = 0; i < 12; ++i)
	{
		res += (oriMove[s].angles[i]-oriMove[e].angles[i]) * (oriMove[s].angles[i]-oriMove[e].angles[i]);
	}
	return sqrt(res);
}

// Upper curve simplification, check related papers for more details on the algorithm
int curveUp(int s, int e)
{
	vector<int>::iterator vi;
	float maxDis = -1.0f;
	int maxInd = 0;

	// Calculate the furthest point from the line consists of the beginning point and the ending point in the subset
	for (vi = subset.begin(); vi != subset.end(); ++vi)
	{
		float dis = getDis(oriMove[s], oriMove[e], oriMove[*vi]);
		if (dis > maxDis)
		{
			maxDis = dis;
			maxInd = *vi;
		}
	}
	
	// If the point satisfied the threshold, add it into the interpolation curve
	float ratio = maxDis / (realDis(s,e));
	if (ratio > delta)
	{
		pairs[maxInd] = e;
		pairs[s] = maxInd;
		return 0;
	}
	else return 1;
}

// Lower curve simplification, check related papers for more details on the algorithm
int curveDown(int s, int e)
{
	vector<int>::iterator vi;
	float maxDis = -1.0f;
	int maxInd = 0;

	// Calculate the furthest point from the line consists of the beginning point and the ending point in the subset
	for (vi = subset.begin(); vi != subset.end(); ++vi)
	{
		float dis = getDis(oriMove[s], oriMove[e], oriMove[*vi]);
		if (dis > maxDis)
		{
			maxDis = dis;
			maxInd = *vi;
		}
	}

	// If the point satisfied the threshold, add it into the interpolation curve
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

// Main curve process
void curveMain()
{
	// Calculate Pre-Key Frame set
	//speedyPreProcess();
	preProcess();

	int dir = 1;			// 1 means the number of calculated frames is larger than required key frame£¬ 0 means vice versa
	int last = 0;			// Clarify that if last iteration result was similar to this one 
	int vibTimes[10000];	// A box that saves that how many times each number appeared during iteration
	int bestAns = 10000;	// Record the best calculated frame numbers
	
	memset(vibTimes, 0, 10000*sizeof(int));

	// The Process of iteration
	while (true)
	{		
		// Initialization
		for (int i = 0; i < numOfFrames; ++i)
		{
			pairs[i] = -1;
		}
		pairs[0] = numOfFrames - 1;
		pairs[numOfFrames-1] = 0;
		int trackMark = 0;
		memcpy(c_num, cNumTrue, numOfFrames*sizeof(int));		// Copy the original key-frame's hash-information to current array


		while (pairs[trackMark] != 0)
		{
			subset.clear();
			int flag = 0;

			// Calculate if there's key frame between current two points in the curve
			for (int i = trackMark + 1; i < pairs[trackMark]; ++i)
			{
				if (c_num[i] == 1)
				{
					flag = 1;
					subset.push_back(i);
				}
			}

			// If certain key frames exisits, goto curveUp
			if (flag == 1)
			{
				// If this segment of curve could not be simplified any more, come to the next segment
				int f = curveUp(trackMark, pairs[trackMark]);
				if (f == 1) trackMark = pairs[trackMark];
			}
			// If not, goto curveDown
			else 
			{
				// Get all the frame between current endpoints of the curve and add them into subset
				for (int i = trackMark; i <= pairs[trackMark]; ++i)
				{
					subset.push_back(i);
				}
				// If certain point couldn't be found to join the curve , com to the next segment
				int f = curveDown(trackMark, pairs[trackMark]);
				if (f == 1) trackMark = pairs[trackMark];
			}
		}

		// Calculate the temporary key frames generated from this iteration
		keyFrames.clear();
		int numOfKeyFrames = 0;
		for (int i = 0; i < numOfFrames; ++i)
		{
			if (pairs[i] != -1)
			{
				++numOfKeyFrames;
				keyFrames.insert(i);
			}
		}

		// If the calculated number of key frames satisfied the required number, iteration can be ended
		if (numOfKeyFrames == KEY_FRAMES)
		{
			set<int>::iterator si;
			realKeyFrames.clear();
			for (si = keyFrames.begin(); si != keyFrames.end(); si++)
			{
				realKeyFrames.insert(*si);
			}
			break;
		}

		// Adjust the value of delta based on certain situations(For more details check related papers)
		if (numOfKeyFrames >  KEY_FRAMES)
		{
			if (last != 0)
			{
				if (last == 1)
				{
					dDeltaU = sqrt(1 - power2(dDeltaU-1));
				}
				else dDeltaU = 1 - sqrt(1-dDeltaU*dDeltaU);
			}
			delta = delta * (1+dDeltaU);
			last = 1;
		}
		else
		{
			if (last != 0)
			{
				if (last == -1)
				{
					dDeltaU = sqrt(1-power2(dDeltaD-1));
				}
				else dDeltaU = 1 - sqrt(1-dDeltaU*dDeltaD);
			}
			delta = delta * (1-dDeltaD);
			last = -1;
		}
		
		// Always keep the best result recorded
		if (abs(numOfKeyFrames-KEY_FRAMES) < abs(bestAns-KEY_FRAMES))
		{
			bestAns = numOfKeyFrames;
			realKeyFrames.clear();
			set<int>::iterator si;
			for (si = keyFrames.begin(); si != keyFrames.end(); si++)
			{
				realKeyFrames.insert(*si);
			}
		}
		
		// If iteration couldn't end normally and one certain number's iteration-num is larger than 30
		vibTimes[numOfKeyFrames]++;
		if (vibTimes[numOfKeyFrames] > 30)
		{
			srand(time(0));
			int tmp = bestAns;

			// If current best result is larger, choose some points to make a linear interpolation randomly
			if (bestAns > KEY_FRAMES)
			{
				int delNum = 0;
				for (int i = 0; i < tmp - KEY_FRAMES; ++i)
				{
					delNum = rand() % (bestAns-3) + 1;
					set<int>::iterator si;
					int j;
					for (j = 0, si = realKeyFrames.begin(); si != realKeyFrames.end(); si++, ++j)
					{
						if(j == delNum)	
						{
							break;
						}
					}
					set<int>::iterator si1 = si;
					set<int>::iterator si2 = ++si;
					si--;
					for (int k = 0; k < 12; ++k)
					{
						oriMove[*si1].angles[k] = (oriMove[*si1].angles[k] + oriMove[*si2].angles[k]) / 2.0f;
					}
					bestAns--;
					realKeyFrames.erase(si2);
				}
			}
			// If current best result is smaller, choose some points to be deleted randomly
			else
			{
				int addNum = 0;
				for (int i = 0; i < KEY_FRAMES - tmp;)
				{
					addNum = rand() % (bestAns-1);
					set<int>::iterator si;
					int j;
					for (j = 0, si = realKeyFrames.begin(); si != realKeyFrames.end(); si++, ++j)
					{
						if(j == addNum)	
						{
							break;
						}
					}
					set<int>::iterator si1 = si;
					set<int>::iterator si2 = ++si;
					si--;
					if ((*si2) - (*si1) > 1)
					{
						int inc = (rand() % ((*si2)-(*si1))-1) + 1 + (*si1);
						if (realKeyFrames.find(inc) != realKeyFrames.end()) continue;
						realKeyFrames.insert(inc);
						bestAns++;
						++i;
					}
				}
			}
			break;
		}
	}
}

#endif