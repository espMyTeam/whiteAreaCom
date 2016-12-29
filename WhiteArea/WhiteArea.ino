#include "EEPROM.h"
#include "cc1101.h"
#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(6, 7); // (RX, TX) (pin Rx BT, pin Tx BT)

String messageAEnvoyer = "";
String messageRecu = "";
bool finMessageRecu = false;

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

// The connection to the hardware chip CC1101 the RF Chip
CC1101 cc1101;

byte syncWord[2] = {199, 0};
CCPACKET paquet;

// a flag that a wireless packet has been received
boolean packetAvailable = false;


/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Debut");

  initBluetooth();
  
  // initialisation de l'antenne RF
  cc1101.init();
  
  cc1101.setSyncWord(syncWord, false);
  cc1101.setCarrierFreq(CFREQ_433);
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"

  attachInterrupt(0, cc1101signalsInterrupt, FALLING);

  
  Serial.println("Equipement initialise");
}

void envoiPaquet() {
  if(cc1101.sendData(paquet)){
    Serial.println("Envoi termine");
   }else{
    Serial.println("Echec de l'envoi");
  }
}

void formatPaquet(String message){
  if(message.length()<61){
    Serial.print("Taille ");
    Serial.println(message.length());
    paquet.length=message.length();
    message.getBytes(paquet.data, message.length()+1);
    envoiPaquet();
  }
  else{
    String partie1 = message.substring(0, 55);
    String partie2 = message.substring(55);
    partie1+="~@]#`";
    formatPaquet(partie1);
    //delay(500);
    formatPaquet(partie2);
  }
}

void receptionPaquet(){
  CCPACKET paquet;
    
    if(cc1101.receiveData(&paquet) > 0){
      if(!paquet.crc_ok) {
        Serial.println("Erreur CRC");
      }
      
      if(paquet.length > 0){
        if(paquet.length == 60 && paquet.data[55]=='~' && paquet.data[56]=='@' && paquet.data[57]==']' && paquet.data[58]=='#' && paquet.data[59]=='`' ){
          for(int j=0; j<55; j++){
            //Serial.write(paquet.data[j]);
            messageRecu += paquet.data[j];
          }
        }
        else{
          for(int j=0; j<paquet.length; j++){
            //Serial.write(paquet.data[j]);
            messageRecu += paquet.data[j];
          }
          //Serial.println("");
          finMessageRecu = true;
        }
      }
    }
    //return messageRecu;
}

void verificationPaquetRecu(){
  if(packetAvailable){
    //Serial.println("Paquet recu");
    
    // Desactivation de la reception
    detachInterrupt(0);

    packetAvailable = false;
    
    receptionPaquet();

    if (finMessageRecu){
      for(int j = 0; j < messageRecu.length(); j++){
        sendBluetooth(messageRecu.charAt(j));
      }
      messageRecu = "";
      finMessageRecu = false;
    }
    
    // Activation de la reception
    attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  }
}

void verificationPaquetAEnvoyer(){
  messageAEnvoyer = receiveBluetooth();
  if(messageAEnvoyer != "") {
    formatPaquet(messageAEnvoyer);
    
  }
  //formatPaquet("Test");
  
}

void loop(){
  verificationPaquetRecu();
  verificationPaquetAEnvoyer();
}

