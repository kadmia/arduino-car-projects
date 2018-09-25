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

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

// The loop function is called in an endless loop
void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                       // wait for a second
}
