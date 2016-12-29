#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(11, 10); // (RX, TX) (pin Rx BT, pin Tx BT)
String donnees;
void setup()
{
    
    Serial.begin(9600);
    initBluetooth();
    donnees="";    
}

void loop() // run over and over
{
    donnees=receiveBluetooth();
    if(donnees != "")
      Serial.println(donnees);
      
    if (Serial.available()) {
        bluetoothSerial.write(Serial.read());
    }
}

void initBluetooth(){
  bluetoothSerial.begin(9600);
  bluetoothSerial.write("AT+NAMERelai_Bluetooth");
}

void sendBluetooth(char donnees){
  bluetoothSerial.write(donnees);
}

String receiveBluetooth(){
  String receiv="";
  while (bluetoothSerial.available()) {
        //Serial.write(bluetoothSerial.read());
        receiv+=String((char)bluetoothSerial.read());  
    }
   return receiv;
}

