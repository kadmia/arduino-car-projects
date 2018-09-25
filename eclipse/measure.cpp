/*
 * measure.cpp
 *
 *  Created on: 11 Sep 2018
 *      Author: adam
 */

#include "models.h"


void measureTaco(Taco taco, int tacoPin) {
	tacoLeft.newState = digitalRead(TACO_LEFT_PIN);
	if (tacoLeft.newState == tacoLeft.lastState) {
		if (tacoLeft.lastState == 0)
			tacoLeft.lastState = 1;
		else
			tacoLeft.lastState = 0;
		tacoLeft.counter++;
	}
	if (millis() - tacoLeft.lastUpdate > TACOE_UPDATE_TIME) {
		Serial.print("Rotations per second (Left): ");
		Serial.println(tacoLeft.counter);
		tacoLeft.lastCount = tacoLeft.counter;
		tacoLeft.lastUpdate = millis();
		tacoLeft.counter = 0;
	}

	tacoRight.newState = digitalRead(TACO_RIGHT_PIN);
	if (tacoRight.newState == tacoRight.lastState) {
		if (tacoRight.lastState == 0)
			tacoRight.lastState = 1;
		else
			tacoRight.lastState = 0;
		tacoRight.counter++;
	}
	if (millis() - tacoRight.lastUpdate > TACOE_UPDATE_TIME) {
		Serial.print("Rotations per second (Right): ");
		Serial.println(tacoRight.counter);
		tacoRight.lastCount = tacoRight.counter;
		tacoRight.lastUpdate = millis();
		tacoRight.counter = 0;
		Serial.print("Left skew is: ");
		Serial.println(leftSkew);
		Serial.print("Right skew is: ");
		Serial.println(rightSkew);
		checkMotorSpeed = true;
	}
}



