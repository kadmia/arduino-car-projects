#include "Arduino.h"
#include "models.h"

#define TACO_RIGHT_PIN 8
#define TACO_LEFT_PIN 9
#define TACOE_UPDATE_TIME 1000

#define MOTOR_LEFT_PWM_PIN 6
#define MOTOR_LEFT_DIR_PIN 7
#define MOTOR_RIGHT_PWM_PIN 10
#define MOTOR_RIGHT_DIR_PIN 12

//Speed
#define SPEED_SLOW 100  // arbitrary slow speed PWM duty cycle
#define SPEED_MID 150 // arbitrary medium speed PWM duty cycle
#define SPEED_FAST 200 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

#define MOVE_FORWARD HIGH
#define MOVE_REVERSE LOW

//Distance measurements
#define DISTANCE_TRIGGER_MIDDLE_PIN 5
#define DISTANCE_ECHO_MIDDLE_PIN 4
#define DISTANCE_APPROXIMATION .034 / 2
//25 milliseconds is 51 cm
#define DISTANCE_MAX_MEASURE 25 * 1000
#define DISTANCE_UPDATE_TIME 50

unsigned long _time;
Taco tacoLeft;
Taco tacoRight;
int motorSpeedSpread;
Distance distanceMiddle;
boolean checkMotorSpeed = false;
int leftSkew = 0;
int rightSkew = 0;

void setup() {
	Serial.begin(9600);
	pinMode( MOTOR_LEFT_DIR_PIN, OUTPUT);
	pinMode( MOTOR_LEFT_PWM_PIN, OUTPUT);
	digitalWrite( MOTOR_LEFT_DIR_PIN, LOW);
	digitalWrite( MOTOR_LEFT_PWM_PIN, LOW);

	pinMode( MOTOR_RIGHT_DIR_PIN, OUTPUT);
	pinMode( MOTOR_RIGHT_PWM_PIN, OUTPUT);
	digitalWrite( MOTOR_RIGHT_DIR_PIN, LOW);
	digitalWrite( MOTOR_RIGHT_PWM_PIN, LOW);

	pinMode(TACO_LEFT_PIN, INPUT);
	pinMode(TACO_RIGHT_PIN, INPUT);

	pinMode(DISTANCE_TRIGGER_MIDDLE_PIN, OUTPUT);
	pinMode(DISTANCE_ECHO_MIDDLE_PIN, INPUT);
	setSpeed(SPEED_SLOW, SPEED_SLOW, MOVE_FORWARD);
}

void loop() {
	_time = millis();
	measureTaco();
	measureDistanceToObject();
	adjustMotors();
}

void measureTaco() {
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

void adjustMotors() {
	if (checkMotorSpeed) {
		if (tacoLeft.lastCount > tacoRight.lastCount) {
			if (leftSkew > 0) {
				leftSkew--;
			} else {
				rightSkew++;
			}
		}
		else if (tacoLeft.lastCount < tacoRight.lastCount) {
			if (leftSkew >= 0) {
				leftSkew++;
			} else {
				rightSkew--;
			}
		}
		checkMotorSpeed = false;
		incrementSpeed(SPEED_SLOW + leftSkew, SPEED_SLOW + rightSkew, MOVE_FORWARD);
	}

}

void setSpeed(int leftSpeed, int rightSpeed, int direction) {
	stopMotors();
	incrementSpeed(leftSpeed, rightSpeed, direction);
}

void incrementSpeed(int leftSpeed, int rightSpeed, int direction) {
	if (direction == HIGH) {
		//If reverse
		leftSpeed = invertOurValue(leftSpeed);
		rightSpeed = invertOurValue(rightSpeed);
	}
	digitalWrite( MOTOR_LEFT_DIR_PIN, direction);
	digitalWrite( MOTOR_RIGHT_DIR_PIN, direction);

	analogWrite( MOTOR_LEFT_PWM_PIN, leftSpeed);
	analogWrite( MOTOR_RIGHT_PWM_PIN, rightSpeed);
}

int invertOurValue(int input) {
	return 255 - input;
}

void stopMotors() {
	// always stop motors briefly before abrupt changes
	digitalWrite( MOTOR_LEFT_DIR_PIN, LOW);
	digitalWrite( MOTOR_LEFT_PWM_PIN, LOW);
	digitalWrite( MOTOR_RIGHT_DIR_PIN, LOW);
	digitalWrite( MOTOR_RIGHT_PWM_PIN, LOW);
	delay( DIR_DELAY);
}

void measureDistanceToObject() {
	if (millis() - distanceMiddle.lastUpdate > DISTANCE_UPDATE_TIME) {
		digitalWrite(DISTANCE_TRIGGER_MIDDLE_PIN, LOW);
		delayMicroseconds(2);

		// Sets the trigPin on HIGH state for 10 micro seconds
		digitalWrite(DISTANCE_TRIGGER_MIDDLE_PIN, HIGH);
		delayMicroseconds(10);
		digitalWrite(DISTANCE_TRIGGER_MIDDLE_PIN, LOW);

		// Reads the echoPin, returns the sound wave travel time in microseconds
		//This limits reading up to .51m. We don't care about values higher than that.
		distanceMiddle.duration = pulseIn(DISTANCE_ECHO_MIDDLE_PIN, HIGH,
		DISTANCE_MAX_MEASURE);

		// Calculating the distance
		distanceMiddle.distance = distanceMiddle.duration
				* DISTANCE_APPROXIMATION;

		//Serial.print("Distance: ");
		//Serial.println(distanceMiddle.distance);
		distanceMiddle.lastUpdate = millis();
	}
	// Prints the distance on the Serial Monitor
}
