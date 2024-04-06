#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

#include <TMCStepper.h>
#include <AccelStepper.h>

#include "debug.h"

#define R_SENSE 0.11
#define DRIVER_ADDRESS 0b00
#define ENABLE_PIN 5
#define STEP_PIN 22
#define DIR_PIN 23
#define STEPPING 4

#define IN HIGH
#define OUT LOW

// focuser specific defines
#define CFZ (2.0*2.0*2.2)
#define THREAD_PITCH 0.75
#define FULL_STEP_ANGLE 0.9
#define STEPS_PER_ANGLE(ALPHA) (ALPHA / FULL_STEP_ANGLE * STEPPING)
#define STEPS_PER_REVOLUTION STEPS_PER_ANGLE(360.0) // / FULL_STEP_ANGLE * STEPPING)
#define T_ROTATION 1000000 // one second for a full rotation (microseconds) or 60 RPM
#define T_STEP_DELAY (T_ROTATION / STEPS_PER_REVOLUTION)
#define STEP_SIZE (THREAD_PITCH * 1000.0 / STEPS_PER_REVOLUTION)
#define MAX_STEP (6.0 * STEPS_PER_REVOLUTION)
// #define MAX_INCREMENT (STEPS_PER_REVOLUTION / 2)
#define MAX_INCREMENT (STEPS_PER_REVOLUTION)

extern const int32_t startPosition;
extern int32_t targetPosition;
extern bool calibratingRMS;

void setupStepperControl();
int32_t readStepperPosition();
bool isStepperMoving();
bool updateStepperState();
void moveStepper();
void stopStepper();
void beginCalibrateRMS();
void calibrateRMS(void *pvParameters);
void overwriteStepperPosition(int32_t position);

#endif //STEPPER_CONTROL_H
