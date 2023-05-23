#ifndef Payload_Vibration_H
#define Payload_Vibration_H

#include <ArduinoJson.h>
#include <TinyGsmClient.h>
#include "MPU_Read.h"
#include "Event_ID_Generate.h"
#include "Battery_Report.h"

#define PAYLOAD_SIZE_VIBRATION 250

extern TinyGsm modem;

extern String pub_payload;
extern String pub_topic;

extern String device_id;

// Set default values for the JSON payload and return it as a String
String Payload_Vibration(const MPUData &mpudata, String pub_topic)
{
    // Define the JSON payload structure
    DynamicJsonDocument doc(PAYLOAD_SIZE_VIBRATION);

    // Set the JSON payload values
    doc["event_type"] = "vb";                   // event type for vibration
    doc["event_id"] = generate_event_id();      // event id
    doc["device_id"] = device_id;               // device ID for device
    doc["bt"] = String(Battery_Report()) + "%"; // battery percentage
    doc["ax"] = String(mpudata.ax, 2);          // accelerometer x-axis
    doc["ay"] = String(mpudata.ay, 2);          // accelerometer y-axis
    doc["az"] = String(mpudata.az, 2);          // accelerometer z-axis
    doc["gx"] = String(mpudata.gx, 2);          // gyroscope x-axis
    doc["gy"] = String(mpudata.gy, 2);          // gyroscope y-axis
    doc["gz"] = String(mpudata.gz, 2);          // gyroscope z-axis
    doc["tp"] = String(mpudata.tempF, 2);       // temperature

    // Serialize JSON document to a String
    String payloadString;
    serializeJson(doc, payloadString);

    // Return the payload string
    return payloadString;
}
#endif