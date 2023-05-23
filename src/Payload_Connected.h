#ifndef Payload_Connected_H
#define Payload_Connected_H

#include <ArduinoJson.h>
#include <TinyGsmClient.h>
#include "MPU_Read.h"
#include "Battery_Report.h"
#include "Event_ID_Generate.h"

#define PAYLOAD_SIZE_CONNECTED 400

extern TinyGsm modem;

extern String pub_payload;
extern String pub_topic;

extern String device_id;
extern String imei;
extern String cop;

extern int csq;

extern const char apn[];

// Set default values for the JSON payload and return it as a String
String Payload_Connected(const MPUData &mpudata, String pub_topic)
{
    // Define the JSON payload structure
    DynamicJsonDocument doc(PAYLOAD_SIZE_CONNECTED);

    // Set the JSON payload values
    doc["event_type"] = "ol";                   // event type for device online
    doc["event_id"] = generate_event_id();      // event id
    doc["device_id"] = device_id;               // device ID for device
    doc["imei"] = imei;                         // imei number
    doc["nw"] = cop;                            // operator
    doc["apn"] = apn;                           // APN
    doc["ss"] = csq;                            // signal strength
    doc["bt"] = String(Battery_Report()) + "%"; // battery percentage                                                                 // modem information

    doc["ax"] = String(mpudata.ax, 2);    // accelerometer x-axis
    doc["ay"] = String(mpudata.ay, 2);    // accelerometer y-axis
    doc["az"] = String(mpudata.az, 2);    // accelerometer z-axis
    doc["gx"] = String(mpudata.gx, 2);    // gyroscope x-axis
    doc["gy"] = String(mpudata.gy, 2);    // gyroscope y-axis
    doc["gz"] = String(mpudata.gz, 2);    // gyroscope z-axis
    doc["tp"] = String(mpudata.tempF, 2); // temperature

    // Serialize JSON document to a String
    String payloadString;
    serializeJson(doc, payloadString);

    // Return the payload string
    return payloadString;
}
#endif