#ifndef BOARD
#define BOARD

#include <SPI.h>
#include <MFRC522.h>
#include "webserver.h"

#define RELAY D3
#define BUZZER D2
#define RESET D9
#define SLAVESELECT D14

#define CREDENTIAL "C2 D6 F7 2C"

MFRC522 rfid(SLAVESELECT, RESET);

bool isInitialized = false;
bool isStandBy = false;

class Board{
    public:
    static void Setup()
    {
        pinMode(RELAY, OUTPUT);
        pinMode(BUZZER, OUTPUT);
        pinMode(LED_BUILTIN, OUTPUT);

        SPI.begin();
        rfid.PCD_Init();
        delay(10);

        WebServer::Setup();
        WebServer::Start();
    }

    class RFID{
        public:
            static void Open()
            {
                if(isInitialized)
                    return;

                Serial.println("PREPARING. PLEASE WAIT.");
                digitalWrite(LED_BUILTIN, LOW);
                digitalWrite(RELAY, LOW);
                digitalWrite(BUZZER, HIGH);
                delay(100);
                digitalWrite(LED_BUILTIN, HIGH);
                digitalWrite(RELAY, HIGH);
                digitalWrite(BUZZER, LOW);
                isInitialized = true;
            }

            static void Run()
            {
                if(toggle)
                    Authenticate();
                else
                    Registration();
            }

            static void Registration()
            {
                Serial.println("[ STAND BY ... ]");
                delay(1000);
            }

            static void Authenticate()
            {
                // Serial.println("[ STAND BY ... ]");
                delay(1000);

                if(!rfid.PICC_IsNewCardPresent())
                    return;

                if(!rfid.PICC_ReadCardSerial())
                    return;

                Serial.print("UID HEX: ");
                String content = "";
                for (byte i = 0; i < rfid.uid.size; i++)
                {
                    byte uid = rfid.uid.uidByte[i];
                    String letter = uid < 0x10 ? " 0" : " ";

                    Serial.print(letter);
                    Serial.print(uid, HEX);
                    content.concat(String(letter));
                    content.concat(String(uid, HEX));
                }

                Serial.println();
                Serial.print("AUTH: ");
                content.remove(0, 1);
                content.toUpperCase();
                // Serial.println(content);

                content.replace(" ", "%20");
                const char* uri = content.c_str();
                String auth = WebServer::httpGetRequestAuth(apiAuth, uri);
                Serial.println(auth);

                if(auth.substring(0) == "granted")
                {
                    Serial.println("ACCESS GRANTED.");
                    digitalWrite(RELAY, LOW);
                    digitalWrite(LED_BUILTIN, LOW);
                    delay(2500);
                    digitalWrite(RELAY, HIGH);
                    digitalWrite(LED_BUILTIN, HIGH);
                }
                else
                {
                    Serial.println("ACCESS DENIED.");
                    digitalWrite(BUZZER, HIGH);
                    delay(500);
                    digitalWrite(BUZZER, LOW);
                }
            }
    };

};

#endif