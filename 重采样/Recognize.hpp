#ifndef _RECG_H
#define _RECG_H

#include "Train.hpp"

/*
    Time: 03-20-2013
    @Author: Rudy Snow
    Description: Recognizing Process of PCA
*/

int theGesNum = numRecords;		// Pointer which point to the start point of the gesture to be recognized in the gesture pool
double everyDis[numRecords];

// Calculate the Euclidean distance between two vectors
double distance1(vec x, vec y)
{
	double dis = 0;
	for (int i = 0; i < x.size(); ++i)
	{
		dis += (x.at(i) - y.at(i)) * (x.at(i) - y.at(i));
	}
	return dis;
}

// Calculate threshold to be used for judging if one gesture is a new gesture
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

// Calculate the distance between each two vectors
void calculateEveryDis(vec theGesVec)
{
	for (int i = 0; i < numRecords; ++i)
	{
		everyDis[i] = distance1(dataProjection[i], theGesVec);
	}
}

void recogGes()
{
	cout << "**********************************************" << endl;
	cout << "Recognizing Process Start..." << endl << endl;
	
	int corretNum = 0;
	double theta = calculateTheta();		
	vector<double> mean_Ges = pca.get_mean_values();
	vec mean_Ges_Vec(mean_Ges);

	// Get gestures to be recognized
	for (int k  = theGesNum; k < theGesNum * 3 + 14; ++k)
	{
		readKf(k);
		vector<double> theGes(numVariables);
		vector<double>::iterator vi;
		int j = 0;
		for (vi = theGes.begin(); vi != theGes.end(); ++vi)
		{
			*vi = singleVec[j++];
		}
		vec theGesVec(theGes);

		// Project gesture to be recognized to the eigen gesture space
		vec thisGesVec = gEigen_Ges.st() * (theGesVec - mean_Ges_Vec);

		calculateEveryDis(thisGesVec);

		// Re-constructed gesture using eigen gesture space
		vec reconGesVec = gEigen_Ges * thisGesVec + mean_Ges_Vec;
		double reconDis = sqrt(distance1(theGesVec, reconGesVec));

		if (reconDis >= theta)
		{
			cout << "This is not a gesture at all..." << endl;
		}
		else if (reconDis < theta)
		{	
			int flag = 0, minN = 0;
			double minD = sqrt(everyDis[0]);
			
			// Get the minimum Euclidean distance between gesture projections
			for (int i = 0; i < numRecords; ++i)
			{
				if (sqrt(everyDis[i]) < theta)
				{
					if (minD > sqrt(everyDis[i]))
					{
						minN = i;
						minD = sqrt(everyDis[i]);
					}
					flag = 1;
				}
			}

			if (k < 2 * theGesNum)
			{
				cout << "You input the " << k - theGesNum + 1 << "-th gesture" << endl;
				if (k - theGesNum == minN) ++corretNum;
			}
			else if (k < 3 * theGesNum)
			{
				cout << "You input the " << k - 2*theGesNum + 1 << "-th gesture" << endl;
				if (k - 2*theGesNum == minN) ++corretNum;
			}
			else
			{
				cout << "This is a Tianyuan's Gesture~" << endl;
				int tmp = k - 3 * theGesNum;
				if (tmp == 0)
				{
					cout << "You input the 2-th gesture" << endl;
				}
				else if (tmp == 1)
				{
					cout << "You input the 2-th gesture" << endl;
				}
				else if (tmp == 2)
				{
					cout << "You input the 3-th gesture" << endl;
				}
				else if (tmp == 3)
				{
					cout << "You input the 3-th gesture" << endl;
				}
				else if (tmp == 4)
				{
					cout << "You input the 4-th gesture" << endl;
				}
				else if (tmp == 5)
				{
					cout << "You input the 4-th gesture" << endl;
				}
				else if(tmp == 6)
				{
					cout << "You input the 6-th gesture" << endl;
				}
				else if (tmp == 7)
				{
					cout << "You input the 6-th gesture" << endl;
				}
				else if (tmp == 8)
				{
					cout << "You input the 7-th gesture" << endl;
				}
				else if (tmp == 9)
				{
					cout << "You input the 7-th gesture" << endl;
				}
				else if (tmp == 10)
				{
					cout << "You input the 9-th gesture" << endl;
				}
				else if (tmp == 11)
				{
					cout << "You input the 9-th gesture" << endl;
				}
				else if (tmp == 12)
				{
					cout << "You input the 11-th gesture" << endl;
				}
				else if (tmp == 13)
				{
					cout << "You input the 11-th gesture" << endl;
				}
			}
			if (flag == 0)
			{
				cout << "And It's been recognized as a different gesture!" << endl;
			}
			else cout << "And It's been recognized as the " << minN+1 << "-th gesture." << endl;
		}

		system("pause");
		cout << endl;
	}
	cout << "**********************************************" << endl;

	// Calculate the correct ratio in the recognizing process
	cout  << "The Whole Accuracy Ratio is: " << double(corretNum) / double(numRecords*2) << endl << endl;
}

#endif