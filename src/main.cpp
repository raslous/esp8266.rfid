#include <Arduino.h>
#include "board.h"

void setup() 
{
    Serial.begin(115200);
    while (!Serial);

    Board::Setup();  
}

void loop()
{
    Board::RFID::Open();
    Board::RFID::Run();
    // Board::RFID::Authenticate();
}