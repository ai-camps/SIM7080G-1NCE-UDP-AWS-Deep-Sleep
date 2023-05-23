#ifndef SubscribeTopic_H
#define SubscribeTopic_H
#include <TinyGsmClient.h>

extern TinyGsm modem;

// Define the function to subscribe to MQTT message topic from AWS IoT Core
void SubscribeTopic(String topic) 
{
    // Construct the AT command to subscribe to the MQTT message
    char buffer[1024];
    snprintf(buffer, 1024, "+SMSUB=\"%s\",1", topic.c_str());

    // Send the AT command to subscribe to the MQTT message
    modem.sendAT(buffer);

    // Wait for the prompt character '>'
    if (modem.waitResponse(">") == 1)
    {
        // Read the MQTT message containing the payload
        String sub_payload = modem.stream.readStringUntil('\r');
        Serial.print("Subscribed message from AWS IoT Core: ");
        Serial.println(sub_payload);
    }
    else
    {
        Serial.println("Failed to subscribe message");
    }
}


#endif