#include "RTC.h"

RTC::RTC() 
    : timeClient(ntpUDP, "pool.ntp.org", 0, 60000) {}

void RTC::begin(bool wifiConnected) {
    initializeRTC(wifiConnected);
}

void RTC::initializeRTC(bool wifiConnected) {
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10); 
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, trying to set time!");
        if (wifiConnected) {
            Serial.println("Connected to NTP server, fetching time...");
            timeClient.begin();
            timeClient.update();
            unsigned long epochTime = timeClient.getEpochTime();
            DateTime ntpTime = DateTime(epochTime);
            rtc.adjust(ntpTime);
            Serial.println("RTC adjusted to NTP time.");
        } else {
            Serial.println("No internet connection. Keeping current RTC time.");
        }
    } else {
        Serial.println("RTC is running correctly.");
    }
}

DateTime RTC::get_time() {
    return rtc.now();
}

String RTC::get_date() {
    DateTime now = rtc.now();
    char dateStr[11];
    sprintf(dateStr, "%04d/%02d/%02d", now.year(), now.month(), now.day());
    return String(dateStr);
}

String RTC::get_day() {
    DateTime now = rtc.now();
    return String(daysOfTheWeek[now.dayOfTheWeek()]);
}

void RTC::set_time(int hour, int minute, int second) {
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, second));
}

void RTC::set_date(int year, int month, int day) {
    DateTime now = rtc.now();
    rtc.adjust(DateTime(year, month, day, now.hour(), now.minute(), now.second()));
}
