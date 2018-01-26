#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include "Adafruit_Thermal.h"

#include <SoftwareSerial.h>

#define TX_PIN 6    // YELLOW WIRE  RX on printer
#define RX_PIN 5    // GREEN WIRE   TX on printer

#define SS_PIN 10
#define RST_PIN 9

//SoftwareSerial RFID(2, 3);                // PIN2 125 KHZ
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);         // 16 zeichen 2 zeilen
SoftwareSerial mySerial(RX_PIN, TX_PIN);    // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);        // Pass addr to printer constructor


typedef struct
{
    String name;
    long uid;

} artikel_type;

const artikel_type artikel[] = {
    {
        .name = "sonstiges",
        .uid = 503220
    },
    {
        .name = "Brot",
        .uid = 2032910
    },
    {
        .name = "Nutella",
        .uid = 1681810
    },
    {
        .name = "Nudeln",
        .uid = 586010
    },
    {
        .name = "Pesto",
        .uid = 1027810
    },
    {
        .name = "Parmesan",
        .uid = 2127210
    },
    {
        .name = "Kaese",
        .uid = 1472110
    },
    {
        .name = "Butter",
        .uid = 1336410
    },
    {
        .name = "Milch",
        .uid = 1148510
    },
    {
        .name = "Reis",
        .uid = 1707710
    },
    {
        .name = "Sirup",
        .uid = 2012310
    }
}


void setup()
{
    //RFID.begin(9600);    //125 KHZ
    Serial.begin(9600); // DEBUG

    mySerial.begin(19200);
    printer.begin();
    printer.setSize('M');
    delay(3000);
    printer.sleep();

    SPI.begin();
    mfrc522.PCD_Init();
    pinMode (2, OUTPUT); // led
    lcd.begin();
}

void loop()
{
    long uidtemp = 0;

    /*if (RFID.available() > 0) //125 KHZ
      {
         i = RFID.read();
         Serial.print(i, DEC);
         Serial.print(" ");
      }*/

    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    long code = 0;

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        // Problem: uidByte[] = [0x0, 0xA] and uidByte[] = [0x1, 0x0] result in same code
        code = ((code + mfrc522.uid.uidByte[i]) * 10);
    }

    //Serial.print("Die Kartennummer lautet:");//DEBUG
    //Serial.println(code);

    for (int i = 0; i < 30; ++i)
    {
        if (artikel[i].uid == code)
        {
            //Serial.println(artikel[i].name);//DEBUG
            /*lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(artikel[i].name);
            lcd.setCursor(0,1);
            lcd.print("Einkaufen");*/

            if (artikel[i].uid == uidtemp)
            {
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
        }
    }
}
