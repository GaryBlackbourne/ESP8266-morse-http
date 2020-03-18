#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

String message = "ha5kfu";
String code;
const int output = LED_BUILTIN;

//morse codes:
String lettercode[] = {
  ".-",   //A
  "-...", //B
  "-.-.",  //C
  "-..",   //D
  ".",     //E
  "..-.",  //F
  "--.",   //G
  "....",  //H
  "..",    //I
  ".---",  //J
  "-.-",   //K
  ".-..",  //L
  "--",    //M
  "-.",    //N
  "---",   //O
  ".--.",  //P
  "--.-",  //Q
  ".-.",   //R
  "...",   //S
  "-",     //T
  "..-",   //U
  "...-",  //V
  ".--",   //W
  "-..-",  //X
  "-.--",  //Y
  "--.."  //Z
};

String numcode[] = {
  "-----",  //0
  ".----",  //1
  "..---",  //2
  "...--",  //3
  "....-",  //4
  ".....",  //5
  "-....",  //6
  "--...",  //7
  "---..",  //8
  "----."   //9
};

void beep(){
  digitalWrite(output, 0);
  delay(300);
  digitalWrite(output, 1);
  delay(100);
  return;
}

void bip(){
  digitalWrite(output, 0);
  delay(100);
  digitalWrite(output, 1);
  delay(100);
  return;
}

void space(){
  delay(400);
  return;
}

//case insensitive message conversion:
String chartomorse(char input){
  if(input >= 'a' && input <= 'z'){
    return lettercode[input-'a'];
  }else if(input >= 'A' && input <= 'Z'){
    return lettercode[input-'A'];
  }else if('0' <= input && input <= '9'){
    return numcode[input-'0'];
  }else if(input == '\n' || input == ' '){
    return "spc";
  }else{
    return "err";
  }
}

void sendMorse(){
      for(int i = 0; i < message.length(); i++){

        code = chartomorse(message[i]);


        //todo: fix stupid empty serial print at the end
        Serial.print("'");
        Serial.print(code);
        Serial.print("'");
        Serial.print(" --> ");
        Serial.print(message[i]);
        Serial.println("");


        if(code != "err"){
          for(int j = 0; j < code.length(); j++){
            if(code[j] == '.'){
              bip();
            }
            if(code[j] == '-'){
              beep();
            }
            if(code == "spc"){
              space();
            }
        }
        delay(300); // gap between letters(space: 700 = 400 + 300)
      }
    }
}


ESP8266WebServer server(80);

String ssid = "Hyperion";
String psswd = "13243546";

void handleRoot(){
  server.send(200,"text/plain", "Hello there!");  //General Kenobi..!
}

void handleMorse(){
  // get the message to be transmitted
  if(server.arg("message")==""){
    server.send(200,"text/plain","No message detected");
  }else{
    message = server.arg("message");
    String httpmessage = "Message is beeing sent: ";
    httpmessage += message;
    server.send(200,"text/plain",httpmessage);
  }
  sendMorse();
}



void setup() {
    pinMode(output, OUTPUT);
    digitalWrite(output, 1); //set initial transmitter state to 0 (in cases of built in led, 0 is OFF)
    Serial.begin(9600);

    WiFi.begin(ssid,psswd);
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.println("Waiting to connect...");
    }
    Serial.println("Connected!");

    Serial.println(WiFi.localIP()); // local ip for ESP

    server.on("/", handleRoot);
    server.on("/morse", handleMorse);

    server.begin();
}



void loop() {

    // if there is input, read new message:
    // while(Serial.available()>0){
    //   message = Serial.readString();
    // }

    server.handleClient();
}
