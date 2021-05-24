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
SoftwareSerial SIM800(4,5);     //создать сериал порт для прослушки на пинах 2,3
SoftwareSerial WIFI(2,3); //TX желтый RX зеленый
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
String data;
String additionalInfo;
String command;
String otvet;
//--------------------- ПЕРЕМЕННЫЕ ----------------------

void setup() {
  WIFI.begin(9600);
  Serial.begin(9600);    //открываем порт для связи с ПК
  radioSetup();          //ф-я настройки радио модуля
  simSetup();            //ф-я настройки модуля sim800L
  WIFI.listen();
  delay(10);
//  sendSMS("+79520534351","Hi");
//  writePhoneNumber(1,"+79520534351");
//
  for(int i =0; i<50; i++){
//    writeNewSensorSettings(0,1,mySensor);
//    clearSensorById(i);
//    delay(200);
   Serial.print("Info "); Serial.println(eeprom_read_byte(i));
   delay(5);
  }
//
//  deletePhoneNumber(1);
//  delay(100);
//  Serial.println(getPhoneNumber(1));

//    writePhoneNumber(1,"+79520534351");
pinMode(A0, OUTPUT);
analogWrite(A0,255);
}

void(* resetFunc) (void) = 0;

void loop() { 
 if(radio.available()){
  Serial.println("Что-то пришло ,читаю");
  radio.read(&callbackData, sizeof(callbackData));
  readSensorSettingsById(callbackData[0],cashDataToSend);
  delay(100);
 if(callbackData[0]==0 && callbackData[1]!= 0){
    registerNewSensor();
      for(int i =0; i<50; i++){
//    writeNewSensorSettings(0,1,mySensor);
//    clearSensorById(i);
//    delay(200);
   Serial.print("Info "); Serial.println(eeprom_read_byte(i));
   delay(5);
  }
 }else if (cashDataToSend.id == callbackData[0] && cashDataToSend.type == callbackData[1]){
  otvet="";
  Serial.println("Аларм");
  Serial.println(callbackData[0]);
  Serial.println(callbackData[1]);
  otvet = "Srabotal datchik ";
  if (callbackData[1] == 1){
    otvet = otvet+"zvonok";
    }else if (callbackData[1]==2){
      otvet = otvet+"dim";
      }
  Serial.println(otvet);  
  delay(20);
  simSetup();
  sendSMS(getPhoneNumber(2),otvet);  
  simSetup();
  delay(200);
  sendSMS(getPhoneNumber(1),otvet);
  delay(2000);
  analogWrite(A0,0);
  delay(10);
  analogWrite(A0,255);
  delay(1000);
  resetFunc();
//  makeCall(getPhoneNumber(1));
  }   
}else{
  radio.flush_tx();
  }

 if(WIFI.available()){
  Serial.println("Есть инфа с wifi");
  data = WIFI.readString();
  delay(10);
  Serial.println(data);
  if(data.substring(data.indexOf(' ')+1)!=""){
    additionalInfo = data.substring(data.indexOf(' ')+1,data.length());
    }
  data.remove(data.indexOf(' '));
  command = data; 
  if(command == "addMainPhoneNumber"){
    Serial.println("Команда на добавление главного номера телефона");
    Serial.println(additionalInfo);
    writePhoneNumber(1,additionalInfo);
    delay(4000);
      analogWrite(A0,0);
  delay(10);
  analogWrite(A0,255);
  delay(1000);
    resetFunc();
    }

  if(command == "addAdditionalPhoneNumber"){
    Serial.println("Команда на добавление дополнительного номера телефона");
    Serial.println(additionalInfo);
    writePhoneNumber(2,additionalInfo);
    delay(2000);
       analogWrite(A0,0);
  delay(10);
  analogWrite(A0,255);
  delay(1000);
    resetFunc();
  }  
    
  if(command == "deleteMainPhoneNumber"){
    Serial.println("Команда на удаление главного номера телефона");
    deletePhoneNumber(1);
    }

  if(command == "deleteAdditionalPhoneNumber"){
    Serial.println("Команда на удаление второго номера телефона");
    deletePhoneNumber(2);
  }
    
  if(command == "getTelephone" || command == "getTelephone " ){
  String s;
  s= s + getPhoneNumber(1);
  s = s + " ";
  s = s + getPhoneNumber(2);
  Serial.println(s);
  WIFI.print(s);
  }

  if(command == "getSensors"){
  Serial.println("Отправил");
  Serial.println(parseSensors());
  WIFI.print(parseSensors());
  
  }
  
  if(command == "deleteSensor"){
    Serial.println("Команда на удаление сенсора");
    radio.stopListening();
    readSensorSettingsById(additionalInfo.toInt(),cashDataToSend);
    clearSensorById(additionalInfo.toInt());
    Data[0]= cashDataToSend.id;
    Data[1]= cashDataToSend.type;
    Data[2]= 0;
    Data[3]= 0;
    Serial.println(Data[0]);
    Serial.println(Data[1]);
        if (waitRadioResponse()){
          Serial.println("Отправил, удачное удаление датчика");  
          radio.startListening();
          Serial.println("Начинаю слушать");
      }else{
          Serial.println("Не достучался до датчика,но удалил");  
          radio.startListening();
        }
         
  }
    
 }


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
  SIM800.listen();  
  delay(100);
  sendATCommand("AT", true);               //Настраиваем Sim модуль для общения (скорость)
  sendATCommand("AT+CMGF=1", true);        //Включаем функцию отправки сообщений
  delay(100);
  WIFI.listen();
  delay(100);
  }

void makeCall(String phone)                      //ф-я отправки sms 
{
  SIM800.listen();
  delay(100);
  sendATCommand("ATD"+phone+";", false);
  WIFI.listen();
  delay(100);
}  

void sendSMS(String phone, String message)                      //ф-я отправки sms 
{
  SIM800.listen();
  delay(100);
  sendATCommand("AT+CMGS=\"" + phone + "\"", true);             // Переходим в режим ввода текстового сообщения
  sendATCommand(message + "\r\n" + (String)((char)26), true);   // После текста отправляем перенос строки и Ctrl+Z
  WIFI.listen();
  delay(100);
}

void registerNewSensor(){
    Serial.println("Новенький");
    radio.stopListening();
    Serial.println("Перестал читать");
    writeNewSensorSettings(callbackData[1],callbackData[2],cashDataToSend);
    Data[0]= callbackData[0];
    Data[1]= callbackData[1];
    Data[2]= cashDataToSend.id;
    Data[3]= cashDataToSend.type;
    Serial.print("Новый id"); Serial.println(Data[2]);
    Serial.print("Новый type"); Serial.println( Data[3]);
    if (waitRadioResponse()){
          Serial.println("Отправил, удачная регистрация");  
          radio.startListening();
          Serial.println("Начинаю слушать");
      }else{
          clearSensorById(Data[2]);
          Serial.println("Нет ответа, освободил id");  
          radio.startListening();
          Serial.println("Начинаю слушать");
        }
  }

bool waitRadioResponse(){
  bool _resp=false;
  long _timeout = millis() + 2000;                                             // Переменная для отслеживания таймаута (10 секунд)
  while (!_resp &&  millis() < _timeout)  {
    _resp = radio.write(Data, sizeof(Data));
    };                                                                         // Ждем ответа или выжидаем таймаут в 2 секунды
  if (_resp) {                                                     // Если есть, что считывать...
    return true;                                                               // ... считываем и запоминаем
  }
  else {                                                                       // Если пришел таймаут, то...
    return false;                                                              // ... оповещаем об этом и...
  }  
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
    for (int i =10;i<500;i= i+3){
       if (eeprom_read_byte((uint8_t*)i) == id) {
        eeprom_read_block((void*)&callbackData, (int*)i, sizeof(callbackData));
        break;
      }
    }
  }
  
void writePhoneNumber(int phoneType, String Phone){      //ф-я записи номеров телефона long поскольку int(16 бит) -32768 до 32767 и по этим причинам не подходит для номера телефона. phoneType - Главный номер =1, дополнительный номер телефона =2; phomeNumber должен быть вида +79.........
    Phone = Phone.substring(3);
    long phomeNumber = Phone.toInt();
    if(phoneType == 1){
      int n=0;
      for(int i=5;i>0;i--){
      delay(50);
      eeprom_write_byte(n, phomeNumber%100);
      delay(50);
      n++;
      phomeNumber/=100;
    }
      }else{
          int n=0;
          for(int i=5;i>0;i--){
            delay(50);
            eeprom_write_byte(n+5, phomeNumber%100);
            delay(50);
            n++;
            phomeNumber/=100;
            }
        }
  }

void deletePhoneNumber(int phoneType){      //ф-я удаления номеров телефона  phoneType - Главный номер =1, дополнительный номер телефона =2;
  if(phoneType == 1){
    for(int i=0;i<5;i++){
      delay(10);
      eeprom_write_byte(i, 0);
      delay(10);
      }
    }else {
        for(int i=5;i<10;i++){
          delay(10);
          eeprom_write_byte(i, 0);
          delay(10);
        }
      }
  }

String getPhoneNumber(int phoneType){      //ф-я получения основонго номеров телефона phoneType - Главный номер =1, дополнительный номер телефона =2;
  long readPhoneNumber=0;
  String phoneNumber;
  long mnozhitel=1;
  if(phoneType == 1){
    for(int i=0;i<5;i++){
      readPhoneNumber = mnozhitel * eeprom_read_byte(i)   + readPhoneNumber ;
      mnozhitel *= 100;
    }
    }else{
          for(int i=5;i<10;i++){
            readPhoneNumber = mnozhitel * eeprom_read_byte(i)   + readPhoneNumber ;
            mnozhitel *= 100;
          }   
      }
          if(readPhoneNumber < 1){
            phoneNumber = "";
            }
          else if(readPhoneNumber < 9999999){
            phoneNumber = "+7900";
            }else if (readPhoneNumber < 99999999){
              phoneNumber = "+790";
              }else{
                phoneNumber = "+79";
                }
          phoneNumber += readPhoneNumber; 
          return phoneNumber;
  }
  

  
String waitResponse() {                                           // Функция ожидания ответа и возврата полученного результата
  String _resp = "";                                              // Переменная для хранения результата
  long _timeout = millis() + 10000;                               // Переменная для отслеживания таймаута (10 секунд)
  while (!SIM800.available() &&  millis() < _timeout)  {};        // Ждем ответа или выжидаем таймаут в 10 секунд
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

String parseSensors(){
  Sensor cash;
  cash.id=0;
  int num=0;
  String parsedSensors;
  for (int i =1; i<=100; i++){
    readSensorSettingsById(i,cash);
    if(cash.id != 0){
//      Serial.println(cashString.length());
      parsedSensors = parsedSensors+" "+cash.id+" "+cash.type;
      num +=2;
      cash.id=0;
      cash.type=0;
      }
    }
    return parsedSensors;
  }
