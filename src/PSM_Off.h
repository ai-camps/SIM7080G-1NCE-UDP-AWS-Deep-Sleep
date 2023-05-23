/*
 * Disable the PSM mode, and the modem will stay active
 * */

#ifndef PSM_Off_H
#define PSM_Off_H
#include <TinyGsmClient.h>

extern TinyGsm modem;

void PSM_Off()
{
    // Disable deep sleep wakeup indication
    modem.sendAT("+CPSMSTATUS=0");

    // Set the EPS registration status to "0" to disable PSM
    modem.sendAT("+CEREG=0");
    if (modem.waitResponse() != 1)
    {
        Serial.println("Disabling PSM failed!");
        return;
    }

    // Disable PSM mode
    modem.sendAT("+CPSMS=0");
    if (modem.waitResponse() != 1)
    {
        Serial.println("Disabling PSM mode failed!");
        return;
    }

    Serial.println("PSM has been disabled successfully!");
}

#endif