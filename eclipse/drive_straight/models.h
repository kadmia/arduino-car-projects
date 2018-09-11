/*
 * models.h
 *
 *  Created on: 8 May 2018
 *      Author: adam
 */

#ifndef MODELS_H_
#define MODELS_H_



struct Taco {
	int lastState;
	int newState;
	int counter;
	int lastCount;
	unsigned long lastUpdate;
};

struct Distance
{
	int duration;
	int distance;
	unsigned long lastUpdate;
};




#endif /* MODELS_H_ */
