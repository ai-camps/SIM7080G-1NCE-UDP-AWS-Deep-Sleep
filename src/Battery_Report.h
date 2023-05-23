#ifndef Battery_Report_H
#define Battery_Report_H

#include <Arduino.h>
#define XPOWERS_CHIP_AXP2102
#include "XPowersLib.h"

extern XPowersPMU PMU;

float Battery_Report() {
  float batteryPercentage = -1;
  
  if (!PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL)) {
    Serial.println("Failed to initialize power.....");
    return batteryPercentage;
  }

  PMU.enableBattDetection();
  PMU.enableBattVoltageMeasure();
  
  if (PMU.isBatteryConnect()) {
    batteryPercentage = PMU.getBatteryPercent();
  }
  
  return batteryPercentage;
}

#endif