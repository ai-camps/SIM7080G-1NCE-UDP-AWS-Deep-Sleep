#ifndef Wakeup_Reason_H
#define Wakeup_Reason_H

// Define ESP32 wakeup function for power on, timer, vibration sensor and ULP
void Wakeup_Reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Device is wakeup caused by external signal from vibration sensor");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Device is wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Device is wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Device is wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Device reboot caused by power on: %d\n", wakeup_reason);
        break;
    }
}

#endif