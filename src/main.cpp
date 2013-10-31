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

#include <unistd.h>
#include <stdio.h>

#include "Utils.h"

#include "Normalize.h"
#include "MultiLayerNN.h"

using namespace std;

typedef struct typeInputData{
	bool restore;
	bool inew;
	bool learn;
	bool dump;
	bool predict;
	bool write;
	bool normalize;
	
	string newDessin;
	string restoreFile;
	string learnFile;
	string dumpFile;
	string predictFile;
	string writeFile;
	
	float momentum;
	float learnRate;
	int epochs;
	int overTraining;
} ;

int main(int argc, char** argv) {
	
	typeInputData inputData={false,false,false,false,false, false, false, "", "", "", "", "", "", 0,0,0,0};
	
	
	char errflg=0;
	extern char *optarg;
    extern int optind, optopt;
	char c;
	
	
	while ((c = getopt(argc, argv, ":N:R:L:D:P:W:e:o:m:l:n")) != -1) {
		cout << "Debug:" << c ;
		if (optarg!=NULL){
			cout << "->" << optarg;
		}
		cout << endl;
		
        switch(c) {
			case 'R':
				inputData.restore=true;
				inputData.restoreFile=optarg;
				break;
			case 'N':
				inputData.inew=true;
				inputData.newDessin=optarg;
				break;
			case 'L':
				inputData.learn=true;
				inputData.learnFile=optarg;
				break;	
			case 'l':
				inputData.learnRate=atof(optarg);
				break;
			case 'm':
				inputData.momentum=atof(optarg);
				break;
			case 'e':
				inputData.epochs=atoi(optarg);
				break;
			case 'o':
				inputData.overTraining=atoi(optarg);
				break;
			case 'D':
				inputData.dump=true;
				inputData.dumpFile=optarg;
				break;
			case 'P':
				inputData.predict=true;
				inputData.predictFile=optarg;
				break;
			case 'W':
				inputData.write=true;
				inputData.writeFile=optarg;
				break;
			case 'n':
				inputData.normalize=true;
            case ':':
				fprintf(stderr,
						"Option -%c requires an operand\n", optopt);
				errflg++;
				break;
			case '?':
				fprintf(stderr,
						"Unrecognized option: -%c\n", optopt);
				errflg++;
        }
    }
	if (inputData.inew && inputData.restore){
		errflg++;
		cerr << "The -R and -N options are mutual exclusive, please select one" << endl;
	}
	
	if (!inputData.inew && !inputData.restore){
		errflg++;
		cerr << "The option -R or -N is obligatory" << endl;
	}
	
	if (inputData.inew && ! inputData.learn){
		errflg++;
		cerr << "The option -N require the parameter -L" << endl;
	}
	
	cout << "Your actual configuration is:" << endl;
	cout << "restore:" << inputData.restore << " inew:" << inputData.inew << " learn:" << inputData.learn << " dump:" << inputData.dump << " predict:" << inputData.predict << " write:" << inputData.write << endl;
	
	cout << "newDessin;" << inputData.newDessin << " restoreFile;" << inputData.restoreFile << " learnFile;" << inputData.learnFile << " dumpFile;" << inputData.dumpFile << " predictFile;" << inputData.predictFile << " writeFile;" << inputData.writeFile << endl;
	
	cout << "momentum:" << inputData.momentum << " learnRate:" << inputData.learnRate << " epochs:" << inputData.epochs << " overTraining:" << inputData.overTraining << endl;
	
	if (!inputData.dump && ! inputData.predict){
		errflg++;
		cerr << "Do you wish loose time with nothing? please select -D or -P option, thanks!" << endl;
	}
	
    if (errflg) {
        cerr << "usage: [ -R restoreFile | -N dessign ][-L learningFile -e epochs=1000 -o overtraining=0 -l learningRate=0.1 -m momentum=0.3] [-D dumpFile] [-P predictFile -W writeFile]" << endl;
        exit(2);
    }
	
	MultiLayerNN* NN;
	
	if (inputData.inew){
		vector<string> dessign=tokenize(inputData.newDessin, ";,.");
		vector<int> layers;
		for (int i=0; i<dessign.size(); i++){
			layers.push_back(atoi(dessign[i].c_str()));
		}
		NN=new MultiLayerNN(layers);
	} else {
		NN=new MultiLayerNN(inputData.restoreFile);
	}
	
	if (inputData.learn){
		vector<vector< float> > data=loadFile(inputData.learnFile);	
		
		if (inputData.normalize){
			vector<Normalize*> normalizeList=obtainNormalizations(data);
			
			//cout << "debug:" << endl;
			int i,j;
			for (i=0; i<data.size(); i++){
				for (j=0; j<data[i].size(); j++){
					data[i][j]=normalizeList[j]->normalize(data[i][j]);
				}
			}
		}
				
		pair<vector<vector<float> >, vector<vector<float> > > splitedData= splitData(data, NN->getInput());
		NN->learn(splitedData.first, splitedData.second,inputData.epochs, inputData.overTraining, inputData.learnRate, inputData.momentum);
	}
	
	if (inputData.dump){
		NN->dumpFile(inputData.dumpFile);
	}
	
	if (inputData.predict){
		vector<vector< float> > data=loadFile(inputData.predictFile);	
		
		if (inputData.normalize){
			vector<Normalize*> normalizeList=obtainNormalizations(data);
			
			//cout << "debug:" << endl;
			int i,j;
			for (i=0; i<data.size(); i++){
				for (j=0; j<data[i].size(); j++){
					data[i][j]=normalizeList[j]->normalize(data[i][j]);
				}
			}
		}
		
		pair<vector<vector<float> >, vector<vector<float> > > splitedData= splitData(data, NN->getInput());
		
		ofstream writer;
		writer.open(inputData.writeFile.c_str(), ios::out);
		int i,j;
		for (i=0; i<data.size(); i++){
			writer << i << ";";
			vector<float> result=NN->predict(splitedData.first[i]);
			for (j=0; j<data[i].size(); j++){
				writer << data[i][j] << ";";
			}
			for (j=0; j<result.size(); j++){
				if (result[j]<0.5f){
					writer << 0;
				} else {
					writer << 1;
				}
				//debugFile << result[j];
			}
			writer << endl;
		}
		writer.close();
	}

	//ofstream debugFile;
/*	

	
		
	MultiLayerNN* NN=new MultiLayerNN(layers);
	//MultiLayerNN* NN=new MultiLayerNN("dumpingNN.csv");
	

	//NN->learn(splitedData.first, splitedData.second,500, 500);
	//NN->learn(splitedData.first, splitedData.second,10000, 500);

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
	}//* /
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
		//cout << "ED:" << diferenceSum << "-" << total << endl;//* /
		
		
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
	*/
	return 0;
}



