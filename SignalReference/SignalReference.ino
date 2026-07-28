#include <WiFi.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#define SIGNAL 18

uint32 SignalWidth = 30;//microseconds, an oscilloscope is used for calibrating the receiver side pulse width

void setup() {
  pinMode(SIGNAL, OUTPUT);

  WiFi.disconnect(true); 
  WiFi.mode(WIFI_OFF); 
  
  btStop();             
  esp_bt_controller_disable();
  esp_bt_controller_deinit();

}

void loop() {
  digitalWrite(SIGNAL, LOW);
  delayMicroseconds(SgnalWidth);
  digitalWrite(SIGNAL, HIGH);
  delayMicroseconds(SignalWidth*4);

}
