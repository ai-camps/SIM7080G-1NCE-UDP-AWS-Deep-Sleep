#ifndef Network_Bearer_Activate_H
#define Network_Bearer_Activate_H
#include <TinyGsmClient.h>

extern TinyGsm modem;

void Network_Bearer_Activate()
{
    // Activating network bearer
    Serial.println("..............................................");
    Serial.println("Activating the network bearer now ...");
    Serial.println("..............................................");

    // Send the AT command to activate the network bearer
    modem.sendAT("+CNACT=0,1");

    // Response string
    String response = "";

    // Wait for the response with a 10-second timeout
    int8_t ret = modem.waitResponse(10000UL, response);

    // Check if the response contains "ERROR" indicating network bearer activation is failed
    if (response.indexOf("ERROR") >= 0)
    {
        Serial.println("..............................................");
        Serial.println("The network bearer activation is failed");
        Serial.println("..............................................");
    }
    // Check if the response contains "OK" indicating network bearer activation is in progress
    else if (response.indexOf("OK") >= 0)
    {
        Serial.println("..............................................");
        Serial.println("In progress, waiting for network response...");
        Serial.println("..............................................");
        // Wait for the "+APP PDP: 0,ACTIVE" response
        bool activationConfirmed = false;

        // Start time
        unsigned long startTime = millis();

        // Wait for 60 seconds
        while (millis() - startTime < 60000UL) // Wait for 60 seconds
        {
            // Check if there is any data available
            if (modem.stream.available())
            {
                // Read the response
                response = modem.stream.readString();

                // Check if the response contains "+APP PDP: 0,ACTIVE" indicating network bearer activation is successful
                if (response.indexOf("+APP PDP: 0,ACTIVE") >= 0)
                {
                    // Network bearer activation is successful
                    activationConfirmed = true;
                    break;
                }
            }
            delay(100);
        }

        // Check if the network bearer activation is successful
        if (activationConfirmed)
        {
            Serial.println("..............................................");
            Serial.println("The network bearer is activated successfully !");
            Serial.println("..............................................");
        }
        else
        {
            Serial.println("..............................................");
            Serial.println("No network response within the timeout");
            Serial.println("..............................................");
        }
    }
    else
    {
        Serial.println("No valid response");
    }
}

#endif
