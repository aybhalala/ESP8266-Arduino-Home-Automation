/*
============================================================================
Created by Aditya Patel
Email: me@pateladitya.com
================================================================================
*/
#include <Metro.h>

#define DEBUG true
int Flame = HIGH;
int signal = LOW;
int pirState = LOW;
String pir = "No";
float current=0;
const unsigned long sampleTime = 10000UL;
const unsigned long numSamples = 250UL;
const unsigned long sampleInterval = sampleTime/numSamples;
const int adc_zero = 516;

String server = "your server url"; // www.example.com, this location should contain the esp.php file
String uri = "/esp.php"; //if your server url does not contain esp.php file, edit the path accordingly
String data;


Metro metro0 = Metro(250); 
Metro metro1 = Metro(10000); 
Metro metro2 = Metro(5000); 
Metro metro3 = Metro(10000); 
Metro metro4 = Metro(15000); 


void setup() 
{
  Serial.begin(9600);   // Setting the baudrate to 9600
  Serial1.begin(9600);  // Set it according to your esp’s baudrate. Different esp’s have different baud rates.
  pinMode(10,OUTPUT);   // Setting the pin 10 as the output pin.
  digitalWrite(10,LOW); // Making it low.
  pinMode(11,OUTPUT);   // Setting the pin 11 as the output pin.
  digitalWrite(11,LOW); // Making it low.
  pinMode(12,OUTPUT);   // Setting the pin 12 as the output pin..
  digitalWrite(12,LOW); // Making pin 12 low.
  pinMode(13,OUTPUT);   // Setting the pin 13 as the output pin.
  digitalWrite(13,LOW); // Making pin 13 low.     
  pinMode(8,INPUT); //Fire sensor Input
  pinMode(9,OUTPUT); //Buzzer Pin
  pinMode(7,INPUT); //PIR Input Pin
  pinMode(6,OUTPUT);
  sendData("AT\r\n",2000,DEBUG);
  sendData("AT+RST\r\n",2000,DEBUG);            
  sendData("AT+CWMODE=1\r\n",1000,DEBUG);      
  sendData("AT+CWJAP=\"ssid\",\"password\"\r\n",10000,DEBUG); //replace the wifi ssid and password with your data.
  delay(10000);
  sendData("AT+CIFSR\r\n",1000,DEBUG);          // This will get ip address and will show it, put it in your html file
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG);       
  sendData("AT+CIPSERVER=1,343\r\n",1000,DEBUG); // This will set the server on port 343, you can keep it anything
}
void loop() 
{
  if (metro0.check() == 1) {
  switchingControl();
  }
  if (metro1.check() == 1) {
  fireSense();
  }
  if (metro2.check() == 1) {
  pirSense();
  }
  if (metro3.check() == 1) {
  CurrentSense();
  }
  if (metro4.check() == 1) {
  httppost();
  }
  
}
String sendData(String command, const int timeout, boolean debug) // Function to send the data to the Serial1
{
  String response = ""; 
  Serial1.print(command);          
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(Serial1.available())   
    {
      char c = Serial1.read();      
      response+=c;                  
    }
  }
  if(debug)
  { 
    Serial.print(response);       
  }
  return response;
}
void httppost(){
  
  data = "current=" + String(current) + "&human=" + pir;
  Serial1.println("AT+CIPSTART=3,\"TCP\",\"" + server + "\",80\r\n");//start a TCP connection.

if( Serial1.find("OK")) {

Serial.println("TCP connection ready");

} delay(1000);

String postRequest =

"POST " + uri + " HTTP/1.0\r\n" +

"Host: " + server + "\r\n" +

"Accept: *" + "/" + "*\r\n" +

"Content-Length: " + data.length() + "\r\n" +

"Content-Type: application/x-www-form-urlencoded\r\n" +

"\r\n" + data + "\r\n";

String sendCmd = "AT+CIPSEND=3,";//determine the number of caracters to be sent.
sendCmd +=String(postRequest.length());
sendCmd += "\r\n";
Serial1.println(sendCmd);


delay(500);

if(Serial1.find(">")) { Serial.println("Sending.."); 
Serial1.print(postRequest);

if( Serial1.find("SEND OK")) { Serial.println("Packet sent");

while (Serial1.available()) {

String tmpRSerial1 = Serial1.readString();

Serial.println(tmpRSerial1);

}

// close the connection

Serial1.println("AT+CIPCLOSE=3\r\n");

}

}
}

void CurrentSense()
{
  unsigned long currentAcc = 0;
  unsigned int count = 0;
  unsigned long prevMicros = micros() - sampleInterval;
  while (count < numSamples)
  {
    if (micros() - prevMicros >= sampleInterval)
    {
      long adc_raw = analogRead( A0 ) - adc_zero;
      currentAcc += (unsigned long)(adc_raw * adc_raw);
      ++count;
      prevMicros += sampleInterval;
    }
  }  

float rms = (sqrt((float)currentAcc/(float)numSamples) * (50 / 1024.0));
rms=rms-0.10;
if (rms<0.1)
{
 rms=0; 
}  

current=rms;

}
void switchingControl()
{
  if(Serial1.available()) 
  { 
    if(Serial1.find("+IPD,"))
    { 
      delay(1000);      
      int connectionId = Serial1.read()-48;   
      Serial1.find("pin=");                   
      int pinNumber = (Serial1.read()-48)*10; 
      pinNumber += (Serial1.read()-48);    
      Serial.println(pinNumber);
    switch (pinNumber) {
    case 11:
    digitalWrite(10, LOW);
    break;
    case 12:
    digitalWrite(10, HIGH);
    break;
    case 13:
    digitalWrite(11, LOW);
    break;
    case 14:
    digitalWrite(11, HIGH);
    break;
    case 15:
    digitalWrite(12, LOW);
    break;
    case 16:
    digitalWrite(12, HIGH);
    break;
    case 17:
    digitalWrite(13, LOW);
    break;
    case 18:
    digitalWrite(13, HIGH);
    break;
    case 19:
    digitalWrite(6, LOW);
    break;
    case 20:
    digitalWrite(6, HIGH);
    break;

    }
      
      // The following commands will close the connection 
      String closeCommand = "AT+CIPCLOSE="; 
      closeCommand+=connectionId; 
      closeCommand+="\r\n";
      sendData(closeCommand,1000,DEBUG);     // Sending the data to the Serial1 to close the command
    } 
  }
}

void fireSense()
{
  Flame = digitalRead(8);
 if (Flame== LOW)
 {
 Serial.println("HIGH FLAME");
 digitalWrite(9, HIGH);
 
 }
 else
 {
 digitalWrite(9, LOW);
 }
}

void pirSense()
{
  signal = digitalRead(7);  // read input value
if(digitalRead(6) == HIGH){
  if (signal == HIGH) {            // check if the input is HIGH
    digitalWrite(9, HIGH);  // turn Buzzer ON   
    Serial.println("Motion");
    if (pirState == LOW) {     // we have just turned on   
      Serial.println("Motion detected!");
      pirState = HIGH;
      pir = "Yes";
    }
  } else {
    digitalWrite(9, LOW); // turn Buzzer OFF
    Serial.println("No Motion");
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      pirState = LOW;
      pir = "No";
  }
  }
}
}

