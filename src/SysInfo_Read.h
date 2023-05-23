#ifndef SysInfo_Read_H
#define SysInfo_Read_H
#include <TinyGsmClient.h>

extern TinyGsm modem;
extern String imei;
extern String imsi;
extern String iccid;
extern String cop;
extern String Modem_Info;

extern int csq;

extern const char apn[];

void SysInfo_Read()
{

    Serial.println("T-SIM7080G Firmware Version: ");

    String response;

    // Send the AT command to get the firmware version
    modem.sendAT("+CGMR");
    if (modem.waitResponse(10000L) != 1)
    {
        Serial.println("Get Firmware Version Failed!");
    }
    else
    {
        Serial.println(response);
    }

    iccid = modem.getSimCCID();
    Serial.print("ICCID:");
    Serial.println(iccid);

    imei = modem.getIMEI();
    Serial.print("IMEI:");
    Serial.println(imei);

    imsi = modem.getIMSI();
    Serial.print("IMSI:");
    Serial.println(imsi);

    cop = modem.getOperator();
    Serial.print("Operator:");
    Serial.println(cop);

    csq = modem.getSignalQuality();
    Serial.print("Signal quality:");
    Serial.println(csq);

    Modem_Info = modem.getModemInfo();
    Serial.print("Modem Info:");
    Serial.println(Modem_Info);
}

#endif