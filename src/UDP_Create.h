#ifndef UDP_Create_H
#define UDP_Create_H

#include <TinyGsmClient.h>

extern TinyGsm modem;
extern const char UDP_Server[];
extern const int UDP_Port;

bool UDP_Create()
{
    // Create a new UDP connection and retry 3 times if failed
    int retries = 3;
    while (retries--)
    {
        modem.sendAT("+CAOPEN=0,0,\"UDP\",\"" + String(UDP_Server) + "\"," + String(UDP_Port));
        if (modem.waitResponse("+CAOPEN: 0,0", 3000) == 1)
        {
            if (modem.waitResponse("OK", 3000) == 1)
            {
                Serial.println();
                Serial.println("..............................................");
                Serial.println("UDP connection is created successfully.");
                Serial.println("..............................................");
                return true;
            }
            else
            {
                Serial.println();
                Serial.println("..............................................");
                Serial.println("Failed to create UDP connection, trying again...");
                Serial.println("..............................................");
            }
        }
        else
        {
            Serial.println();
            Serial.println("..............................................");
            Serial.println("Failed to create UDP connection, trying again...");
            Serial.println("..............................................");
        }
    }
    Serial.println();
    Serial.println("..............................................");
    Serial.println("Failed to create UDP connection.");
    Serial.println("..............................................");
    return false;
}

#endif
