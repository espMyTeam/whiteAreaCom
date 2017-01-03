#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 RTC;


void setup () {
    Serial.begin(9600);
    initRTC();
    
}
void loop () {
    Serial.println(getDate());    
    delay(1000);
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
  date+=String(now.year());
  date+=" ";
  date+=String(now.hour());
  date+=":";
  date+=String(now.minute());
  date+=":";
  date+=String(now.second());

  return date;

}

