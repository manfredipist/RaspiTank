#include "pisteppermotor.h"

PiStepperMotor::PiStepperMotor(const int &pi, const int &en, const int &in_1, const int &in_2, QObject *parent) : QObject(parent), pi(pi), en(en), in_1(in_1), in_2(in_2)
{
    set_mode(pi, en, PI_OUTPUT);
    set_mode(pi, in_1, PI_OUTPUT);
    set_mode(pi, in_2, PI_OUTPUT);

    gpio_write(pi, en, PI_ON);
    gpio_write(pi, in_1, PI_OFF);
    gpio_write(pi, in_2, PI_OFF);   
}

PiStepperMotor::~PiStepperMotor()
{
    gpio_write(pi, en, PI_OFF);
    gpio_write(pi, in_1, PI_OFF);
    gpio_write(pi, in_2, PI_OFF);
}

void PiStepperMotor::forward(){
    gpio_write(pi, en, PI_ON);
    gpio_write(pi, in_1, PI_ON);
    gpio_write(pi, in_2, PI_OFF);
}

void PiStepperMotor::backward(){
    gpio_write(pi, en, PI_ON);
    gpio_write(pi, in_1, PI_OFF);
    gpio_write(pi, in_2, PI_ON);
}

void PiStepperMotor::stop(){
    gpio_write(pi, en, PI_OFF);
    gpio_write(pi, in_1, PI_OFF);
    gpio_write(pi, in_2, PI_OFF);
}
