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
            uint32_t increment = 1;
            if (abs(targetPosition - position) > 100)
            {
                increment = 100;
            }
            else if (abs(targetPosition - position) > 10)
            {
                increment = 10;
            }

            if (position < targetPosition)
            {
                step(increment, true);
                position += increment;
            }
            else if (position > targetPosition)
            {
                step(increment, false);
                position -= increment;
            }
            else
            {
                serialPrintln("updateFocuserPosition(): reached targetPosition");
                submitEvent<EventType::HALT>(0);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
