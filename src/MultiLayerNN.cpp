
#include "MultiLayerNN.h"
#include "Math.h"
#include <assert.h>
#include <iostream>
#include <fstream>

#define kRandValue (0.4f)
#define kRandValueOld (0.3f)

using namespace std;

float randf(){
	return (float)rand()/(float)RAND_MAX;
}

//class MultiLayerNN{
MultiLayerNN::MultiLayerNN(std::vector< int > layersCount){
	int i, acumulator;
	acumulator=0;
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
	
std::vector<float> MultiLayerNN::predict(std::vector<float> input){
	vector<float> output;
	assert(input.size()==layers[1]);
	for (int i=0; i<input.size(); i++){
		//cout <<"predict" << i << ":" << input[i] << endl;
		values[i]=input[i];
	}
	
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
	
	for (int i=change; i<end; i++){
		output.push_back(values[i]);
	}
	return output;
}
	
void MultiLayerNN::learn(std::vector<std::vector<float> > input, std::vector<std::vector<float> > output, int epochNumber,int crossValidation){
	
	int numLayers=layers.size();
	int numNeurons=layers[numLayers-1];
	
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
	csvResultsErrors.open("errorsResults.csv", ios::out);
	
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
						//wdiference[i][j]=-delta[i]*values[j]*(1-epoch/(epochNumber*2.0f))+wdiferenceOld[i][j]*(epoch/(epochNumber*5.0f));
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
		if (epoch>200){
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
			csvResultsErrors <<errorAcumulation/(input.size()-dataLarge) << ";"<< endl;
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
}
	
float MultiLayerNN::function(float value){
	//cout << "debug-function:" << value << endl;
	float ret=1/(1+exp(-value));
	//cout << "debug-function:" << ret << endl;
	return ret;
}

void MultiLayerNN::debug(){
	
}
