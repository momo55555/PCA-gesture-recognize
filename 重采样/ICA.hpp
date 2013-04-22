#ifndef _ICA_H
#define _ICA_H

#include "itpp\signal\fastica.h"
#include "Train.hpp"

/*
    Time: 04-18-2013
    @Author: Rudy Snow
    Description: Training Process of ICA
*/

using namespace std;
using namespace itpp;

vector<itpp::vec> dataProjectionICA;
itpp::mat gICA(7, KEY_FRAMES * 12);		// "S" in X = A*S

void doICA()
{
	// Using PCA to get some parameters
	doPCA();

	vector<double> mean_Ges = pca.get_mean_values();

	// Separate nrIC independent components in nrSamples samples
	int nrSamples = numVariables, nrIC = numRecords;

	cout << "==========================================" << endl;
	cout << "   Calculating ICA using FastICA / IT++   " << endl;
	cout << "==========================================" << endl << endl;

	// Form "X" in X = A*S
	itpp::mat X(nrIC, nrSamples);
	X.clear();

	for (int i = 0; i < numRecords; ++i)
	{
		readKf(i);
		for (int j = 0; j < numVariables; ++j)
		{
			X(i, j) = (singleVec[j]);
		}
	}

	// Instantiate an ICA object with default parameters : SYMM approach and
	// POW3 non-linearity
	// Be sure that :
	//- nrSamples = number of samples = nb of columns of the input matrix
	//- nrIC = number of sensors = nb of rows of the input matrix
	Fast_ICA my_fastica(X);

	// Set number of independent components to separate :
	// By default, this value is taken from the dimension of
	// the input data. This line is for illustration purposes.
	// May help in some cases.
	my_fastica.set_nrof_independent_components(7);
	
	// Set GAUSS non-linearity
	//my_fastica.set_non_linearity(FICA_NONLIN_GAUSS);

	// Perform ICA
	my_fastica.separate();
	
	// Write ICA results to file
	ofstream fout("res.txt");
	fout << my_fastica.get_independent_components() << endl;
	gICA = my_fastica.get_independent_components();

	//ifstream fin("res.txt");
	//for (int i = 0; i < maxNumOfEigenVal; ++i)
	//{
	//	for (int j = 0; j < numVariables; ++j)
	//	{
	//		double ICAtmp;
	//		fin >> gICA(i, j);
	//	}
	//}

	// Get result : separated independent components
	dataProjectionICA.clear();

	// Project each gesture key frame data to ICA space
	for(int i = 0; i < nrIC; ++i)
	{
		itpp::vec v = X.get_row(i);
		for(int j = 0; j < v.size(); ++j)
		{
			v(j) -= mean_Ges[j];
		}
		itpp::vec thisVecTmp = gICA * (v);
		for(int j = 0; j < thisVecTmp.size(); ++j)
		{
			thisVecTmp(j) /= 1000.0f;
		}
		dataProjectionICA.push_back(thisVecTmp);
	}

	cout << "End of Fast_ICA test execution. " << endl << endl;
}

#endif