#include <BluetoothSerial.h>
#include <MFRC522.h> 
#include <SPI.h> 


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


#define SS_PIN    21
#define RST_PIN   22
#define led       13

// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 


BluetoothSerial SerialBT;

void setup() 
{
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  SerialBT.begin("ESP32_Bluetooth_device"); //Bluetooth device name
  pinMode(led,OUTPUT);
  
  // Init MFRC522
  mfrc522.PCD_Init(); 
 
}

String kondisi = "LED OFF";

void loop() 
{ 
  if ( mfrc522.PICC_IsNewCardPresent()) 
  {
    if ( mfrc522.PICC_ReadCardSerial()) 
    {
      String UID=""; 

      for(byte i = 0; i < mfrc522.uid.size; i++) {
        String satu =  mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
        String dua = String ( mfrc522.uid.uidByte[i], HEX);
        String tampung = satu+dua;
        UID = UID+tampung;
      }

      Serial.print("ID:");
      Serial.println(UID);
      
      SerialBT.print("ID:");
      SerialBT.println(UID);
      mfrc522.PICC_HaltA();

      if(UID == " ec e0 de 2b"){
        if(kondisi == "LED OFF"){
          digitalWrite(led,HIGH);
          kondisi = "LED ON";
          Serial.println(kondisi);
        }else{
          digitalWrite(led,LOW);
          kondisi = "LED OFF";
          Serial.println(kondisi);
        }
      }  
       
    }
    
  }
  
}
