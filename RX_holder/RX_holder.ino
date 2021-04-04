// ЭТО СКЕТЧ ПРИЁМНИКА (БАЗА)!!!
//--------------------- НАСТРОЙКИ ----------------------
#define CH_NUM 0x60   // номер канала, должен совпадать с передатчиком (датчик)

// УРОВЕНЬ МОЩНОСТИ ПЕРЕДАТЧИКА
#define SIG_POWER RF24_PA_MAX

// СКОРОСТЬ ОБМЕНА ДАННЫМИ
// должна быть одинакова на приёмнике и передатчике!
#define SIG_SPEED RF24_1MBPS
//--------------------- НАСТРОЙКИ ----------------------

//--------------------- БИБЛИОТЕКИ ---------------------
#include "nRF24L01.h"            // библиотеки для радио модуля
#include "RF24.h"
RF24 radio(9, 10);              // создать радио модуль на пинах 9 и 10
#include "SoftwareSerial.h"     //библиотека для создания сериал порта на любом пине
SoftwareSerial SIM800(2,3);     //создать сериал порт для прослушки на пинах 2,3
//--------------------- БИБЛИОТЕКИ ---------------------

//--------------------- ПЕРЕМЕННЫЕ ----------------------
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};     //возможные номера труб
byte callbackData[3];                                                           // массив принятых данных (id от кого, type тип датчика, data что передал)
byte Data[4];                                                                   //массив отправляемых данных (id кому , type кому , id новый , type новый)
byte pipeNo;                                                                    //Байтовая переменная хранения номера трубы для отправки
struct Sensor {                                                                 //Переменная типа struct для хранения данных о датчике
  byte id;
  byte type;
  byte data;
};
Sensor cashDataToSend;                                                          // кешевая переменная для отправки данных
String _response = "";                                                          // Переменная для хранения ответа модуля sim800L
//--------------------- ПЕРЕМЕННЫЕ ----------------------

void setup() {
  Serial.begin(9600);    //открываем порт для связи с ПК
  radioSetup();          //ф-я настройки радио модуля
  simSetup();            //ф-я настройки модуля sim800L


//  for(int i =0; i<54; i++){
////    writeNewSensorSettings(0,1,mySensor);
////    clearSensorById(i);
////    delay(200);
//   Serial.print("Info "); Serial.println(eeprom_read_byte(i));
//   delay(5);
//  }

}
void loop() {
 
// if(radio.available()){
//  Serial.println("Читаю");
//  radio.read(&callbackData, sizeof(callbackData));
//  if(callbackData[0]==0){
//    Serial.println("Новенький");
//    radio.stopListening();
//    Serial.println("Перестал читать");
//    writeNewSensorSettings(callbackData[1],callbackData[2],cashDataToSend);
//    Data[0]= callbackData[0];
//    Data[1]= callbackData[1];
//    Data[2]= cashDataToSend.id;
//    Data[3]= cashDataToSend.type;
//    Serial.print("Новый id"); Serial.println(Data[2]);
//    Serial.print("Новый type"); Serial.println( Data[3]);
//    while(radio.write(Data, sizeof(Data)) == false){
//      Serial.println("Пишу");
//      delay(50);
//      }
//    Serial.println("Отправил");  
//    radio.startListening();
//    Serial.println("Начинаю слушать");
//    }
//  }  
//
//  if(Serial.available()){
//    bustatus=Serial.parseInt();
//    Serial.print(bustatus);
//    switch(bustatus){
//      case 2:
//        Serial.print("Я на лоу");
//        digitalWrite(4,LOW);
//        break;
//      case 1:
//        Serial.print("Я на хай");
//        digitalWrite(4,HIGH);
//        break;
//      }
//    }
}

void radioSetup() {                        // настройка радио модуля
  radio.begin();                           // активировать модуль
  radio.setAutoAck(1);                     // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);                 // (время между попыткой достучаться, число попыток)
  radio.setPayloadSize(32);                // размер пакета, байт
  radio.openReadingPipe(1, address[0]);    // хотим слушать трубу 0
  radio.openWritingPipe(address[1]);       // хотим писать на трубу по первому адресу
  radio.setChannel(CH_NUM);                // выбираем канал
  radio.setPALevel(SIG_POWER);             // задаем уровень мощности передатчика
  radio.setDataRate(SIG_SPEED);            // задаем скорость обмена
                                           // должна быть одинакова на приёмнике и передатчике!
                                           // при самой низкой скорости имеем самую высокую чувствительность и дальность!! (логично)

  radio.powerUp();                         // начать работу
  radio.startListening();                  // начинаем слушать эфир, мы приёмный модуль
} 

void simSetup(){
  SIM800.begin(9600);                      //Скорость порта для связи Arduino с GSM модемом    
  delay(100);
  sendATCommand("AT", true);               //Настраиваем Sim модуль для общения (скорость)
  sendATCommand("AT+CMGF=1", true);        //Включаем функцию отправки сообщений
  }

void sendSMS(String phone, String message)                      //ф-я отправки sms 
{
  sendATCommand("AT+CMGS=\"" + phone + "\"", true);             // Переходим в режим ввода текстового сообщения
  sendATCommand(message + "\r\n" + (String)((char)26), true);   // После текста отправляем перенос строки и Ctrl+Z
}

bool isItFreeCell(int num) {                    //ф-я проверки клетки eeprom на пустоту
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

void clearSensorById(int id) {                  //ф-я очистки клеток памяти eeprom от сенсора по его id
  for (int i = 10; i < 1000; i= i + 3) 
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

void writeNewSensorSettings(byte type, byte data, Sensor& CallbackData) { //ф-я записи нового датчика в память eeprom (дывается id)
  Sensor mySensor;
  mySensor.type = type;
  mySensor.data = data;
  for (int i = 10; i < 1000; i = i + 3) {
    Serial.print("i= "); Serial.println(i) ;
    if (isItFreeCell(i) == true) {
      delay(500);
      if (i==10){
         mySensor.id=1;
        }else {
         mySensor.id = eeprom_read_byte((uint8_t*)i-3)+1;           //Чтение из eeprom 
          }
      Serial.print("Старый id = "); Serial.println(mySensor.id);
        Serial.print("Новый id = "); Serial.println(eeprom_read_byte((uint8_t*)i-3)) ;
      eeprom_write_block((void*)&mySensor, (int*)i, sizeof(mySensor));
      eeprom_read_block((void*)&CallbackData, (int*)i, sizeof(CallbackData)); 
      break;
    }
  }
}

void readSensorSettingsById(int id,Sensor& callbackData){         //ф-я вывода данных о сенсое по id
    for (int i =10;i<1000;i= i+3){
       if (eeprom_read_byte((uint8_t*)i) == id) {
        eeprom_read_block((void*)&callbackData, (int*)i, sizeof(callbackData));
      }
    }
  }
  
void writeMainPhone(long phomeNumber){      //ф-я записи основонго номера телефона long поскольку int(16 бит) -32768 до 32767 и по этим причинам не подходит для номера телефона
  int n=0;
  for(int i=5;i>0;i--){
//    Serial.println(i-n);
//    Serial.println(phomeNumber%100);
    delay(50);
    eeprom_write_byte(n, phomeNumber%100);
    delay(50);
    n++;
    phomeNumber/=100;
//    Serial.println(n);
//    Serial.println(phomeNumber);
    }
  }
void writeAddPhone(long phomeNumber){           //ф-я записи вторичного номера телефона long поскольку int(16 бит) -32768 до 32767 и по этим причинам не подходит для номера телефона
  int n=0;
  for(int i=5;i>0;i--){
//    Serial.println(i-n);
//    Serial.println(phomeNumber%100);
    delay(50);
    eeprom_write_byte(n+5, phomeNumber%100);
    delay(50);
    n++;
    phomeNumber/=100;
//    Serial.println(n);
//    Serial.println(phomeNumber);
    }
  }
    
String waitResponse() {                                           // Функция ожидания ответа и возврата полученного результата
  String _resp = "";                                              // Переменная для хранения результата
  long _timeout = millis() + 10000;                               // Переменная для отслеживания таймаута (10 секунд)
  while (!SIM800.available() &&  millis() < _timeout)  {};        // Ждем ответа или выдидаем таймаут в 10 секунд
  if (SIM800.available()) {                                       // Если есть, что считывать...
    _resp = SIM800.readString();                                  // ... считываем и запоминаем
  }
  else {                                                          // Если пришел таймаут, то...
    Serial.println("Timeout...");                                 // ... оповещаем об этом и...
  }
  return _resp;                                                   // ... возвращаем результат. Пусто, если проблема
}

String sendATCommand(String cmd, bool waiting) {
  String _resp = "";                            // Переменная для хранения результата
  Serial.println(cmd);                          // Дублируем команду в монитор порта
  SIM800.println(cmd);                          // Отправляем команду модулю
  if (waiting) {                                // Если необходимо дождаться ответа...
    _resp = waitResponse();                     // ... ждем, когда будет передан ответ
    if (_resp.startsWith(cmd)) {  // Убираем из ответа дублирующуюся команду
      _resp = _resp.substring(_resp.indexOf("\r", cmd.length()) + 2);
    }
    Serial.println(_resp);                      // Дублируем ответ в монитор порта
  }
  return _resp;                                 // Возвращаем результат. Пусто, если проблема
}
