#include <RTC.h>

RTC myRTC("mydadswifi", "1tounlimiteddata");

void setup() {
    Serial.begin(57600);
    myRTC.begin();
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