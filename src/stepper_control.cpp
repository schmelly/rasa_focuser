#include "stepper_control.h"

TMC2209Stepper stepper(&Serial2, R_SENSE, DRIVER_ADDRESS);

void print_DRV_STATUS()
{
    auto otpw = stepper.otpw();
    auto ot = stepper.ot();
    auto s2ga = stepper.s2ga();
    auto s2gb = stepper.s2gb();
    auto s2vsa = stepper.s2vsa();
    auto s2vsb = stepper.s2vsb();
    auto ola = stepper.ola();
    auto olb = stepper.olb();
    auto t120 = stepper.t120();
    auto t143 = stepper.t143();
    auto t150 = stepper.t150();
    auto t157 = stepper.t157();
    auto cs_actual = stepper.cs_actual();
    auto stealth = stepper.stealth();
    auto stst = stepper.stst();
    auto pwm_grad_auto = stepper.pwm_grad_auto();
    auto pwm_ofs_auto = stepper.pwm_ofs_auto();
    auto pwm_scale_auto = stepper.pwm_scale_auto();
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
    stepper.begin();
    stepper.toff(4);
    stepper.blank_time(24);
    stepper.rms_current(750);
    stepper.microsteps(STEPPING);
    // stealthchop configuration
    stepper.en_spreadCycle(false); // enable stealthchop, i.e., fast velocities ('0' <=> enabled)
    stepper.pwm_autoscale(true); // enable automatic current scaling
    stepper.pwm_autograd(true); // enable automatic tuning of PWM_GRAD_AUTO
    stepper.pwm_grad(10); // hard coded from a manual rms calibration run
    stepper.pwm_ofs(78); // hard coded from a manual rms calibration run
    // coolstep configuration
    stepper.TCOOLTHRS(0xFFFFF);
    stepper.semin(5);
    stepper.semax(2);
    stepper.sedn(0b01);
    stepper.SGTHRS(100);

    serialPrintln("done, waiting 200 milliseconds for stealth chop calibration");
    delay(200);

    print_DRV_STATUS();
}

void step(uint32_t steps, bool forward)
{
    if (forward)
    {
        digitalWrite(DIR_PIN, IN);
    }
    else
    {
        digitalWrite(DIR_PIN, OUT);
    }
    delayMicroseconds(1);

    for (uint32_t i = 0; i < steps; i++)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(T_STEP_DELAY - 1);
    }

    digitalWrite(DIR_PIN, LOW);
    delayMicroseconds(1);
}

void calibrateRMS() {

    // test full rotation forward
    serialPrintln("stepping forward");
    step(STEPS_PER_REVOLUTION, true);
    serialPrintln("done, waiting a second");
    delay(1000);

    // test full rotation backward
    serialPrintln("stepping backward");
    step(STEPS_PER_REVOLUTION, false);
    serialPrintln("done");

    print_DRV_STATUS();
}