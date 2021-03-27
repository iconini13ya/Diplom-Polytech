// ЭТО СКЕТЧ ПРИЁМНИКА (БАЗА)!!!
//--------------------- НАСТРОЙКИ ----------------------
#define CH_NUM 0x60   // номер канала, должен совпадать с передатчиком (датчик)

// УРОВЕНЬ МОЩНОСТИ ПЕРЕДАТЧИКА
// На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
#define SIG_POWER RF24_PA_MAX

// СКОРОСТЬ ОБМЕНА
// На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
// должна быть одинакова на приёмнике и передатчике!
// при самой низкой скорости имеем самую высокую чувствительность и дальность!!
// ВНИМАНИЕ!!! enableAckPayload (основная функция обмена данными) НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!
#define SIG_SPEED RF24_1MBPS
//--------------------- НАСТРОЙКИ ----------------------

//--------------------- БИБЛИОТЕКИ ---------------------
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10
//--------------------- БИБЛИОТЕКИ ---------------------

//--------------------- ПЕРЕМЕННЫЕ ----------------------
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
byte Data[3];         // массив принятых данных
byte callbackData[2]; //массив отправляемых данных
byte pipeNo; //Байтовая переменная хранения номера трубы для отправки
struct Sensor { //Переменная типа struct для хранения данных о датчике
  byte id;
  byte type;
  byte data;
};
//--------------------- ПЕРЕМЕННЫЕ ----------------------

void setup() {
  Serial.begin(9600); //открываем порт для связи с ПК
  radioSetup();
//  for(int i =0; i<60; i++){
////    writeNewSensorSettings(0,1,mySensor);
//    clearSensorById(i);
////    delay(200);
//   Serial.print("Info "); Serial.println(eeprom_read_byte(i));
//   delay(5);
//  }
 
}

void loop() {
      while( radio.available(&pipeNo)){  
        Serial.print("Ушел на прослушку");// слушаем эфир со всех труб
        radio.read( &Data, sizeof(Data) ); 
         Serial.print("Принял");// чиатем входящий сигнал
        if (Data[0] == 0){
          Sensor newSensor;
          writeNewSensorSettings(Data[1],Data[2],newSensor);
          callbackData[0]=newSensor.id;
          callbackData[1]=newSensor.type;
          Serial.print("Отправил инфу на датчик");
          radio.writeAckPayload(pipeNo, &callbackData, sizeof(callbackData));
          Serial.print("Отправиляю"); Serial.print(callbackData[0]); Serial.println(callbackData[1]);
          }
        Serial.print("SEND: ID "); Serial.println(callbackData[0]);
        Serial.print("SEND: TYPE "); Serial.println(callbackData[1]);
  
        Serial.print("Recieved: ID "); Serial.println(Data[0]);
        Serial.print("Recieved: TYPE "); Serial.println(Data[1]);
        Serial.print("Recieved: DATA "); Serial.println(Data[2]);
     }
     
}

void radioSetup() {         // настройка радио модуля
  radio.begin();               // активировать модуль
  radio.setAutoAck(1);         // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);     // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);    // размер пакета, байт
  radio.openReadingPipe(1, address[0]);   // хотим слушать трубу 0
  radio.setChannel(CH_NUM);               // выбираем канал
  radio.setPALevel(SIG_POWER);            // уровень мощности передатчика
  radio.setDataRate(SIG_SPEED);           // скорость обмена
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();         // начать работу
  radio.startListening();  // начинаем слушать эфир, мы приёмный модуль
}

bool isItFreeCell(int num) {
  Serial.print("isItFreeCell() <-"); Serial.println(num) ;
  byte cash = eeprom_read_byte((uint8_t*)num);
  Serial.print("Что находится в ячейке num"); Serial.println(cash) ;
  delay(15);
  Serial.print(cash); Serial.print(" = "); Serial.print(cash); Serial.println(" ?") ;
  if (cash == 255) {
    Serial.println(" Да") ;
    return true;
  }
  else {
    Serial.println(" Нет") ;
    return false;
  }
}

void clearSensorById(int id) {
  for (int i = 0; i < 1000; i= i + 3) 
  {
    if (eeprom_read_byte((uint8_t*)i) == id) 
    {
      delay(10);
      eeprom_write_byte((byte*)i, 255);
      delay(10);
      eeprom_write_byte((byte*)i + 1, 255);
      delay(10);
      eeprom_write_byte((byte*)i + 2, 255);
      delay(10);
      break;
    }
  }
}

void writeNewSensorSettings(byte type, byte data, Sensor& CallbackData) {
  Sensor mySensor;
  mySensor.type = type;
  mySensor.data = data;
  for (int i = 0; i < 1000; i = i + 3) {
    Serial.print("i= "); Serial.println(i) ;
    if (isItFreeCell(i) == true) {
      delay(500);
      if (i==0){
         mySensor.id=1;
        }else {
         mySensor.id = eeprom_read_byte((uint8_t*)i-3)+1; //Чтение из eeprom 
          }
      Serial.print("Старый id = "); Serial.println(mySensor.id);
        Serial.print("Новый id = "); Serial.println(eeprom_read_byte((uint8_t*)i-3)) ;
      eeprom_write_block((void*)&mySensor, (int*)i, sizeof(mySensor));
      eeprom_read_block((void*)&CallbackData, (int*)i, sizeof(CallbackData)); 
      break;
    }
  }
}

void readSensorSettingsById(Sensor& mySensor, int id){
    for (int i =0;i<1000;i= i+3){
       if (eeprom_read_byte((uint8_t*)i) == id) {
        eeprom_read_block((void*)&mySensor, (int*)i, sizeof(mySensor));
      }
    }
  }
