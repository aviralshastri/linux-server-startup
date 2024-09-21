#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <RTClib.h>

class RTC {
private:
    RTC_DS3231 rtc;
    WiFiUDP ntpUDP;
    NTPClient timeClient;
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    void initializeRTC(bool wifiConnected);

public:
    RTC();
    void begin(bool wifiConnected);
    DateTime get_time();
    String get_date();
    String get_day();
    void set_time(int hour, int minute, int second);
    void set_date(int year, int month, int day);
};

#endif // RTC_H
