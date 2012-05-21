//
//  Utils.cpp
//  P2
//
//  Created by Dracks on 21/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

#include "Utils.h"



using namespace std;

vector<vector<float> > loadFile(char* fileName){
	ifstream ifs(fileName, ifstream::in);
	
	float value;
	int lineNumber, column;
	
	string line;
	
	vector<vector<float> > matrix(0, vector<float>(0));
	if (ifs.is_open()){
		while (!ifs.eof()){
			vector<float> row;
			getline(ifs, line);
			//cou << ifs.flags() << endl;
			istringstream iss(line);
			//cout << line << endl;
			bool endOfLine=false;
			//istream tst;
			//row.clear();
			
			while (!endOfLine){
				if (!(iss >> value).fail()){
					//cout << "Value:" << value << endl;
					row.push_back(value);
				} else {
					char c;
					bool wrongFormat=true;
					while (true){
						iss >> c;
						if (c=='#' || c=='\r' || c=='\0' || c=='\n'){
							wrongFormat=false;
							endOfLine=true;
							break;
						} else if (c!=' ' && c!='\t'){
							endOfLine=true;
							break;
						}
					}
					
					if (wrongFormat){
						cerr << "Format error in line " << lineNumber << " from file " << fileName << endl;
					}
				}
			}
			
			if (row.size()>0){
				matrix.push_back(row);
			}
			
			lineNumber++;
		}
		
	} else {
		cerr << "Error opening file: " << fileName << endl;
	}
	return matrix;
}

vector<Normalize*> obtainNormalizations(vector<vector<float> > data){
	vector<float> min,max;
	int i,j;
	vector<Normalize*> ret;
	for (j=0; j<data[0].size(); j++){
		min.push_back(data[0][j]);
		max.push_back(data[0][j]);
	}
	for (i=1; i<data.size(); i++){
		for (j=0; j<data[i].size(); j++){
			if (max[j]<data[i][j])
				max[j]=data[i][j];
			else if (min[j]>data[i][j])
				min[j]=data[i][j];
		}
	}
	for (j=0; j<max.size(); j++){
		//cout << min[j] << ":" << max[j] << endl;
		ret.push_back(new Normalize(min[j],max[j]));
	}
	return ret;
}

pair<vector<vector<float> >, vector<vector<float> > > splitData(vector<vector<float> > data, int inputNumber){
	vector<vector<float> > input(data.size(),vector<float>(0));
	vector<vector<float> > output(data.size(),vector<float>(0));
	
	for (int i=0; i<data.size(); i++){
		int j=0;
		assert(data[i].size()>inputNumber);
		for (;j<inputNumber; j++){
			input[i].push_back(data[i][j]);
		}
		
		for (;j<data[i].size(); j++){
			output[i].push_back(data[i][j]);
		}
	}
	
	return pair<vector<vector<float> >, vector<vector<float> > >(input, output);
	
}

vector<vector<float> > yequalsx(){
	vector<vector<float> > ret(25, vector<float>(3));
	
	for (int i=0; i<5; i++){
		for (int j=0; j<5; j++){
			ret[i+j*5][0]=i;
			ret[i+j*5][1]=j;
			ret[i+j*5][2]=((float)i+((float)j*j));
			cout << ret[i+j*5][0] << "-" << ret[i+j*5][1] << ":" << ret[i+j*5][2] <<endl;
		}
	}
	
	return ret;
}

void print(vector<float> data, vector<Normalize*> n, int offset){
	for (int i=0; i<data.size(); i++){
		cout << "Resultat["<<  i << "]:" << n[offset+i]->restore(data[i]) << endl;
	}
}

void print(vector<float> data){
	for (int i=0; i<data.size(); i++){
		cout << "Resultat["<<  i << "]:" << data[i] << endl;
	}
}