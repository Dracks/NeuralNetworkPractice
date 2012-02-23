/* 
 * File:   main.cpp
 * Author: eickot
 *
 * Created on 23 / febrer / 2012, 09:32
 */

#include <vector>
#include <fstream>
#include "Normalize.h"
#include <string>
#include <sstream>
#include <iostream>

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
			cout << line << endl;
			bool endOfLine=false;
			//istream tst;
			//row.clear();
			
			while (!endOfLine){
				if (!(iss >> value).fail()){
					cout << "Value:" << value << endl;
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

/*
 * 
 */
int main(int argc, char** argv) {
	//char* name=;
	vector<vector< float> > data=loadFile("turbine.txt");
	
	vector<Normalize*> normalizeList=obtainNormalizations(data);
	cout << "debug:" << endl;
	int i,j;
	for (i=0; i<data.size(); i++){
		for (j=0; j<data[i].size(); j++){
			//cout << i << '-' << j << ':' << data[i][j] << " ";
			cout << normalizeList[j]->normalize(data[i][j]) << " ";
		}
		cout << endl;
	}//*/
	
	return 0;
}

