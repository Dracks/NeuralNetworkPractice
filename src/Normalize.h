/* 
 * File:   Normalize.h
 * Author: eickot
 *
 * Created on 23 / febrer / 2012, 10:19
 */

#ifndef NORMALIZE_H
#define	NORMALIZE_H


class Normalize{
private:
	float minimum, diference;
public:
	Normalize(float min, float max, float margin=0.1f);
	virtual ~Normalize();
	
	float normalize(float v);
	float restore(float v);
};

#endif	/* NORMALIZE_H */

