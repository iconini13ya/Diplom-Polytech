
void setup() {
  pinMode(4,OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
}
int bustatus;
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    bustatus=Serial.parseInt();
    Serial.print(bustatus);
    switch(bustatus){
      case 0:
        Serial.print("Я на лоу");
        digitalWrite(4,LOW);
        break;
      case 1:
        Serial.print("Я на хай");
        digitalWrite(4,HIGH);
        break;
      }
    }
}
