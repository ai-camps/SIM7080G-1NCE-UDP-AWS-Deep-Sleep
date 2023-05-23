#ifndef Topic_Publish_H
#define Topic_Publish_H

#include <TinyGsmClient.h>

extern TinyGsm modem;
extern const char UDP_Server[];
extern const int UDP_Port;

void Topic_Publish(String pub_topic, String pub_payload)
{
    Serial.println();
    Serial.println("..............................................");
    Serial.println("Sending payload now ...");
    Serial.println("..............................................");

    // Send the payload now
    modem.sendAT("+CASEND=0, " + String(pub_payload.length()));
    if (modem.waitResponse(">", 3000) == 1)
    {
        modem.stream.write((const uint8_t *)pub_payload.c_str(), pub_payload.length());
        if (modem.waitResponse("OK", 3000) == 1)
        {
            Serial.println();
            Serial.println("..............................................");
            Serial.println("Payload is sent successfully");
            Serial.println("..............................................");
        }
        else
        {
            Serial.println();
            Serial.println("..............................................");
            Serial.println("Failed to send payload");
            Serial.println("..............................................");
        }
    }
    else
    {
        Serial.println();
        Serial.println("..............................................");
        Serial.println("Failed to send payload");
        Serial.println("..............................................");
    }
}
#endif
