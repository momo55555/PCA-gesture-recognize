#ifndef _RECG_H
#define _RECG_H

#include "Train.hpp"

int theGesNum = numRecords;
double everyDis[numRecords];

double distance1(vec x, vec y)
{
	double dis = 0;
	for (int i = 0; i < x.size(); ++i)
	{
		dis += (x.at(i) - y.at(i)) * (x.at(i) - y.at(i));
	}
	return dis;
}

double calculateTheta()
{

	double maxDis = -1.0f;
	for (int i = 0; i < numRecords; ++i)
	{
		for (int j = i + 1; j < numRecords; ++j)
		{
			double dis = distance1(dataProjection[i], dataProjection[j]);
			dis = 0.75f * sqrt(dis);
			if (dis > maxDis) maxDis = dis;
		}
	}
	return maxDis;
}

void calculateEveryDis(vec theGesVec)
{
	for (int i = 0; i < numRecords; ++i)
	{
		everyDis[i] = distance1(dataProjection[i], theGesVec);
	}
}

void recogGes()
{
	readKf(theGesNum);
	vector<double> theGes(numVariables);
	vector<double>::iterator vi;
	int j = 0;
	for (vi = theGes.begin(); vi != theGes.end(); ++vi)
	{
		*vi = singleVec[j++];
	}
	vec theGesVec(theGes);
	vector<double> mean_Ges = pca.get_mean_values();
	vec mean_Ges_Vec(mean_Ges);
	vec thisGesVec = gEigen_Ges.st() * (theGesVec - mean_Ges_Vec);
	double theta = calculateTheta();
	calculateEveryDis(thisGesVec);
	vec reconGesVec = gEigen_Ges * thisGesVec + mean_Ges_Vec;
	double reconDis = sqrt(distance1(theGesVec, reconGesVec));

	cout << endl << reconDis << " " << theta << endl << endl;
	if (reconDis >= theta)
	{
		cout << "Not a gesture" << endl;
	}
	else if (reconDis < theta)
	{	
		int flag = 0, minN = 0;
		double minD = sqrt(everyDis[0]);
		for (int i = 0; i < numRecords; ++i)
		{
			cout << sqrt(everyDis[i]) << endl;
			if (sqrt(everyDis[i]) < theta)
			{
				if(minD > sqrt(everyDis[i]))
				{
					minN = i;
					minD = sqrt(everyDis[i]);
				}
				flag = 1;
			}
		}
		if(flag == 0)
		{
			cout << "It's a different gesture!" << endl;
		}
		else cout << "It's the " << minN+1 << "-th gesture." << endl;
	}
}

#endif