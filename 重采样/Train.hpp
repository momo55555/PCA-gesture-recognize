#ifndef _TRAIN_H
#define _TRAIN_H

#include <iostream>
#include <fstream>
#include <armadillo>
#include "Curve.hpp"
#include "pca.hpp"

/*
    Time: 03-20-2013
    @Author: Rudy Snow
    Description: Training Process of PCA
*/

using namespace std;
using namespace arma;

const int numVariables = 270;	// 数据向量中变量的数目
const int numRecords = 6;	// 数据集的总数
const double alpha = 0.9999;	// 特征值能量贡献率

float singleVec[numVariables] = {0};
stats::pca pca(numVariables);
mat gEigen_Ges;
vector<vec> dataProjection;

// 关键帧文件
char kfFile[numRecords+1][256] = 
{
	"C:/Users/rudysnow/Desktop/HorizontalGes2KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/VerticalGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/GrabGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/FanGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/CircleGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/WaveGes1KeyFrame.txt",
	"C:/Users/rudysnow/Desktop/HorizontalGes1KeyFrame.txt"
};

// 从数据文件中读取数据，放入Vector中
void readKf(int i)
{
	ifstream inFile(kfFile[i]);
	int num;
	float a1, a2, a3, x1, y1, z1, x2, y2, z2;
	for(int i = 0; i < KEY_FRAMES; ++i)
	{
		inFile >> num >> a1 >> a2 >> a3; //>> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
		singleVec[i*3] = a1;
		singleVec[i*3+1] = a2;
		singleVec[i*3+2] = a3;
		//singleVec[i*9+3] = x1;
		//singleVec[i*9+4] = y1;
		//singleVec[i*9+5] = z1;
		//singleVec[i*9+6] = x2;
		//singleVec[i*9+7] = y2;
		//singleVec[i*9+8] = z2;
	}
	inFile.close();
}

// 用PCA训练数据的主函数
void doPCA()
{
	// 预处理，将数据集读入
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

	//pca.set_do_normalize(false);
	//pca.set_num_retained(numRetain);
	cout << "Solving ..." << endl << endl;
	pca.solve();	// 计算PCA

	//计算能量值占到99.99%的特征值
	auto eigenvalues = pca.get_eigenvalues();
	
	//vector<double>::iterator vTmp;
	//for(vTmp = eigenvalues.begin(); vTmp != eigenvalues.end(); ++ vTmp)
	//{
	//	cout << *vTmp << endl;
	//}
	//cout << endl;

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
	cout << i << endl;
	//vector<double> meanTmp = pca.get_mean_values();
	//for (int i = 0; i < numRecords; ++i)
	//{
	//	vector<double>::iterator vi, vi1;
	//	vector<double> recordTmp = pca.get_record(i);
	//	
	//	for (vi = recordTmp.begin(), vi1 = meanTmp.begin(); vi != recordTmp.end(); ++vi, ++vi1)
	//	{
	//		cout << *vi + *vi1 << " ";
	//	}
	//	cout << endl << endl;
	//}


	// 将特征向量组成特征姿态空间
	vector<double> fV = pca.get_eigenvector(0);

	vec fVec(fV);
	mat eigenGes(fVec);
	for(int j = 1; j <= i; ++j)
	{
		vector<double> eV = pca.get_eigenvector(j);
		vec eVec(eV);
		eigenGes.insert_cols(1, eVec);	
	}
	gEigen_Ges = eigenGes;

	dataProjection.clear();

	vector<double> rec1 = pca.get_record(0);	// 此处可直接得到原始数据与平均值的差值
	vec recVec1(rec1);
	vec tmp = eigenGes.t() * recVec1;
	dataProjection.push_back(tmp);
	// 将原始数据与平均值的差值都投影到特征姿态空间
	for (int i = 1; i < numRecords; ++i)
	{
		vector<double> rec = pca.get_record(i);	// 此处可直接得到原始数据与平均值的差值
		vec recVec(rec);
		vec tmp1 = eigenGes.st() * recVec;
		dataProjection.push_back(tmp1);
	}
}

#endif