#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
RTC_DS1307 RTC;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("heure du systeme");
  initRTC();
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(getDate());
  Serial.println(getDate());
}

void initRTC(){
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

String getDate(){
  DateTime now = RTC.now(); 
  String date="";
  date+=String(now.day());
  date+="/";
  date+=String(now.month());
  date+="/";
  date+=String(now.year()).substring(2);
  date+=" ";
  date+=String(now.hour());
  date+=":";
  date+=String(now.minute());
  date+=":";
  date+=String(now.second());

  return date;

}
