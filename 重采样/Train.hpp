#ifndef _TRAIN_H
#define _TRAIN_H

#include "Curve.hpp"
#include "pca.hpp"

/*
    Time: 03-20-2013
    @Author: Rudy Snow
    Description: Training Process of PCA
*/

using namespace std;
using namespace arma;

const int numVariables = KEY_FRAMES * 12;	// Number of Variables in each data vector
const int numRecords = 11;	// Number of data vectors
const double alpha = 0.9999;	// Eigen Value energy contribution threshold

int maxNumOfEigenVal = 0;
float singleVec[numVariables] = {0};
stats::pca pca(numVariables);
mat gEigen_Ges;
vector<vec> dataProjection;

// Gesture pool
char kfFile[numRecords*3+14][256] = 
{
	"C:/Users/rudysnow/Desktop/CarrierGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/GanumGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulWalkingGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulPushGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulTechnicalGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceFowardGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceStopGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandHowAreYouGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandPleaseLookAfterGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandILoveYouGes1KeyFrame.txt",

	"C:/Users/rudysnow/Desktop/CarrierGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/GanumGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulWalkingGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulPushGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulTechnicalGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceFowardGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceStopGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandHowAreYouGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandPleaseLookAfterGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandILoveYouGes2KeyFrame.txt",

	"C:/Users/rudysnow/Desktop/CarrierGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/GanumGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulWalkingGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulPushGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulTechnicalGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceFowardGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceStopGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceWaitRightGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandHowAreYouGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandPleaseLookAfterGes3KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HandILoveYouGes3KeyFrame.txt",

	"C:/Users/rudysnow/Desktop/GanumTianyuan1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/GanumTianyuan2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulWalkingTianyuan1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulWalkingTianyuan2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulPushTianyuan1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FoulPushTianyuan2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceFowardTianyuan1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceFowardTianyuan2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceStopTianyuan1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/PoliceStopTianyuan2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HowAreYouTianyuan1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HowAreYouTianyuan2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/ILoveYouTianyuan1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/ILoveYouTianyuan2KeyFrame.txt"

};

// Read key frame from files
void readKf(int i)
{
	ifstream inFile(kfFile[i]);
	int num;
	for (int i = 0; i < KEY_FRAMES; ++i)
	{
		inFile >> num;
		for (int j = 0; j < 12; ++j)
		{
			inFile >> singleVec[i*12+j];
		}
	}
	inFile.close();
}

void doPCA()
{
	for (int i = 0; i < numRecords; ++i)
	{
		readKf(i);
		vector<double> rec(numVariables);
		vector<double>::iterator vi;
		int j = 0;
		for (vi = rec.begin(); vi != rec.end(); ++vi)
		{
			*vi = singleVec[j++];
		}
		pca.add_record(rec);
	}
	cout << "**********************************************" << endl;
	cout << "Solving PCA..." << endl << endl;
	pca.solve();

	// Get needed eigen values based on energy contribution
	auto eigenvalues = pca.get_eigenvalues();
	double sum = 0;
	int i;
	for (i = 0; i < numVariables; ++i)
	{
		sum += eigenvalues[i];
		if (sum >= alpha)
		{
			break;
		}
	}
	maxNumOfEigenVal = i + 1;		// Save the num of eigen values that added up to the 99% of energy

	// From eigen gesture space
	vector<double> fV = pca.get_eigenvector(0);
	vec fVec(fV);
	mat eigenGes(fVec);
	for (int j = 1; j <= i; ++j)
	{
		vector<double> eV = pca.get_eigenvector(j);
		vec eVec(eV);
		eigenGes.insert_cols(j, eVec);	
	}
	gEigen_Ges = eigenGes;

	// Project each gesture key frame data to eigen gesture space
	dataProjection.clear();
	vector<double> rec1 = pca.get_record(0);	// Get difference between original data and mean via this member function directly
	vec recVec1(rec1);
	vec tmp = eigenGes.t() * recVec1;
	dataProjection.push_back(tmp);
	for (int i = 1; i < numRecords; ++i)
	{
		vector<double> rec = pca.get_record(i);
		vec recVec(rec);
		vec tmp1 = eigenGes.st() * recVec;
		dataProjection.push_back(tmp1);
	}
	cout << "PCA has been solved!" << endl << endl;
}

#endif