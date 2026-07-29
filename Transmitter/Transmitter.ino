#include <WiFi.h>
#include <esp_bt.h>
#include <esp_bt_main.h>


#define SIGNAL 18//output signal pin

enum class TransmitterState{
  TransmitPreamble,
  TransmitBitReset,
  TransmitPositionX,
  TransmitPositionY,
  TransmitColor
};

bool RunBitStream = true;
bool ArrayLimitReached = false;
TransmitterState CASE = TransmitterState::TransmitPreamble;

uint16_t ArrayIndex = 0;
uint8_t BitIndex = 7;//bit index starts at right most(LSB) index 0, because there are 8 bits in a byte the MSB bit is at index 7 as 
uint8_t PreambleBitIndex = 8;
uint8_t Counter = 0;

const uint8_t ImageSize = 128;
const uint16_t Preamble[1] = {0b111111111};

const uint8_t ImageData[ImageSize*ImageSize] = {};//paste data from "ImageToBinary\Output\" here
uint8_t PositionX = 0;
uint8_t PositionY = 0;

uint32_t ONBIT = 30;//bit width/spacing in microseconds
uint32_t OFFBIT = 10;
uint32_t BITWIDTH = 0;
uint32_t BITSPACING = 17;

void setup(){
  Serial.begin(500000);

  WiFi.disconnect(true); //turn off wifi and bluetooth to reduce interference
  WiFi.mode(WIFI_OFF); 
  
  btStop();             
  esp_bt_controller_disable();
  esp_bt_controller_deinit();

  pinMode(SIGNAL, OUTPUT);
  digitalWrite(SIGNAL, HIGH);
  delay(1000);

}

void loop(){
  if(RunBitStream){
    Counter++;
    switch(CASE){

        case TransmitterState::TransmitBitReset:{
          if(Counter == 10){
            CASE = TransmitterState::TransmitPositionX;
            BITWIDTH = OFFBIT;
            break;
          }
          if(Counter == 19){
            CASE = TransmitterState::TransmitPositionY;
            BITWIDTH = OFFBIT;
            break;
          }
          if(Counter == 28){
            CASE = TransmitterState::TransmitColor;
            BITWIDTH = OFFBIT;
            break;
          }
          if(Counter == 37){
            if(ArrayLimitReached){
              RunBitStream = false;
              PositionX = 0;
              PositionY = 0;
            }
            BITWIDTH = OFFBIT;
            CASE = TransmitterState::TransmitPreamble;
            Counter = 0;
            break;
          }
          break;
        }


        case TransmitterState::TransmitPreamble:{
          uint8_t bit = bitRead(Preamble[0], PreambleBitIndex);
          if(bit){
            BITWIDTH = ONBIT;
          }else{
            BITWIDTH = OFFBIT;
          }
          if(PreambleBitIndex == 0){
            PreambleBitIndex = 8;
            CASE = TransmitterState::TransmitBitReset;
            break;
          }
          PreambleBitIndex--;
          break;
        }

        case TransmitterState::TransmitPositionX:{
          if(PositionX == ImageSize){
            PositionX = 0;
            PositionY++;
          }
          uint8_t bit = bitRead(PositionX, BitIndex);
          if(bit){
            BITWIDTH = ONBIT;
          }else{
            BITWIDTH = OFFBIT;
          }
          if(BitIndex == 0){
            PositionX++;
            BitIndex = 7;
            CASE = TransmitterState::TransmitBitReset;
            break;
          }
          BitIndex--;
          break;
        }

        case TransmitterState::TransmitPositionY:{
          uint8_t bit = bitRead(PositionY, BitIndex);
          if(bit){
            BITWIDTH = ONBIT;
          }else{
            BITWIDTH = OFFBIT;
          }
          if(BitIndex == 0){
            BitIndex = 7;
            CASE = TransmitterState::TransmitBitReset;
            break;
          }
          BitIndex--;
          break;
        }

        case TransmitterState::TransmitColor:{
          uint8_t bit = bitRead(ImageData[ArrayIndex], BitIndex);
          if(bit){
            BITWIDTH = ONBIT;
          }else{
            BITWIDTH = OFFBIT;
          }
          if(BitIndex == 0){
            BitIndex = 7;
            ArrayIndex++;
            if(ArrayIndex >= ImageSize*ImageSize){
            ArrayLimitReached = true;
            }
            CASE = TransmitterState::TransmitBitReset;
            break;
          }
          BitIndex--;
          break;
        }

    }
    digitalWrite(SIGNAL, LOW);
    delayMicroseconds(BITWIDTH);
    digitalWrite(SIGNAL, HIGH);
    delayMicroseconds(BITSPACING);

    
  }

}
