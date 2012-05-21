/* 
 * File:   BackPropagation.h
 * Author: eickot
 *
 * Created on 23 / febrer / 2012, 10:14
 */

#ifndef MULTILAYERNN_H
#define	MULTILAYERNN_H

#include <vector>
#include <map>

class MultiLayerNN {
private:
	std::vector<int> layers;
	std::vector< float> values;
	std::vector< float> ovalue;
	std::map<int, std::map<int, float> > weights;//*/
	
public:
	MultiLayerNN(std::vector< int > layers);
	MultiLayerNN(std::string file);
	
	std::vector<float> predict(std::vector<float>);
	
	void learn(std::vector<std::vector<float> > inputs, std::vector<std::vector<float> > inputs,int epoch=100, int crossValidation=0, float learnRate=0.1f, float momentum=0.4f);
	
	float function(float value);//*/
	
	void debug();
	void dumpFile(std::string name);
};

#endif	/* BACKPROPAGATION_H */

