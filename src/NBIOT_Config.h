#ifndef NBIOT_Config_H
#define NBIOT_Config_H
#include <TinyGsmClient.h>

extern TinyGsm modem;

void NBIOT_Config()
{
    // Set the network mode to NB-IOT
    modem.sendAT("+CNMP=38"); // 38 for LTE only

    // Set the preferred network mode to NB-IOT
    modem.sendAT("+CMNB=2"); // 2 for NB-IOT

    // Get the preferred network mode
    uint8_t pre = modem.getPreferredMode();

    // Get the network mode
    uint8_t mode = modem.getNetworkMode();

    Serial.printf("getNetworkMode:%u getPreferredMode:%u\n", mode, pre);
}

#endif