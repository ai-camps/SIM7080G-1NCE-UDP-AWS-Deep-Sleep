#ifndef Modem_Start_H
#define Modem_Start_H
#include <TinyGsmClient.h>

extern TinyGsm modem;

void Modem_Start() 
{
    Serial1.begin(115200, SERIAL_8N1, BOARD_MODEM_RXD_PIN, BOARD_MODEM_TXD_PIN);

    pinMode(BOARD_MODEM_PWR_PIN, OUTPUT);
    pinMode(BOARD_MODEM_DTR_PIN, OUTPUT);
    pinMode(BOARD_MODEM_RI_PIN, INPUT);

    int retry = 0;
    while (!modem.testAT(1000))
    {
        Serial.print(".");
        if (retry++ > 6)
        {
            // Pull down PWRKEY for more than 1 second according to manual requirements
            digitalWrite(BOARD_MODEM_PWR_PIN, LOW);
            delay(100);
            digitalWrite(BOARD_MODEM_PWR_PIN, HIGH);
            delay(1000);
            digitalWrite(BOARD_MODEM_PWR_PIN, LOW);
            retry = 0;
            Serial.println("Retry start modem .");
        }
    }
    Serial.println();
    Serial.println("..............................................");
    Serial.println("T-SIM7080G modem is well started ...");
    Serial.println("..............................................");

}
#endif