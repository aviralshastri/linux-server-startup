#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "RTClib.h"

class RTC {
private:
    RTC_DS3231 rtc;
    WiFiUDP ntpUDP;
    NTPClient timeClient;
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

public:
    RTC() : timeClient(ntpUDP, "pool.ntp.org", 0, 60000) {}

    void rtc_startup(const char* ssid, const char* password) {
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.begin(ssid, password);

        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi connected.");
        } else {
            Serial.println("\nWiFi connection failed. Proceeding without NTP.");
        }

        if (!rtc.begin()) {
            Serial.println("Couldn't find RTC");
            Serial.flush();
            while (1) delay(10); 
        }

        if (rtc.lostPower()) {
            Serial.println("RTC lost power, trying to set time!");
            if (WiFi.status() == WL_CONNECTED) {
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

    DateTime get_time() {
        return rtc.now();
    }

    String get_date() {
        DateTime now = rtc.now();
        char dateStr[11];
        sprintf(dateStr, "%04d/%02d/%02d", now.year(), now.month(), now.day());
        return String(dateStr);
    }

    String get_day() {
        DateTime now = rtc.now();
        return String(daysOfTheWeek[now.dayOfTheWeek()]);
    }

    void set_time(int hour, int minute, int second) {
        DateTime now = rtc.now();
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, second));
    }

    void set_date(int year, int month, int day) {
        DateTime now = rtc.now();
        rtc.adjust(DateTime(year, month, day, now.hour(), now.minute(), now.second()));
    }
};

RTC myRTC;

void setup() {
    Serial.begin(57600);
    myRTC.rtc_startup("mydadswifi", "1tounlimiteddata");
}

void loop() {
    DateTime now = myRTC.get_time();
    Serial.print("Current Time: ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());

    Serial.print("Current Date: ");
    Serial.println(myRTC.get_date());

    Serial.print("Current Day: ");
    Serial.println(myRTC.get_day());

    delay(10000);
}