#include <WiFi.h>
#include <esp_bt.h>
#include <esp_bt_main.h>

#define SIGNAL 19//input signal pin

const uint8_t ImageSize = 128;
volatile uint32_t ImageData[ImageSize*ImageSize] = {0};
volatile uint32_t ArrayIndex = 0;

volatile uint32_t SignalTimeStamp = 0;
volatile uint32_t SignalWidth = 0;


//these values have been tuned to the absolute limits this system can handle, going any less will cause unreliable data reception which will lead to corruption.
//the photodiode receiver circuit has been modifed in a way to decrease signal latency! a stock photodiode will not be able to reach these speeds unless modifed so be sure to increase the
//the transmitter and receiver ONBIT/OFFBIT which will reduce data rate at a cost of reliability. tune the right/left signal width deadzone such that the bit identifier deadzone statements dont intersect so bits are registered correctly 
//an oscilloscope is recommended for signal analysis and calibration
volatile uint32_t ONBIT = 30;//bit width in unit of microseconds
volatile uint32_t OFFBIT = 10;
volatile uint8_t RightSideSignalWidthDeadzone = 14;
volatile uint8_t LeftSideSignalWidthDeadzone = 5;

volatile bool SignalInitialRise = false;
volatile bool PreambleCheck = false;
volatile bool BitLogging = true;
volatile bool OutputData = false;
volatile int8_t Counter = 0;
volatile uint8_t PreambleCounter = 0;
volatile uint8_t Bit = 0;


void IRAM_ATTR SignalChange(){
  bool pinState = (REG_READ(GPIO_IN_REG) >> SIGNAL) & 0b1;
  if(pinState){
    SignalTimeStamp = esp_timer_get_time();
    SignalInitialRise = true;
  }
  else{
    if(SignalInitialRise && BitLogging){
       SignalWidth = esp_timer_get_time() - SignalTimeStamp;
       if(SignalWidth >= ONBIT-LeftSideSignalWidthDeadzone && SignalWidth <= ONBIT+RightSideSignalWidthDeadzone){
          Bit = 1;
          PreambleCounter++;
       }
       else if(SignalWidth >= OFFBIT-LeftSideSignalWidthDeadzone && SignalWidth <= OFFBIT+RightSideSignalWidthDeadzone){
          Bit = 0;
          PreambleCounter = 0;
       }else{
        return;
       }
    }
    if(PreambleCheck){
      Counter++;
      ImageData[ArrayIndex] <<= 1;
      ImageData[ArrayIndex] |= Bit;
      if(Counter == 28){
        PreambleCheck = false;
        Counter = 0;
        ArrayIndex++;
      }
      if(ArrayIndex >= ImageSize*ImageSize){
        BitLogging = false;
        OutputData = true;
        ArrayIndex = 0;
        PreambleCounter = 0;
      }
    }
    if(PreambleCounter == 9){
      PreambleCheck = true;
      PreambleCounter = 0;
    }
  }
}


void setup(){

  Serial.begin(500000);

  attachInterrupt(digitalPinToInterrupt(SIGNAL), SignalChange, CHANGE);

  WiFi.disconnect(true); 
  WiFi.mode(WIFI_OFF);
  
  btStop();            
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
}

void loop(){
  if(OutputData){
    for(auto& element : ImageData){
      Serial.println(element);
    }
    OutputData = false;
  }
}