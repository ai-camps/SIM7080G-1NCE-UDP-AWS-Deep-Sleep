#ifndef SIM_Check_H
#define SIM_Check_H

#include <TinyGsmClient.h>

extern TinyGsm modem;

bool SIM_Check()
{
    if (modem.getSimStatus() != SIM_READY)
    {
        Serial.println("..............................................");
        Serial.println("SIM Card is NOT inserted !!!");
        Serial.println("..............................................");
        return false;
    }
    else
    {
        Serial.println("..............................................");
        Serial.println("SIM Card is inserted !");
        Serial.println("..............................................");
        return true;
    }
}

#endif
