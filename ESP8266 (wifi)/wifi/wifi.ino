#import "index.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
/* Put your SSID & Password */
const char* ssid = "MyArduino";  // Enter SSID here

 
ESP8266WebServer server(80);
 
void setup() {
  Serial.begin(9600);
  delay(100);
  WiFi.softAP(ssid);
//  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("HTTP server started");

  server.on("/", handleRoot);
  server.on("/phoneSettings", handlePhone);
  server.on("/deleteMainPhone", deleteMainPhone);
  server.on("/deleteAdditionalPhone", deleteAdditionalPhone);
  server.on("/additionalPhoneNumber", addAdditionalPhone);
  server.on("/mainPhoneNumber", addMainPhone);
  server.on("/deleteSensor", deleteSensor);
//  server.on("/mainPhoneNumber", !!!!);
//  server.on("/additionalPhoneNumber", !!!!!!);
}
void loop() {
  server.handleClient();
}

void handleRoot() {
  String s = MAIN_page;
  String htmlResponse;
  Serial.print("getSensors");
  String cashData;
  long _timeout = millis() + 4000; 
    while(!Serial.available() && millis() < _timeout){yield();}
    if(Serial.available()){
      delay(5);
      cashData = Serial.readString();
      delay(5);
      while (cashData.length()){
        String sensorId=cashData.substring(cashData.indexOf(" ")+1,cashData.indexOf(" ")+2);
        htmlResponse = htmlResponse + "<tr><td><form id="+sensorId+" action=deleteSensor> <input type=text value="+sensorId+" name=deleteSensor readonly> </form> </td>";
        cashData.remove(cashData.indexOf(" "),2);
        String sensorType = checkSensorType(cashData.substring(cashData.indexOf(" ")+1,cashData.indexOf(" ")+2));
        htmlResponse = htmlResponse + "<td>"+ sensorType +"</td><td><div id="+sensorId+" class=button onclick=deleteSensor(this)>Удалить датчик</div></td></tr>";
        cashData.remove(cashData.indexOf(" "),2);
        }
        s.replace("@@Sensors@@", htmlResponse);
        server.send(200, "text/html", s);
//      server.send(200, "text/plain", htmlResponse);
      }else{
        htmlResponse="<tr><td></td><td class=noSensors>Датчиков не найдено</td><td></td></tr>";
        s.replace("@@Sensors@@", htmlResponse);
        server.send(200, "text/html", s);
//        server.send(200, "text/plain", "timeout...");
        }
}

void handlePhone(){
  String s = PHONE_page;
  String data;
  Serial.print("getTelephone");
  long _timeout = millis() + 4000; 
  while(!Serial.available() && millis() < _timeout){yield();} 
   if (Serial.available()){
    data = Serial.readString();
    Serial.flush();
    String additionalPhone = data.substring(data.indexOf(' ')+1,data.length());
    data.remove(data.indexOf(' '));
    String mainPhone = data;
    s.replace("@@P1@@", mainPhone);
    s.replace("@@P2@@", additionalPhone);
    server.send(200, "text/html", s);  
    } else {
      server.send(200, "text/plain", "timeout...");
      }
 
      
//      else {
//        server.send(200, "text/plain", "Oops, an error, please, refresh the page");   
//        }
  }

 void deleteMainPhone(){
  Serial.print("deleteMainPhoneNumber");
  long _timeout = millis() + 4000; 
  while(millis() < _timeout){yield();} 
  handlePhone();
  }

 void deleteAdditionalPhone(){
  Serial.print("deleteAdditionalPhoneNumber");
  long _timeout = millis() + 4000; 
  while(millis() < _timeout){yield();} 
  handlePhone();
  }
 String checkSensorType(String type){
  if(type == "1"){
    return "Дверной звонок";
    }
  if (type == "2"){
    return "Дыма";
    }
  }

void addMainPhone(){
  String Phone = server.arg("mainPhoneNumber"); 
  String dataToSend = "addMainPhoneNumber "+Phone;
  Serial.print(dataToSend);
  long _timeout = millis() + 4000; 
  while(millis() < _timeout){yield();} 
  handlePhone();
  }
  void addAdditionalPhone(){
  String Phone = server.arg("additionalPhoneNumber"); 
  String dataToSend = "addAdditionalPhoneNumber "+Phone;
  Serial.print(dataToSend);
  long _timeout = millis() + 4000; 
  while(millis() < _timeout){yield();} 
  handlePhone();
  }

void deleteSensor(){
  String sensorId = server.arg("deleteSensor");
  String dataToSend = "deleteSensor "+sensorId;
  Serial.print(dataToSend);
  long _timeout = millis() + 4000; 
  while(millis() < _timeout){yield();} 
  handleRoot();
  }
