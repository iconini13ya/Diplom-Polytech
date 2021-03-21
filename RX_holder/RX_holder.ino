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
RF24 radio(9,10); // "создать" модуль на пинах 9 и 10
//--------------------- БИБЛИОТЕКИ ---------------------

//--------------------- ПЕРЕМЕННЫЕ ----------------------
byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб
int Data[3];         // массив принятых данных
int callbackData[2]; //массив отправляемых данных
byte pipeNo;
//--------------------- ПЕРЕМЕННЫЕ ----------------------

void setup(){
  Serial.begin(9600); //открываем порт для связи с ПК
  radioSetup();
}

void loop() {                          
    while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
      radio.read( &Data, sizeof(Data) );         // чиатем входящий сигнал
      if (Data[0] == 0){
        callbackData[0]=1;
        callbackData[1]=0;
        radio.writeAckPayload(pipeNo, &callbackData, sizeof(callbackData));    // отправляем пакет телеметрии
        }
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
