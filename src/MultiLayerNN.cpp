
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
		ovalue.push_back(0.0f);
	}
	
	for (int i=1; i<layers.size()-1; i++){
		int ini=layers[i-1];
		int change=layers[i];
		int end=layers[i+1];
		for (int j=ini; j<change; j++){
			for (int z=change; z<end; z++){
				weights[z][j]=0.4f;
			}
		}
	}
}
	
std::vector<float> MultiLayerNN::predict(std::vector<float> input){
	vector<float> output;
	assert(input.size()==layers[1]);
	for (int i=0; i<input.size(); i++){
		cout << i << ":" << input[i] << endl;
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
				cout << i << "," << j << ":" << values[j] << endl;
				acumulator+=weights[i][j]*values[j]-ovalue[i];
			}
			values[i]=this->function(acumulator);
		}
	}
	
	for (int i=change; i<end; i++){
		output.push_back(values[i]);
	}
	return output;
}
	
void MultiLayerNN::learn(std::vector<std::vector<float> > inputs, std::vector<std::vector<float> > outputs, int crossValidation){
	
	//vector<float> 
	
}
	
float MultiLayerNN::function(float value){
	cout << "debug-function:" << value << endl;
	float ret=1/(1+exp(-value));
	cout << "debug-function:" << ret << endl;
	return ret;
}

void MultiLayerNN::debug(){
	
}
