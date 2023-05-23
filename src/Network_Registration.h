#ifndef Network_Registration_H
#define Network_Registration_H

#include <TinyGsmClient.h>
#define XPOWERS_CHIP_AXP2102
#include "XPowersLib.h"

extern TinyGsm modem;
extern String iccid;
bool level = false;
extern XPowersPMU PMU;

extern const char apn[];

const char *register_info[] = {
    "0 - Not registered, MT is not currently searching an operator to register to.The GPRS service is disabled, the UE is allowed to attach for GPRS if requested by the user.",
    "1 - Registered, home network.",
    "2 - Not registered, but MT is currently trying to attach or searching an operator to register to. The GPRS service is enabled, but an allowable PLMN is currently not available. The UE will start a GPRS attach as soon as an allowable PLMN is available.",
    "3 - Registration denied, The GPRS service is disabled, the UE is not allowed to attach for GPRS if it is requested by the user.",
    "4 - Unknown.",
    "5 - Registered, roaming."};

bool Network_Registration()
{
    Serial.println("..............................................");
    Serial.print("Set the APN to: ");
    Serial.println(apn);
    Serial.println("..............................................");

    // Configure APN
    Serial.println("Configuring APN...");
    String setapn = "+CGDCONT=1,\"IP\",\"" + String(apn) + "\"";
    modem.sendAT(setapn);
    modem.waitResponse();

    // Set operator to AT&T
    modem.sendAT("+COPS=0,0,\"AT&T\",9");
    modem.waitResponse();

    // Set LTE band to 12
    modem.sendAT("+CBANDCFG=\"NB-IoT\",12");
    modem.waitResponse();

    // Configure Band Scan Optimization for NB-IOT
    modem.sendAT("+CNBS=0");

    // Wait for network registration
    RegStatus s;
    do
    {
        s = modem.getRegistrationStatus();
        if (s != REG_OK_HOME && s != REG_OK_ROAMING)
        {
            Serial.print(".");
            delay(1000);
        }
    } while (s != REG_OK_HOME && s != REG_OK_ROAMING);

    Serial.println();
    Serial.println("..............................................");
    Serial.print("Network register info: ");
    Serial.println(register_info[s]);
    Serial.println("..............................................");

    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
}

#endif