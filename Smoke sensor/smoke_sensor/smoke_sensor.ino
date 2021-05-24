// ЭТО СКЕТЧ Датчика дыма !!!
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
boolean alarmFlag=false;
int resetPin = A6;
int alarmPin = A5;
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
byte Data[3]; //массив для посылаемых данных [0] - ID датчика (берется из постоянной памяти), [1] - тип датчика , [2] - показания датчика 
byte callbackData[4]; //массив принятых от Базы данных
struct sensorSettings { //Переменная типа struct для хранения данных о датчике
    byte id;
    byte type;
  };
sensorSettings mySensor;

//--------------------- ПЕРЕМЕННЫЕ ----------------------

void setup() {
    Serial.begin(9600);
    radioSetup();
    initSensor();
    for(int i =0; i<10; i++){
   Serial.print("Info "); Serial.println(eeprom_read_byte(i));
   delay(5);
   }
   if (analogRead(resetPin)>1000){
    clearSensorSettings();
      while(true){
       Serial.println("Была произведена очистка памяти устройства. Поменяйте распололжение свича и перезагрузите устройство");
       delay(1000);
      }
    }
    delay(100);
      if(mySensor.id == 0){
      Serial.println("Пошел на регистрацию");   
      bool registr=false;
      do{
        registr=registrateSensor();
        }while(registr==false);      
        }   
}

void loop() {
  if(digitalRead(alarmPin)==1 && alarmFlag==false){
    alarmFlag=true;
    radio.stopListening();
    Data[2]=digitalRead(alarmPin);
    radio.write(Data, sizeof(Data));
    Serial.println("Отправил аларм");
    delay(200);
    radio.startListening();
    }else if (digitalRead(alarmPin)==0){
      alarmFlag=false;
      }  
  if(radio.available()){
    radio.read(&callbackData, sizeof(callbackData));
    if(callbackData[0]== Data[0] && callbackData[1]== Data[1]){
      if(callbackData[2]==0 && callbackData[3]==0){
      Serial.println("Очистка данных");
        clearSensorSettings();
        }
      }
    }
  
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
  radio.startListening();
  }
void initSensor(){
    readSensorSettings(mySensor);
    Data[0]=mySensor.id;
    Data[1]=mySensor.type;
    Data[2]=0; 
  }
  
void clearSensorSettings(){
    sensorSettings mySensor; //объявление переменной mySensor
    mySensor.id=0; //задание поля id значение 255 (по дефолту все поля EEPROM =255)
    mySensor.type=2; //задание поля type значение 255 (по дефолту все поля EEPROM =255)
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

bool radioWasDelivered(){
  bool _resp=false;
  long _timeout = millis() + 4000;                                             // Переменная для отслеживания таймаута (10 секунд)
  while (!_resp &&  millis() < _timeout)  {
    _resp = radio.write(Data, sizeof(Data));
    delay(100);
    };                                                                         // Ждем ответа или выжидаем таймаут в 2 секунды
  if (_resp) {                                                     // Если есть, что считывать...
    return true;                                                               // ... считываем и запоминаем
  }
  else {                                                                       // Если пришел таймаут, то...
    return false;                                                              // ... оповещаем об этом и...
  }  
}

bool registrateSensor(){
      radio.stopListening();
      delay(100);
      if(radioWasDelivered()){
        Serial.println("Сообщение доставлено");
        Serial.println("Начинаю слушать");
        radio.startListening();   
        long _timeout = millis() + 5000;
        while(!radio.available() && millis() < _timeout){}
        if(radio.available()){
            Serial.println("Что-то пришло, читаю");
            radio.read(&callbackData, sizeof(callbackData));
            if(callbackData[0]== Data[0] && callbackData[1]== Data[1]){
              Serial.println("О, это мне !");
              writeSensorSettings(callbackData[2],callbackData[3]);
              Data[0]=callbackData[2];
              Data[1]=callbackData[3];
              Serial.print("Новый id"); Serial.println(Data[0]);
              Serial.print("Новый type"); Serial.println( Data[1]);
              Serial.println("Перестал слушать");
              return true;
              }else{
                Serial.println("Это не мне");
                return false;
                }
            }else{
              Serial.println("Timeout........");
              Serial.println("Не пришел обратный ответ");
              return false;
              }
        }else{
          Serial.println("Сообщение не доставлено базе");
          return false;
          } 
  }
