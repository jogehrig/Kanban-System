#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);//16 zeichen 2 zeilen
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include "Adafruit_Thermal.h"

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
#include <SoftwareSerial.h>
//SoftwareSerial RFID(2, 3); // PIN2 125 KHZ
#define TX_PIN 6 //   YELLOW WIRE  RX on printer
#define RX_PIN 5 // GREEN WIRE   TX on printer
SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor


int i;
long uidtemp = 0;

typedef struct
 {
     String name;
     long uid;

 }  artikel_type;

static artikel_type artikel[30];



void setup()
{
//RFID.begin(9600);    //125 KHZ
Serial.begin(9600); //DEBUG

mySerial.begin(19200);
printer.begin();
delay(3000);
printer.sleep();

SPI.begin();
mfrc522.PCD_Init();
pinMode (2, OUTPUT); // led
lcd.begin();


artikel[0].name = "sonstiges";
artikel[0].uid = 503220;

artikel[1].name = "brot";
artikel[1].uid = 2032910;

artikel[2].name = "nutella";
artikel[2].uid = 1681810;

artikel[3].name = "nudeln";
artikel[3].uid = 586010;

artikel[4].name = "pesto";
artikel[4].uid = 1027810;

artikel[5].name = "parmesan";
artikel[5].uid = 2127210;

artikel[6].name = "kaese";
artikel[6].uid = 1472110;

artikel[7].name = "butter";
artikel[7].uid = 1336410;

artikel[8].name = "milch";
artikel[8].uid = 1148510;

artikel[9].name = "reis";
artikel[9].uid = 1707710;

artikel[10].name = "sirup";
artikel[10].uid = 2012310;


}

void loop()
{

/*if (RFID.available() > 0) //125 KHZ
  {
     i = RFID.read();
     Serial.print(i, DEC);
     Serial.print(" ");
  }*/
  
if ( ! mfrc522.PICC_IsNewCardPresent())
{
return;
}

if ( ! mfrc522.PICC_ReadCardSerial())
{
return;
}

long code=0;

for (byte i = 0; i < mfrc522.uid.size; i++)
{
code=((code+mfrc522.uid.uidByte[i])*10);
}

//Serial.print("Die Kartennummer lautet:");//DEBUG
//Serial.println(code);

for(i = 0; i < 30; ++i) {
    if (artikel[i].uid == code) {
        
        //Serial.println(artikel[i].name);//DEBUG
        lcd.clear();
        lcd.setCursor(0,0);                 
        lcd.print(artikel[i].name);
        lcd.setCursor(0,1);
        lcd.print("Einkaufen");

      if(artikel[i].uid == uidtemp){
      //Serial.println("break");
      break;
      }
        
        printer.wake();
        //Serial.println(artikel[i].name);//DEBUG
        printer.println(artikel[i].name);
        uidtemp = artikel[i].uid;
        delay(1000);
        printer.sleep();
        
        break;
    }}

}
