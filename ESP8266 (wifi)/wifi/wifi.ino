

#include <ESP8266WiFi.h>                                // Подключаем библиотеку ESP8266WiFi
#include <ESP8266WebServer.h>                           // Подключаем библиотеку Wire
//#include <FS.h> 
#include <ESP8266FtpServer.h>
const char* ssid = "MyESP";                             // Название Моей создаваемой WiFi сети

ESP8266WebServer HTTP(80);                              // Указываем порт Web-сервера (80) по дефолту

String command;
String commandAdd="addMainPhoneNumber ";
String commandDelete="deleteMainPhoneNumber ";
String Maintelnum= "+79003450674";
int flag=1;



FtpServer ftpSrv;

void setup() 
{
  Serial.begin(9600);                                   // Скорость передачи 9600 (дефолт) 
  delay(10);                
  WiFi.softAP(ssid);                                    // Создание точки WiFi с названием ssid
  delay(10);
//  SPIFFS.begin();
  HTTP.begin();                                         // Создание веб сервера на 80 порту
  ftpSrv.begin("login","password");


  Serial.println("My ip adress");                       //Вывод ip адреса для подключения по федолту (192.168.4.1)
  Serial.println(WiFi.softAPIP());

  HTTP.on("/",[](){
    HTTP.send(200,"text/plain","/index.html");
    });  
  
  HTTP.on("/changeDiodStatus",[](){
    if(flag==1){
      command=commandAdd + Maintelnum;
      flag=2;
      }else{
        command=commandDelete;
        flag=1;
        }
    Serial.print(command);
    HTTP.send(200,"text/plain","Status changed");
    });  

}
 
void loop() 
{
HTTP.handleClient();
//ftpSrv.handleFTP(); 
}
