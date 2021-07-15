#include "pifan.h"

PiFan::PiFan(const int &pi, const int &en, QObject *parent) : QObject(parent),pi(pi),en(en)
{
    set_mode(pi, en, PI_OUTPUT);

    gpio_write(pi, en, PI_OFF);
}

void PiFan::toggleFan(const bool &enabled){
    if(enabled)
        gpio_write(pi, en, PI_ON);
    else
        gpio_write(pi, en, PI_OFF);
}
