#include <ESP8266WiFi.h>                                // Подключаем библиотеку ESP8266WiFi
#include <ESP8266WebServer.h>                           // Подключаем библиотеку Wire
 
const char* ssid = "MyESP";                             // Название Моей создаваемой WiFi сети

ESP8266WebServer HTTP(80);                              // Указываем порт Web-сервера (80) по дефолту

int pinmode=0;
void setup() 
{
  Serial.begin(9600);                                   // Скорость передачи 9600 (дефолт) 
  delay(10);                
  WiFi.softAP(ssid);                                    // Создание точки WiFi с названием ssid
  HTTP.begin();                                         // Создание веб сервера на 80 порту

  Serial.println("My ip adress");                       //Вывод ip адреса для подключения по федолту (192.168.4.1)
  Serial.println(WiFi.softAPIP());
  
  HTTP.on("/changeDiodStatus",[](){
    Serial.print(!pinmode);
    pinmode=!pinmode;
    HTTP.send(200,"text/plain","Status changed");
    });              
}
 
void loop() 
{
HTTP.handleClient();
}
