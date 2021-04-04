// ЭТО СКЕТЧ ПЕРЕДЕТЧИКА (ДАТЧИК)!!!
//--------------------- НАСТРОЙКИ ----------------------
#define CH_NUM 0x60   // номер канала (должен совпадать с приёмником)

// УРОВЕНЬ МОЩНОСТИ ПЕРЕДАТЧИКА
// На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
#define SIG_POWER RF24_PA_MAX

// СКОРОСТЬ ОБМЕНА
// На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
// должна быть одинакова на приёмнике и передатчике!
// при самой низкой скорости имеем самую высокую чувствительность и дальность!!
// ВНИМАНИЕ!!! enableAckPayload (основаня функция отправки сообщений в ответ на полученные) НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!
#define SIG_SPEED RF24_1MBPS
//--------------------- НАСТРОЙКИ ----------------------

//--------------------- БИБЛИОТЕКИ ----------------------
#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
#include <avr/eeprom.h> //Библиотека хранения данных внутри Arduino
RF24 radio(9, 10); // "создать" модуль на пинах 7 и 10 
//--------------------- БИБЛИОТЕКИ ----------------------

//--------------------- ПЕРЕМЕННЫЕ ----------------------

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
byte Data[3]; //массив для посылаемых данных [0] - ID датчика (берется из постоянной памяти), [1] - тип датчика , [2] - показания датчика 
byte callbackData[4]; //массив принятых от Базы данных
struct sensorSettings { //Переменная типа struct для зранения данных о датчике
    byte id;
    byte type;
  };
sensorSettings mySensor;
//--------------------- ПЕРЕМЕННЫЕ ----------------------

void setup() {
    Serial.begin(9600); //открываем порт для связи с ПК
    radioSetup();
    readSensorSettings(mySensor);
    Data[0]=mySensor.id;
    Data[1]=mySensor.type;
    Data[2]=0;
    Serial.println("Уел на регистрацию");
    if(Data[0] == 0)
    {
      while(radio.write(Data, sizeof(Data)) == false){
         Serial.println("Пишу");
         delay(50);
        }
      Serial.println("Начинаю слушать");
      radio.startListening();
      while(true){
        if( radio.available() ){// Если в буфере имеются принятые данные, то ...
        Serial.println("Читаю приходящую инфу");
        radio.read(&callbackData, sizeof(callbackData));       // Читаем данные из буфера в массив myData указывая сколько всего байт может поместиться в массив.
        if(callbackData[0]== Data[0] && callbackData[1]== Data[1]){
          Serial.println("О, это все мне ?");
          writeSensorSettings(callbackData[2],callbackData[3]);
          Data[0]=callbackData[2];
          Data[1]=callbackData[3];
          Serial.print("Новый id"); Serial.println(Data[0]);
          Serial.print("Новый type"); Serial.println( Data[1]);
          radio.stopListening();
          Serial.println("Перестал слушать");
          break; 
          }
        } 
      }
    }



// clearSensorSettings();
//for(int i =0; i<90; i++){
//
//   Serial.print("Info "); Serial.println(eeprom_read_byte(i));
//   delay(5);
//  }
  
}

void loop() {
  Serial.print("Новый id в цикле"); Serial.println(Data[0]);
  Serial.print("Новый type в цикле"); Serial.println( Data[1]);
}

void radioSetup(){ //настройка радио модуля 
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);//режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);//(время между попыткой достучаться, число попыток)
  radio.setPayloadSize(32);     //размер пакета, в байтах
  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.openReadingPipe(1, address[1]);
  radio.setChannel(CH_NUM);  //выбираем канал для связи
  radio.setPALevel (SIG_POWER); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate(SIG_SPEED); // скорость обмена
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  radio.powerUp();         // начать работу
  radio.stopListening();   // не слушаем радиоэфир, мы передатчик
  }
  
void clearSensorSettings(){
    sensorSettings mySensor; //объявление переменной mySensor
    mySensor.id=0; //задание поля id значение 255 (по дефолту все поля EEPROM =255)
    mySensor.type=1; //задание поля type значение 255 (по дефолту все поля EEPROM =255)
    eeprom_write_block((void*)&mySensor, 0, sizeof(mySensor)); //запись переменной mySensor в поле 0. в поле 1 также появится запись (type) поскольку каждая ячейка eeprom хранит только 1 байт информации.
  }
  
void writeSensorSettings(byte id, byte type){
    sensorSettings mySensor;
    mySensor.id=id;
    mySensor.type=type;
    eeprom_write_block((void*)&mySensor, 0, sizeof(mySensor));
  }
  
void readSensorSettings(sensorSettings& settings){
    eeprom_read_block((void*)&settings, 0, sizeof(settings));
  }
