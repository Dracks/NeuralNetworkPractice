#include "Normalize.h"

//class Normalize{
Normalize::Normalize(float min, float max, float margin){
	/*
	 * min=kMin-margin*diference
	 * min+margin*diference=kMin
	 * min+margin*(max+margin*max-min)=kMin
	 * min+margin*max+margin*margin*max-margin*min=kMin
	 * min(1-margin)=kMin-margin*max-margin*margin*max
	 * min=(kMin-margin*max-margin*margin*max)/(1-margin)
	 */
	min=(min-margin*margin*max-margin*max)/(1-margin);
	max=max*(1+margin);
	this->diference=max-min;
	this->minimum=min;
}

Normalize::~Normalize(){}

float Normalize::normalize(float v){
	return (v-this->minimum)/this->diference;
}

float Normalize::restore(float v){
	return v*this->diference+this->minimum;
}
