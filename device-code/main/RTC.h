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
    const char* ssid;
    const char* password;
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    void connectToWiFi();
    void initializeRTC();

public:
    RTC(const char* ssid, const char* password);
    void begin();
    DateTime get_time();
    String get_date();
    String get_day();
    void set_time(int hour, int minute, int second);
    void set_date(int year, int month, int day);
};

#endif // RTC_H