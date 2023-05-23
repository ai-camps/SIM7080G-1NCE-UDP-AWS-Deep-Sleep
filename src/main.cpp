#include <Arduino.h>
#include <TinyGsmClient.h>
#include "utilities.h"
#define XPOWERS_CHIP_AXP2102
#include "XPowersLib.h"
#include "utilities.h"
#include <SPI.h>
#include <ArduinoJson.h>
#include "Adafruit_MPU6050.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Wakeup_Reason.h"
#include "MPU_Config.h"
#include "MPU_Read.h"
#include "Network_Bearer_Check.h"
#include "Network_Bearer_Activate.h"
#include "Network_Registration.h"
#include "Modem_Start.h"
#include "SIM_Check.h"
#include "PMU_Init.h"
#include "NBIOT_Config.h"
#include "SysInfo_Read.h"
#include "UDP_Check.h"
#include "UDP_Create.h"
#include "Payload_Connected.h"
#include "Payload_Vibration.h"
#include "Topic_Publish.h"
#include "Topic_Subscribe.h"
#include "Battery_Report.h"
#include "PSM_Off.h"
#include "Event_ID_Generate.h"

#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(Serial1, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

/***********************************
 *  Define variables
 ***********************************/

// Define the device information variables
String FIRMWARE_VERSION = "v1.0";
String MD5 = ESP.getSketchMD5();
String device_id = String(ESP.getEfuseMac(), HEX);
String deviceType = "Guardrail_Sentry";
String imei;
String imsi;
String iccid;
String cop;
String battery;
String Modem_Info;

int csq;

// Define the variables to store the boot count
RTC_DATA_ATTR int bootCount = 0;

// Define the variables to store the PSM timer
XPowersPMU PMU;

// Define the pin for MPU6050
#define MPU_INT_PIN 18 // Connect the MPU6050 interrupt pin to GPIO18, also RTC_GPIO18
#define MPU_SDA_PIN 17 // Connect the MPU6050 SDA pin to GPIO17, also RTC_GPIO17
#define MPU_SCL_PIN 16 // Connect the MPU6050 SCL pin to GPIO16, also RTC_GPIO16

// Define MPU6050 settings
Adafruit_MPU6050 mpu;

// Define the MPU6050 data structure
MPUData mpudata;

// ! Define APN to "iot.1nce.net"
RTC_DATA_ATTR const char apn[] = "iot.1nce.net";

// ! Define the UDP Server URL and Port
RTC_DATA_ATTR const char UDP_Server[] = "udp.os.1nce.com";
RTC_DATA_ATTR const int UDP_Port = 4445;

// Define payload and topic for publishing
RTC_DATA_ATTR String pub_payload;
RTC_DATA_ATTR String pub_topic;

// Define payload and topic for subscribing
RTC_DATA_ATTR String sub_payload;
RTC_DATA_ATTR String sub_topic;

void setup()
{
    Serial.begin(115200);

    /* // Start while waiting for Serial monitoring
        while (!Serial)
            ;
        delay(3000); */

    Serial.println();
    Serial.println("Start to initialize the device now");

    // Increment boot number and print it every reboot
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    // Print the wakeup reason
    Wakeup_Reason();

    /***********************************
     *! step 1 : Initialize power chip, turn on modem power channel and turn off gps power channel for power saving
     ***********************************/

    Serial.println("............................................................................Step 1");
    Serial.println("Start to initialize power chip, turn on modem and gps antenna power channel ");

    // Initialize the power chip
    PMU_Init();

    Serial.println("Step 1 done !");

    /***********************************
     *! step 2 : Start modem
     ***********************************/

    Serial.println("............................................................................Step 2");
    Serial.println("Start to initialize T-SIM7080G modem now ");

    Modem_Start();

    Serial.println("Step 2 done !");

    /***********************************
     *! step 3 : Check if the SIM card is inserted
     ***********************************/

    Serial.println("............................................................................Step 3");
    Serial.println("Start to check if the SIM card is inserted ");

    SIM_Check();

    Serial.println("Step 3 done !");

    /***********************************
     *! step 4 : Set the network mode to NB-IOT
     ***********************************/

    Serial.println("............................................................................Step 4");
    Serial.println("Start to configure the network mode to NB-IOT ");

    NBIOT_Config();

    Serial.println("Step 4 done !");

    /***********************************
     *! step 5 : Start network registration
     ***********************************/

    Serial.println("............................................................................Step 5");
    Serial.println("Start to perform network registration and configure APN");

    Network_Registration();

    Serial.println("Step 5 done !");

    /***********************************
     *! step 6 : Activate the network bearer
     ***********************************/

    Serial.println("............................................................................Step 6");
    Serial.println("Start to activate the network bearer");

    // Check if the UDP connection is established
    if (!Network_Bearer_Check())
    {
        // Create UDP connection
        Network_Bearer_Activate();
    }

    Serial.println("Step 6 done !");

    /***********************************
     *! step 7 : Show device information and battery percentage
     ***********************************/

    Serial.println("............................................................................Step 7");
    Serial.println("Show the information of the Modem, SIM and Network  ");

    // Show the information of the Modem, SIM and Network
    SysInfo_Read();

    // Show the battery percentage
    float batteryPercentage = Battery_Report();
    Serial.print("Battery Percentage: ");
    Serial.print(batteryPercentage);
    Serial.println("%");

    Serial.println("Step 7 done !");

    /***********************************
     *! step 8 : Initialize and configure MPU6050 and read the initial data
     ***********************************/

    Serial.println("............................................................................Step 8");
    Serial.println("Initializing MPU6050... ");

    // Initialize the I2C bus for MPU 6050
    Wire.begin(MPU_SDA_PIN, MPU_SCL_PIN); // I2C bus on wire for MPU 6050
    if (mpu.begin())
    {
        Serial.println("MPU6050 chip is found");
    }
    else
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    // Configure MPU6050
    Serial.println("Configuring MPU6050... ");
    MPU_Config();

    // Read the MPU6050 sensor data
    Serial.println("Reading the MPU6050 initial data when device is online");
    MPU_Read(mpudata);

    Serial.println("Step 8 done !");

    /***********************************
     *! step 9 : Generate JSON payload and send packet data to udp.os.1nce.com:4445
     ***********************************/

    Serial.println("............................................................................Step 9");
    Serial.println("Generate JSON payload and send initial MPU6050 data to udp.os.1nce.com:4445 ");

    // Define the topic
    String pub_topic = device_id + "/device_connected";

    // Generate the JSON payload
    Payload_Connected(mpudata, pub_topic);
    String pub_payload = Payload_Connected(mpudata, pub_topic);

    // Check if the UDP connection is established
    if (!UDP_Check())
    {
        // Create UDP connection
        UDP_Create();
    }

    // Publish the topic and payload
    Topic_Publish(pub_topic, pub_payload);

    Serial.println("Step 9 done !");

    /***********************************
     *! step 10 : Disable the PSM mode
     ***********************************/

    Serial.println("............................................................................Step 10");
    Serial.println("Disable the PSM mode, disable GPS and GNSS, turn off gps antenna power channel  ");
    // ! Disable the PSM mode
    PSM_Off();
    modem.waitResponse();

    // ! Disable GPS Power channel
    PMU.disableBLDO2();
    modem.waitResponse();

    // ! Disable GPS
    modem.disableGPS();
    modem.waitResponse();

    // ! Disable GNSS
    modem.sendAT("+SGNSCMD=0");
    modem.waitResponse();

    Serial.println("Step 10 done !");
}

void loop()
{
    // Check for motion interrupt status
    if (mpu.getMotionInterruptStatus())
    {
        // Read the MPU6050 sensor data
        MPU_Read(mpudata);

        // Define the pub_topic
        String pub_topic = device_id + "/vibration_detected";

        // Generate the pub_payload
        String pub_payload = Payload_Vibration(mpudata, pub_topic);

        // Check if the UDP connection is established
        if (!UDP_Check())
        {
            UDP_Create();
        }

        // Publish pub_topic and pub_payload to report vibration detected
        Topic_Publish(pub_topic, pub_payload);
    }
} 

/* void loop()
{
    while (Serial1.available())
    {
        Serial.write(Serial1.read());
    }
    while (Serial.available())
    {
        Serial1.write(Serial.read());
    }
}  
 */
