#include <ESP8266WiFi.h>                                // Подключаем библиотеку ESP8266WiFi
#include <ESP8266WebServer.h>                                    // Подключаем библиотеку Wire
 
const char* ssid = "MyESP";               // Название Вашей WiFi сети

ESP8266WebServer HTTP(80);                                  // Указываем порт Web-сервера
 
void setup() 
{
  Serial.begin(9600);                                 // Скорость передачи 115200 
  delay(10);                                            // Пауза 10 мкс
  WiFi.softAP(ssid);                           // Подключение к WiFi Сети
  HTTP.begin();

  Serial.println("My ip adress");
  Serial.println(WiFi.softAPIP());
  
  HTTP.on("/",[](){
    HTTP.send(200,"text/plain","Hi");
    });               // Печатаем полученный IP-адрес ESP
}
 
void loop() 
{
Serial.write(1);
delay(2000);
Serial.write(0);
delay(2000);
HTTP.handleClient();
}
