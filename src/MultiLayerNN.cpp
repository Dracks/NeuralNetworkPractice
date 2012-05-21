
#include "MultiLayerNN.h"
#include "Math.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"

//#define kRandValue (0.1f+ 0.15f*randf())
//#define kRandValueOld (0.1f+0.4*randf())
//#define kRandValue (0.18f)
//#define kRandValueOld (0.30)
#define kRandValue learnRate
#define kRandValueOld momentum


using namespace std;

float randf(){
	return (float)rand()/(float)RAND_MAX;
}

//class MultiLayerNN{
MultiLayerNN::MultiLayerNN(std::vector< int > layersCount){
	int i, acumulator;
	acumulator=0;
	epochCounter=0;
	layers.push_back(acumulator);
	for (i=0; i<layersCount.size(); i++){
		acumulator+=layersCount[i];
		layers.push_back(acumulator);
	}
	int size=layers[layers.size()-1];
	for (int i=0; i<size; i++){
		values.push_back(0.0f);
		ovalue.push_back(randf());
	}//*/
	
	for (int i=1; i<layers.size()-1; i++){
		int ini=layers[i-1];
		int change=layers[i];
		int end=layers[i+1];
		for (int j=ini; j<change; j++){
			for (int z=change; z<end; z++){
				weights[z][j]=randf();
			}
		}
	}
}

MultiLayerNN::MultiLayerNN(std::string name){
	ifstream file(name.c_str(), ios::in);
	//file.open;
	string line;
	if (file.is_open()){
		int i;
		int j,z;
		
		getline(file, line);
		cout << "Line:" << line << endl;
		
		istringstream iss(line);
		
		int size;
		int tmpInt;
		float tmpFloat;
		char separator;
		iss >> size >> separator;
		iss >> tmpInt >> separator;
		cout << "Restoring size: " << size << " epochs: " << tmpInt << endl;
		
		epochCounter=tmpInt;
		
		for (i=0; i<size; i++){
			iss >> tmpInt >> separator;
			cout << "Restoring Layer:" << tmpInt << endl;
			layers.push_back(tmpInt);
		}
		
		size=layers[layers.size()-1];
		for (int i=0; i<size; i++){
			values.push_back(0.0f);
			ovalue.push_back(randf());
		}
		
		
		
		int ini,middle, end;
		ini=0;
		for (i=0; i<layers.size()-1; i++){

			end=layers[i+1];
			middle=layers[i];
			cout << "Ini:" << ini << " Middle:" << middle << " end:" << end << endl;
			for (j=ini; j<middle; j++){
				getline(file, line);
				cout << "Line(" << i << "/" << j <<"):" << line << endl;
				
				iss.str(line);
				
				//file << ovalue[j] << ";";
				iss >> tmpFloat >> separator;
				//cout << "OValue" << tmpFloat << endl;
				ovalue[j]=tmpFloat;
				for (z=middle; z<end; z++){
					//file << weights[z][j] << ";";
					iss >> tmpFloat >> separator;
					weights[z][j]=tmpFloat;
					//cout << "Weights z j" << tmpFloat << endl;
				}
				//file << endl;
			}
			ini=middle;
		}
		cout << "Out!" << endl;
		for (j=middle; j<end; j++){
			//file << ovalue[j] << ";" << endl;
			
			getline(file, line);
			cout << "Line - last ("<< j <<"):" << line << endl;
			iss.str(line);
			
			iss >> tmpFloat >> separator;
			ovalue[j]=tmpFloat;
			
		}
		file.close();
	}
}

int MultiLayerNN::getInput(){
	return layers[1];
}
	
std::vector<float> MultiLayerNN::predict(std::vector<float> input){
	vector<float> output;
	//cout << "Debug " << input.size() << " " << layers[1] << endl;
	assert(input.size()==layers[1]);
	//cout <<"predict";
	for (int i=0; i<input.size(); i++){
		//cout << i << ": " << input[i] << " - ";
		values[i]=input[i];
	}
	//cout << endl;
	
	int ini,end,change;
	for (int z=1; z<layers.size()-1; z++){
		ini=layers[z-1];
		change=layers[z];
		end=layers[z+1];
		for (int i=change; i<end; i++){
			float acumulator=0;
			for (int j=ini; j<change; j++){
				//cout << i << "," << j << ":" << values[j] << endl;
				acumulator+=weights[i][j]*values[j];
			}
			values[i]=this->function(acumulator-ovalue[i]);
		}
	}
	//cout << "Response" ;
	for (int i=change; i<end; i++){
		//cout << i << ": " << values[i];
		output.push_back(values[i]);
	}
	//cout << endl;
	return output;
}
	
void MultiLayerNN::learn(std::vector<std::vector<float> > input, std::vector<std::vector<float> > output, int epochNumber,int crossValidation, float learnRate, float momentum){
	
	int numLayers=layers.size();
	int numNeurons=layers[numLayers-1];
	string prefix=getPrefix(epochCounter+epochNumber, crossValidation, learnRate, momentum);
	//cout << "Merda " << input.size() << " Merda2 " << layers[1] << endl;
	assert(input.size()>0);
	assert(input[0].size()==layers[1]);
	assert(output[0].size()==(numNeurons-layers[numLayers-2]));
	
	int ini,change,end;
	int i,j,z;
	
	vector<float> delta(numNeurons,0.0f);
	vector<float> deltaOld(numNeurons,0.0f);
	//vector<float> odiference(numNeurons,0.0f);
	//vector<float> output;
	
	std::map<int, std::map<int, float> > wdiference;
	std::map<int, std::map<int, float> > wdiferenceOld;
	
	vector<float> result;
	
	ofstream csvResultsErrors;
	//ofstream debugFile;
	csvResultsErrors.open((prefix+"trainingErrors.csv").c_str(), ios::out);
	
	for (i=1; i<numLayers-1; i++){
		ini=layers[i-1];
		change=layers[i];
		end=layers[i+1];
		for (int j=ini; j<change; j++){
			for (int z=change; z<end; z++){
				wdiference[z][j]=0.0f;
				wdiferenceOld[z][j]=0.0f;
			}
		}
	}
	
	vector<double> trainingErrors;
	vector<double> testErrors;
	
	bool stepover=false;
	
	int dataLarge=input.size()-crossValidation;
	for (int epoch=0; epoch<epochNumber; epoch++){
		for (int loop=0; loop<dataLarge; loop++){
			int rowData=rand()%dataLarge;
			//cout << rowData << " & " << dataLarge << endl;
			
			result=this->predict(input[rowData]);
			
			int offset=layers[numLayers-2];
			for (i=0; i<output[0].size(); i++){
				int neuron=offset+i;
				//cout << "v["<< neuron<<"] " << values[neuron] << " output " << output[rowData][i];
				delta[neuron]=values[neuron]*(1-values[neuron])*(values[neuron]-output[rowData][i]);
				//cout << " deltaN " << delta[neuron] << endl;
			}
			
			for (z=numLayers-2; z>0; z--){
				ini=layers[z-1];
				change=layers[z];
				end=layers[z+1];
				for (j=ini; j<change; j++){
					delta[j]=values[j]*(1-values[j]);
					float acumulator=0;
					for (int i=change; i<end; i++)
					{
						//cout << "deltai" << delta[i] << " & " << weights[i][j] << endl;
						acumulator+=delta[i]*weights[i][j];
					}
					//cout << "deltaj" << delta[j] << " & " << acumulator << endl;
					delta[j]*=acumulator;
				}
			}
			
			for (z=numLayers-2; z>0; z--){
				ini=layers[z-1];
				change=layers[z];
				end=layers[z+1];
				for (j=ini; j<change; j++){
					for (i=change; i<end; i++){
						wdiference[i][j]=-delta[i]*values[j]*(1-epoch/(epochNumber*2.0f))+wdiferenceOld[i][j]*(epoch/(epochNumber*5.0f));
						wdiference[i][j]=-delta[i]*values[j]*kRandValue+wdiferenceOld[i][j]*kRandValueOld;
						wdiferenceOld[i][j]=wdiference[i][j];
					}
				}
			}
			
			map<int, map<int, float> >::iterator it=wdiference.begin();
			while (it!=wdiference.end()){
				map<int, float>::iterator jt=it->second.begin();
				while (jt!=it->second.end()){
					//cout << it->first << "-"<< jt->first<< endl;
					//cout << "weights["<< it->first <<"]["<< jt->first <<"]: " << weights[it->first][jt->first] << "+" << jt->second << endl;
					weights[it->first][jt->first]+=jt->second;
					jt++;
				}
				it++;
			}
			
			for (i=0; i<ovalue.size(); i++){
				//cout << "oval" << ovalue[i] << " & " << delta[i] << endl;
				//delta[i]=delta[i]*(1-epoch/(epochNumber*2.0f))+deltaOld[i]*(epoch/(epochNumber*5.0f));
				delta[i]=delta[i]*kRandValue+deltaOld[i]*kRandValueOld;
				ovalue[i]+=delta[i];
				deltaOld[i]=delta[i];
			}
		}
		if (epoch%50==0){
			csvResultsErrors<< epoch << ";" ;
			
			int errorCount=0;
			for (j=0; j<dataLarge; j++){
				float v=output[j][0];
				result=this->predict(input[j]);
				float r=result[0];
				//cout << "Debug " << epoch  <<  endl;
				for (i=0; i<input[j].size(); i++){
				//	cout << " ["<< i << "]:" << input[j][i] ;
				}
				//cout << endl;
				//cout << " V:" << v << "R:" << r << endl;
				if (v==0.0f && r>0.5f || v==1.0f && r<0.5f){
					//cout << "Epoch " << epoch <<" V:" << v << "R:" << r << endl;
					errorCount++;
				}
			}
			csvResultsErrors <<(float)errorCount/(float)dataLarge << ";";
			
			errorCount=0;
			for (j=dataLarge; j<input.size(); j++){
				float v=output[j][0];
				result=this->predict(input[j]);
				float r=result[0];
				if (v==0.0f && r>0.5f || v==1.0f && r<0.5f){
					//cout << epoch <<"V:" << v << "R:" << r << endl;
					errorCount++;
				}
			}
			csvResultsErrors <<(float)errorCount/(float) crossValidation;//*/
			csvResultsErrors << endl;
		}//*/
		/*if (epoch>200){
			csvResultsErrors<< epoch << ";" ;
			double errorAcumulation=0.0f;
			for (j=0; j<dataLarge; j++){
				double cubeError=0.0f;
				result=this->predict(input[j]);
				for (i=0; i<result.size(); i++){
					cubeError+=(result[i]-output[j][i])*(result[i]-output[j][i]);
				}
				errorAcumulation+=cubeError;
			}
			csvResultsErrors << errorAcumulation/dataLarge << ";";
			
			
			//trainingErrors.push_back(errorAcumulation);
			//cout << "E:" << errorAcumulation ;
			errorAcumulation=0.0f;
			for (j=dataLarge; j<input.size(); j++){
				double cubeError=0.0f;
				result=this->predict(input[j]);
				for (i=0; i<result.size(); i++){
					cubeError+=(result[i]-output[j][i])*(result[i]-output[j][i]);
				}
				errorAcumulation+=cubeError;
			}
			//testErrors.push_back(errorAcumulation);
			//cout << " -> " << errorAcumulation << endl;
			//cout << "Input size:" << input.size() << endl;
			csvResultsErrors << errorAcumulation/(input.size()-dataLarge) << ";"<< endl;
		}//*/
		
		
		
		/*if (!stepover && trainingErrors.size()>5){
			double added=0.0f;
			for (i=1; i<testErrors.size(); i++){
				added+=testErrors[i-1]-testErrors[i];
			}
			if (added>0.0f){
				stepover=true;
			}
		}
		if (trainingErrors.size()>20 && stepover){
			double trainingProc=0.0f;
			double testProc=0.0f;
			for (i=trainingErrors.size()-20; i<trainingErrors.size(); i++){
				trainingProc+=trainingErrors[i-1]-trainingErrors[i];
				testProc+=testErrors[i-1]-testErrors[i];
			}
			//cout << "Training limits["<< epoch << "]: " << trainingProc << " vs " << testProc << endl;
		}//*/
	}
	epochCounter+=epochNumber;
}
	
float MultiLayerNN::function(float value){
	//cout << "debug-function:" << value << endl;
	float ret=1/(1+exp(-value));
	//cout << "debug-function:" << ret << endl;
	return ret;
}

void MultiLayerNN::debug(){
	
}

void MultiLayerNN::dumpFile(std::string name){
	ofstream output;
	output.open(name.c_str(), ios::out);
	output << layers.size() << ";" << epochCounter << ";" ;
	int i;
	int j,z;
	for (i=0; i<layers.size(); i++){
		output << layers[i] << ";";
	}
	output << endl;
	int ini,middle, end;
	ini=0;
	for (i=0; i<layers.size()-1; i++){
		end=layers[i+1];
		middle=layers[i];
		for (j=ini; j<middle; j++){
			output << ovalue[j] << ";";
			for (z=middle; z<end; z++){
				output << weights[z][j] << ";";
			}
			output << endl;
		}
		ini=middle;
	}
	for (j=middle; j<end; j++){
		output << ovalue[j] << ";" << endl;
	}
	output.close();
}
