#ifndef _RECGICA_H
#define _RECGICA_H

#include "ICA.hpp"

/*
    Time: 04-19-2013
    @Author: Rudy Snow
    Description: Recognizing Process of ICA
*/

int theGesNum1 = numRecords;		// Pointer which point to the start point of the gesture to be recognized in the gesture pool
double everyDis1[numRecords];

// Calculate the Cosine distance between two vectors
double distance2(itpp::vec x, itpp::vec y)
{
	double dis1 = 0, dis2 = 0, dis3 = 0;
	for (int i = 0; i < x.size(); ++i)
	{
		dis1 += x(i) * y(i);
	}
	for (int i = 0; i < x.size(); ++i)
	{
		dis2 += x(i) * x(i);
		dis3 += y(i) * y(i);
	}
	dis2 = sqrt(dis2);
	dis3 = sqrt(dis3);
	double res =  dis1 / (dis2*dis3);
	return res;
}

// Calculate threshold to be used for judging if one gesture is a new gesture(Deprecated, not called)
double calculateTheta1()
{
	double maxDis = -1000000000.0f;
	for (int i = 0; i < numRecords; ++i)
	{
		for (int j = i + 1; j < numRecords; ++j)
		{
			double dis = distance2(dataProjectionICA[i], dataProjectionICA[j]);
			dis = 100000 * (dis);
			if (dis > maxDis) maxDis = dis;
		}
	}
	return maxDis;
}

// Calculate the distance between each two vectors
void calculateEveryDis1(itpp::vec theGesVec)
{
	for (int i = 0; i < numRecords; ++i)
	{
		everyDis1[i] = distance2(dataProjectionICA[i], theGesVec);
	}
}

void recogGes1()
{
	cout << "**********************************************" << endl;
	cout << "Recognizing Process Start..." << endl << endl;
	
	int corretNum = 0;
	double theta = calculateTheta1();
	vector<double> mean_Ges = pca.get_mean_values();

	// Get gestures to be recognized
	for (int k  = theGesNum1; k < theGesNum1 * 3; ++k)
	{
		readKf(k);
		itpp::vec theGesVec(numVariables);
		for (int j = 0; j < numVariables; ++j)
		{
			theGesVec(j) = singleVec[j] - mean_Ges[j];
		}

		// Project gesture to be recognized to the ICA space
		itpp::vec thisGesVec =  gICA * theGesVec;

		calculateEveryDis1(thisGesVec);

		int flag = 0, maxN = 0;
		double maxD = (everyDis1[0]);
			
		// Get the maximum Cosine distance between gesture projections
		for (int i = 0; i < numRecords; ++i)
		{
			cout << (everyDis1[i]) << endl;
			if ((everyDis1[i]) < theta)
			{
				if (maxD < (everyDis1[i]))
				{
					maxN = i;
					maxD = (everyDis1[i]);
				}
				flag = 1;
			}
		}

		if (k < 2 * theGesNum1)
		{
			cout << "You input the " << k - theGesNum1 + 1 << "-th gesture" << endl;
			if (k - theGesNum1 == maxN) ++corretNum;
		}
		else if (k < 3 * theGesNum1)
		{
			cout << "You input the " << k - 2*theGesNum1 + 1 << "-th gesture" << endl;
			if (k - 2*theGesNum1 == maxN) ++corretNum;
		}
		if (flag == 0)
		{
			cout << "And It's been recognized as a different gesture!" << endl;
		}
		else cout << "And It's been recognized as the " << maxN+1 << "-th gesture." << endl;

		system("pause");
		cout << endl;
	}
	cout << "**********************************************" << endl;

	// Calculate the correct ratio in the recognizing process
	cout  << "The Whole Accuracy Ratio is: " << double(corretNum) / double(numRecords*2) << endl << endl;
}

#endif