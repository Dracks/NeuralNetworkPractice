//
//  Utils.h
//  P2
//
//  Created by Dracks on 21/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef P2_Utils_h
#define P2_Utils_h

#include "Normalize.h"


std::vector<std::vector<float> > loadFile(char* fileName);
std::vector<Normalize*> obtainNormalizations(std::vector<std::vector<float> > data);
std::pair<std::vector<std::vector<float> >, std::vector<std::vector<float> > > splitData(std::vector<std::vector<float> > data, int inputNumber);
void print(std::vector<float> data, std::vector<Normalize*> n, int offset);
void print(std::vector<float> data);

#endif
