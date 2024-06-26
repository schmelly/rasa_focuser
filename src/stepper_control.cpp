#include "stepper_control.h"

TMC2209Stepper driver(&Serial2, R_SENSE, DRIVER_ADDRESS);
AccelStepper stepper = AccelStepper(stepper.DRIVER, STEP_PIN, DIR_PIN);
TaskHandle_t calibrateRMSTask;

void print_DRV_STATUS()
{
    auto otpw = driver.otpw();
    auto ot = driver.ot();
    auto s2ga = driver.s2ga();
    auto s2gb = driver.s2gb();
    auto s2vsa = driver.s2vsa();
    auto s2vsb = driver.s2vsb();
    auto ola = driver.ola();
    auto olb = driver.olb();
    auto t120 = driver.t120();
    auto t143 = driver.t143();
    auto t150 = driver.t150();
    auto t157 = driver.t157();
    auto cs_actual = driver.cs_actual();
    auto stealth = driver.stealth();
    auto stst = driver.stst();
    auto pwm_grad_auto = driver.pwm_grad_auto();
    auto pwm_ofs_auto = driver.pwm_ofs_auto();
    auto pwm_scale_auto = driver.pwm_scale_auto();
    serialPrintln("DRV_STATUS");
    serialPrintln("otpw %d", otpw);
    serialPrintln("ot %d", ot);
    serialPrintln("s2ga %d", s2ga);
    serialPrintln("s2gb %d", s2gb);
    serialPrintln("s2vsa %d", s2vsa);
    serialPrintln("s2vsb %d", s2vsb);
    serialPrintln("ola %d", ola);
    serialPrintln("olb %d", olb);
    serialPrintln("t120 %d", t120);
    serialPrintln("t143 %d", t143);
    serialPrintln("t150 %d", t150);
    serialPrintln("t157 %d", t157);
    serialPrintln("cs_actual %d", cs_actual);
    serialPrintln("stealth %d", stealth);
    serialPrintln("stst %d", stst);
    serialPrintln("pwm_grad_auto %d", pwm_grad_auto);
    serialPrintln("pwm_ofs_auto %d", pwm_ofs_auto);
    serialPrintln("pwm_scale_auto %d", pwm_scale_auto);
}

void enableStepper()
{
    // enable stepper
    digitalWrite(ENABLE_PIN, LOW);
    delayMicroseconds(1);
}

void disableStepper()
{
    // disable stepper
    digitalWrite(ENABLE_PIN, HIGH);
    delayMicroseconds(1);
}

void setupStepperControl()
{
    serialPrintln("setupStepperControl(), configuring pins");

    // configure pins
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    serialPrintln("setupStepperControl(), enable stepper driver");
    enableStepper();

    serialPrintln("setupStepperControl(), configure stepper");
    driver.begin();
    driver.toff(4);
    driver.blank_time(24);
    driver.rms_current(1000);
    driver.microsteps(STEPPING);
    // stealthchop configuration
    driver.en_spreadCycle(true); // enable stealthchop, i.e., fast velocities ('0' <=> enabled)
    driver.pwm_autoscale(true);  // enable automatic current scaling
    driver.pwm_autograd(true);   // enable automatic tuning of PWM_GRAD_AUTO
    driver.pwm_grad(14);         // hard coded from a manual rms calibration run | was 11
    driver.pwm_ofs(149);         // hard coded from a manual rms calibration run | was 144
    // coolstep configuration
    driver.TCOOLTHRS(0xFFFFF);
    driver.semin(0); // was 5
    // driver.semax(2);           // was active
    // driver.sedn(0b01);         // was active
    driver.SGTHRS(0); // was 100

    stepper.disableOutputs();
    stepper.setMaxSpeed(STEPS_PER_REVOLUTION);
    stepper.setAcceleration(6400); // max speed after 0.5 secs at 1600 steps / revolution
    stepper.setEnablePin(ENABLE_PIN);
    stepper.setPinsInverted(false, false, true);
    stepper.setCurrentPosition(startPosition);

    serialPrintln("done, waiting 200 milliseconds for stealth chop calibration");
    delay(200);

    print_DRV_STATUS();

    xTaskCreatePinnedToCore(
        calibrateRMS,           /* Task function. */
        "calibrateRMSTaskTask", /* name of task. */
        10000,                  /* Stack size of task */
        nullptr,                /* parameter of the task */
        1,                      /* priority of the task */
        &calibrateRMSTask,      /* Task handle to keep track of created task */
        1);                     /* pin task to core 1 */
}

int32_t readStepperPosition()
{
    return stepper.currentPosition();
}

bool isStepperMoving()
{
    return stepper.isRunning();
}

bool updateStepperState()
{
    auto running = stepper.run();
    if (!running)
    {
        stepper.disableOutputs();
    }
    return running;
}

void moveStepper()
{
    stepper.enableOutputs();
    stepper.moveTo(targetPosition);
}

void stopStepper()
{
    stepper.stop();
}

void beginCalibrateRMS()
{
    calibratingRMS = true;
}

void calibrateRMS(void *pvParameters)
{
    for (;;)
    {
        if (calibratingRMS)
        {
            stepper.enableOutputs();

            // test full rotation forward
            serialPrintln("stepping forward");
            stepper.moveTo(stepper.currentPosition() + STEPS_PER_REVOLUTION);
            vTaskDelay(pdMS_TO_TICKS(100));
            print_DRV_STATUS();
            vTaskDelay(pdMS_TO_TICKS(3900));
            serialPrintln("done");

            // test full rotation backward
            serialPrintln("stepping backward");
            stepper.moveTo(stepper.currentPosition() - STEPS_PER_REVOLUTION);
            vTaskDelay(pdMS_TO_TICKS(100));
            print_DRV_STATUS();
            vTaskDelay(pdMS_TO_TICKS(3900));
            serialPrintln("done");

            stepper.disableOutputs();
            calibratingRMS = false;
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
}

void overwriteStepperPosition(int32_t position)
{
    stepper.setCurrentPosition(position);
}
