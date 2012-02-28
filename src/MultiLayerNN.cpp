
#include "MultiLayerNN.h"
#include "Math.h"
#include <assert.h>
#include <iostream>

using namespace std;

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
		ovalue.push_back(0.5f);
	}//*/
	
	for (int i=1; i<layers.size()-1; i++){
		int ini=layers[i-1];
		int change=layers[i];
		int end=layers[i+1];
		for (int j=ini; j<change; j++){
			for (int z=change; z<end; z++){
				weights[z][j]=0.5f;
			}
		}
	}
}
	
std::vector<float> MultiLayerNN::predict(std::vector<float> input){
	vector<float> output;
	assert(input.size()==layers[1]);
	for (int i=0; i<input.size(); i++){
		cout <<"predict" << i << ":" << input[i] << endl;
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
	
void MultiLayerNN::learn(std::vector<std::vector<float> > input, std::vector<std::vector<float> > output, int crossValidation){
	
	int numLayers=layers.size();
	int numNeurons=layers[numLayers-1];
	
	int ini,change,end;
	int i,j,z;
	
	vector<float> delta(numNeurons,0.0f);
	//vector<float> odiference(numNeurons,0.0f);
	//vector<float> output;
	
	std::map<int, std::map<int, float> > wdiference;
	
	for (i=1; i<numLayers-1; i++){
		ini=layers[i-1];
		change=layers[i];
		end=layers[i+1];
		for (int j=ini; j<change; j++){
			for (int z=change; z<end; z++){
				wdiference[z][j]=0.0f;
			}
		}
	}
	
	int dataLarge=input.size()-crossValidation;
	for (int loop=0; loop<100; loop++){
		for (int loopPattern=0; loopPattern<dataLarge; loopPattern++){
			int rowData=rand()%dataLarge;
			cout << rowData << " & " << dataLarge << endl;
			
			vector<float> result=this->predict(input[rowData]);
			
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
						wdiference[i][j]=-delta[i]*values[j];
					}
				}
			}
			
			map<int, map<int, float> >::iterator it=wdiference.begin();
			while (it!=wdiference.end()){
				map<int, float>::iterator jt=it->second.begin();
				while (jt!=it->second.end()){
					//cout << it->first << "-"<< jt->first<< endl;
					//cout << "weights["<< it->first <<"]["<< jt->first <<"]: " << weights[it->first][jt->first] << "+" << jt->second << endl;
					weights[it->first][jt->first]+=0.2f*jt->second;
					jt++;
				}
				it++;
			}
			
			for (i=0; i<ovalue.size(); i++){
				//cout << "oval" << ovalue[i] << " & " << delta[i] << endl;
				ovalue[i]+=0.2f*delta[i];
			}
		}
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
