#include "Normalize.h"

//class Normalize{
Normalize::Normalize(float min, float max, float margin){
	this->minimum=min*(1.0f-margin);
	this->diference=(max-min)*(1.0f+2.0f*margin);
}

Normalize::~Normalize(){}

float Normalize::normalize(float v){
	return (v-this->minimum)/this->diference;
}

float Normalize::restore(float v){
	return v*this->diference+this->minimum;
}
