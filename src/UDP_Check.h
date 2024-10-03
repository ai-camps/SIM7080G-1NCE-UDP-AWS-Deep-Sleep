#ifndef UDP_Check_H
#define UDP_Check_H

#include <TinyGsmClient.h>

extern TinyGsm modem;

bool UDP_Check()
{
    Serial.println("..............................................");
    Serial.println("Checking if there exists a UDP connection...");
    Serial.println("..............................................");

    // Check if there is an existing UDP connection
    modem.sendAT("+CASTATE?");
    if (modem.waitResponse("+CASTATE: 0,1", 3000) == 1)
    {
        Serial.println();
        Serial.println("..............................................");
        Serial.println("Existing UDP connection is found...");
        Serial.println("..............................................");
        return true;
    }
    else
    {
        Serial.println();
        Serial.println("..............................................");
        Serial.println("No existing UDP connection is found, creating a new one now...");
        Serial.println("..............................................");
        return false;
    }
}

#endif
