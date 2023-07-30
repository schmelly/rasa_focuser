#ifndef FOCUSER_CONTROL_H
#define FOCUSER_CONTROL_H

#include <Arduino.h>

#include "state_handling.h"
#include "stepper_control.h"

extern State curState;
extern int32_t targetPosition;

void setupFocuserControl();
void updateFocuserPosition(void *pvParameters);

#endif // FOCUSER_CONTROL_H
