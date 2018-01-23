
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);   //Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall einer mit 16 Zeichen in 2 Zeilen.
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

#include <SoftwareSerial.h>
SoftwareSerial RFID(2, 3); // PIN2 125 KHZ

int i;

void setup()
{

 RFID.begin(9600);    // start serial to RFID reader 125 KHZ
  
Serial.begin(9600);
SPI.begin();
mfrc522.PCD_Init();
pinMode (2, OUTPUT); // led
lcd.begin(); //Im Setup wird der LCD gestartet (anders als beim einfachen LCD Modul ohne 16,2 in den Klammern denn das wurde vorher festgelegt
}

void loop()
{

if (RFID.available() > 0) //125 KHZ
  {
     i = RFID.read();
     Serial.print(i, DEC);
     Serial.print(" ");
  }
  
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

Serial.print("Die Kartennummer lautet:");

Serial.println(code);



if (code==2346930)

{ 
lcd.setCursor(0,0); //Ab hier kann das I2C LCD Modul genau wie das einfache LCD Modul programmiert werden.
lcd.print("Brot");
lcd.setCursor(0,1);  // lcd.setCursor um Zeichen und Zeile anzugeben
lcd.print("Einkaufen"); // lcd.print um etwas auf dem Display anzeigen zu lassen.

digitalWrite (2, HIGH); 

delay (5000); 

digitalWrite (2, LOW); 

} 

}
