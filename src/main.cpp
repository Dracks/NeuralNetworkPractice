/* 
 * File:   main.cpp
 * Author: eickot
 *
 * Created on 23 / febrer / 2012, 09:32
 */

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <math.h>

#include "Utils.h"

#include "Normalize.h"
#include "MultiLayerNN.h"

using namespace std;



/*
 * 
 */
int main(int argc, char** argv) {

	
	ofstream debugFile;
	
	
	if (argc!=3){
		cout << "Usage :" << argv[0] << " file.txt outputs.txt" << endl;
		exit(-1);
	}
	vector<vector< float> > data=loadFile(argv[1]);
	
	vector<Normalize*> normalizeList=obtainNormalizations(data);
	
	//cout << "debug:" << endl;
	int i,j;
	/*for (i=0; i<data.size(); i++){
		for (j=0; j<data[i].size()-1; j++){
			//cout << i << '-' << j << ':' << data[i][j] << " ";
			data[i][j]=normalizeList[j]->normalize(data[i][j]);
		}
		//cout << endl;
	}//*/
	
	pair<vector<vector<float> >, vector<vector<float> > > splitedData= splitData(data, 2);
	
	vector<int> layers;
	layers.push_back(2);
	layers.push_back(9);
	layers.push_back(4);
	layers.push_back(1);
	
	MultiLayerNN* NN=new MultiLayerNN(layers);
	//MultiLayerNN* NN=new MultiLayerNN("dumpingNN.csv");
	

	//NN->learn(splitedData.first, splitedData.second,500, 500);
	//NN->learn(splitedData.first, splitedData.second,10000, 500);
	NN->learn(splitedData.first, splitedData.second,1000, 0, 0.15f, 0.3f);

	debugFile.open("debugData.csv", ios::out);
	
	for (i=0; i<data.size(); i++){
		debugFile << i << ";";
		vector<float> result=NN->predict(splitedData.first[i]);
		for (j=0; j<data[i].size(); j++){
			debugFile << data[i][j] << ";";
		}
		for (j=0; j<result.size(); j++){
			if (result[j]<0.5f){
				debugFile << 0;
			} else {
				debugFile << 1;
			}
			//debugFile << result[j];
		}
		debugFile << endl;
	}
	debugFile.close();
	
	//NN->dumpFile("dumpingNN.csv");
	
	data=loadFile(argv[2]);
	
	assert(data.size()!=0);
	//vector<Normalize*> normalizeList=obtainNormalizations(data);
	//int i,j;
	/*for (i=0; i<data.size(); i++){
		for (j=0; j<data[i].size()-1; j++){
			data[i][j]=normalizeList[j]->normalize(data[i][j]);
		}
	}//*/
	debugFile.open("resultsPrediction.csv", ios::out);
	splitedData=splitData(data,2);
	int errorCount=0;
	for (i=0; i<data.size(); i++){
		vector<float> result=NN->predict(splitedData.first[i]);
		
		assert(splitedData.second[i].size()==1);
		assert(result.size()==1);
		
		//cout << "R:" << result[0] << " V:" << splitedData.second[i][0] << endl;
		float r=result[0];
		float v=splitedData.second[i][0];
		
		if (v==0.0f && r>0.5f || v==1.0f && r<0.5f){
			//cout << "R:" << result[0] << " V:" << splitedData.second[i][0] << endl;
			errorCount++;
		}
		
		/*vector<float> result=NN->predict(splitedData.first[i]);
		cout << "ed;" << normalizeList[2]->restore(result[0]) << endl;
		assert(splitedData.second[i].size()==1);
		assert(result.size()==1);
		diferenceSum+=fabs(normalizeList[2]->restore(result[0])-normalizeList[2]->restore(splitedData.second[i][0]));
		total+=normalizeList[2]->restore(splitedData.second[i][0]);
		//cout << "ED:" << diferenceSum << "-" << total << endl;//*/
		
		
		debugFile << i << ";";
		result=NN->predict(splitedData.first[i]);
		for (j=0; j<data[i].size(); j++){
			debugFile << data[i][j] << ";";
		}
		for (j=0; j<result.size(); j++){
			if (result[j]<0.5f){
				debugFile << 0;
			} else {
				debugFile << 1;
			}
			//debugFile << result[j];
		}
		debugFile << endl;
		
	}
	debugFile.close();
	
	cout << "Error:" << 100.0f*errorCount/data.size() << endl;
	
	return 0;
}



