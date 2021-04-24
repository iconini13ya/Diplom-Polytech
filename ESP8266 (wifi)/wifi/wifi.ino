#import "index.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
/* Put your SSID & Password */
const char* ssid = "MyArduino";  // Enter SSID here

 
ESP8266WebServer server(80);
 
void setup() {
  Serial.begin(9600);
  delay(100);
 
  Serial.println("Connecting to ");

 

  WiFi.softAP(ssid);
 

//  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
 
  server.begin();
  Serial.println("HTTP server started");

  server.on("/", handleRoot);
  server.on("/phoneSettings", handlePhone);
  server.on("/deleteMainPhone", deleteMainPhone);
  server.on("/deleteAdditionalPhone", deleteAdditionalPhone);
//  server.on("/mainPhoneNumber", !!!!);
//  server.on("/additionalPhoneNumber", !!!!!!);
}
void loop() {
  server.handleClient();
}

void handleRoot() {
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void handlePhone(){
  String s = PHONE_page;
  String data;
  Serial.print("getTelephone");
  long _timeout = millis() + 4000; 
  while(Serial.available() && millis() < _timeout){
    data = Serial.readString();
    } 
    if (data != 0){
     String additionalPhone = data.substring(data.indexOf(' ')+1,data.length());
     data.remove(data.indexOf(' '));
     String mainPhone = data;
     s.replace("@@P1@@", mainPhone);
     s.replace("@@P2@@", additionalPhone);
     server.send(200, "text/html", s);   
      } 
//      else {
//        server.send(200, "text/plain", "Oops, an error, please, refresh the page");   
//        }
  }

 void deleteMainPhone(){
  Serial.print("deleteMainPhoneNumber");
  long _timeout = millis() + 4000; 
  while(millis() < _timeout){} 
  handlePhone();
  }

 void deleteAdditionalPhone(){
  Serial.print("deleteAdditionalPhoneNumber");
  long _timeout = millis() + 4000; 
  while(millis() < _timeout){} 
  handlePhone();
  }
