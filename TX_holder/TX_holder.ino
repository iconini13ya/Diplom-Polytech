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
byte callbackData[1]; //массив принятых от Базы данных
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
    if(Data[0] == 0)
    {
      Serial.println("Ушел на регистрацию");
      bool cash;
      radio.write(Data, sizeof(Data));
      while(true)
      {
        Serial.println("Пытаюсь читать");
        if( radio.available())
        {                       // Если в буфере имеются принятые данные из пакета подтверждения приёма, то ...
          Serial.println("Пришел ответ");
          radio.read(&callbackData, sizeof(callbackData));                 // Читаем данные из буфера в массив ackData указывая сколько всего байт может поместиться в массив.
          writeSensorSettings(callbackData[0],Data[1]);
          readSensorSettings(mySensor);
          Data[0]=mySensor.id;
          Data[1]=mySensor.type;
          Serial.print("Новый id"); Serial.println(mySensor.id);
          Serial.print("Новый type"); Serial.println(mySensor.type);
          break;
        }
      }
    }

//    if(Data[0] == 0)
//    {
//     Serial.println("Ушел на nрегистрацию"); 
//    
//    }
//    Serial.print("id"); Serial.println(mySensor.id);
//    Serial.print("type"); Serial.println(mySensor.type);

// clearSensorSettings();
//for(int i =0; i<90; i++){
//
//   Serial.print("Info "); Serial.println(eeprom_read_byte(i));
//   delay(5);
//  }
  
}

void loop() {
//    radio.write(&Data, sizeof(Data));                      // Отправляем данные из массива myData указывая сколько байт массива мы хотим отправить.
//    if( radio.isAckPayloadAvailable() ){                       // Если в буфере имеются принятые данные из пакета подтверждения приёма, то ...
//        Serial.println("Пришла ответочка");
//        radio.read(&callbackData, sizeof(callbackData));                 // Читаем данные из буфера в массив ackData указывая сколько всего байт может поместиться в массив.
//        writeSensorSettings(callbackData[0],Data[1]);
//        Data[0]=callbackData[0];
//        Serial.print("Новый id"); Serial.println(callbackData[0]);
//    }    
//    Data[2]=25;
//    radio.write(Data, sizeof(Data));
//    delay(100);     
}

void radioSetup(){ //настройка радио модуля 
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);//режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);//(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
//  radio.enableDynamicPayloads();
  radio.setPayloadSize(32);     //размер пакета, в байтах
  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
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
