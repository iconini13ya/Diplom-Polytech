#include "SoftwareSerial.h" //библиотека для создания сериал порта на любом пине
void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    Serial.println(Serial.read()); 
    delay(200);
    }
}
