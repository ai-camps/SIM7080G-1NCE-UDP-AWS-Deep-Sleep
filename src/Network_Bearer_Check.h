#ifndef Network_Bearer_Check_H
#define Network_Bearer_Check_H
#include <TinyGsmClient.h>

extern TinyGsm modem;

bool Network_Bearer_Check()
{
    Serial.println("..............................................");
    Serial.println("Checking if there exists a network bearer...");
    Serial.println("..............................................");

    // Send the AT command to query the network bearer status
    modem.sendAT("+CNACT?");

    // Response string
    String response;

    // Wait for the response with a 10-second timeout
    int8_t ret = modem.waitResponse(10000UL, response);

    // Check if the response contains "+CNACT: 0,1" indicating network bearer is activated
    if (response.indexOf("+CNACT: 0,1") >= 0)
    {
        Serial.println("..............................................");
        Serial.println("The network bearer is already activated...");
        Serial.println("..............................................");
        return true;
    }
    // Check if the response contains "+CNACT: 0,0" indicating network bearer is not activated
    else if (response.indexOf("+CNACT: 0,0") >= 0)
    {
        Serial.println("..............................................");
        Serial.println("The network bearer is not activated yet");
        Serial.println("..............................................");
        return false;
    }
    else
    {
        Serial.println("No valid response");
        return false;
    }
}

#endif
