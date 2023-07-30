#include "focuser_control.h"

TaskHandle_t updateFocuserPositionTask;

void setupFocuserControl()
{
    xTaskCreatePinnedToCore(
        updateFocuserPosition,       /* Task function. */
        "updateFocuserPositionTask", /* name of task. */
        10000,                       /* Stack size of task */
        nullptr,                     /* parameter of the task */
        1,                           /* priority of the task */
        &updateFocuserPositionTask,  /* Task handle to keep track of created task */
        0);                          /* pin task to core 0 */
}

void updateFocuserPosition(void *pvParameters)
{
    for (;;)
    {
        if (curState == State::MOVING)
        {
            if(!isStepperMoving() && readStepperPosition() == targetPosition) {
                serialPrintln("updateFocuserPosition(): reached targetPosition");
                submitEvent<EventType::HALT>(0);
            }
            else if(!isStepperMoving() && readStepperPosition() != targetPosition) {
                moveStepper();
            }
        }
        else {
            if(isStepperMoving() && !calibratingRMS) {
                stopStepper();
            }
        }
        
        updateStepperState();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
