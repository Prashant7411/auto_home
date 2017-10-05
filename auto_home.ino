#include <SoftwareSerial.h>
int data;
int data1;
boolean lock=true;

int counter=0;
int Light_status=10;
SoftwareSerial esp8266(3, 4); //set the software serial pins RX pin = 3, TX pin = 4 
//definition of variables
#define DEBUG true //show messages between ESP8266 and Arduino in serial port, when set to true
#define SSID "Sales" //replace x with your wifi network name
#define PASS "Askme@123" //replace x with your wifi network password


String sendAT(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

void setup() {
  esp8266.begin(9600);// begin the software serial communication with baud rate 9600
  Serial.begin(9600); 
  sendAT("AT+RST\r\n", 2000, DEBUG); // call sendAT function to send reset AT command
  sendAT("AT\r\n", 1000, DEBUG);
  sendAT("AT+CWMODE=1\r\n", 1000, DEBUG); //call sendAT function to set ESP8266 to station mode
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n", 2000, DEBUG); //AT command to connect wit the wifi network

  while(!esp8266.find("OK")) { //wait for connection
  } 
  sendAT("AT+CIFSR\r\n", 1000, DEBUG); //AT command to print IP address on serial monitor
  sendAT("AT+CIPMUX=0\r\n", 1000, DEBUG); //AT command to set ESP8266 to multiple connections
  //pinMode(ledPin, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11,INPUT);
  pinMode(8,OUTPUT);
 pinMode(13,OUTPUT);
 pinMode(12,OUTPUT);
  //digitalWrite(ledPin, LOW);
  Serial.print("calibrating sensor ");
    for(int i = 0; i < 30; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    digitalWrite(13,HIGH);
    digitalWrite(12,HIGH);
    delay(2000);
    digitalWrite(12,LOW);
    delay(50);

    
  
  
}

void loop() {
  data = digitalRead(10);
  data1 = digitalRead(11);

if(data==HIGH&&lock==true){
  lock=false;
  digitalWrite(12,HIGH);
  delay(500);
   digitalWrite(12,LOW);
  Serial.println("hello 3");
  
  counter++;
  //Serial.print("person: ");
  //Serial.println(counter);
  
  if(counter>0){
    digitalWrite(8,HIGH);
    Light_status=100;
    passTS(counter,Light_status);
  }
   
  delay(4000);
   lock=true;  
}

if(data1==HIGH&&lock==true){
  lock=false;
  digitalWrite(12,HIGH);
  delay(500);
   digitalWrite(12,LOW);
  Serial.println("hello 4");
  
  counter--;
  
 // Serial.print("person: ");
  //Serial.println(counter);
  if(counter==0){
    digitalWrite(8,LOW);
    Light_status=10;
    
  }
  passTS(counter,Light_status);
  delay(4000);
lock=true;
  
}


}

void passTS(int c,int l){
  String counter1=String(c); //convert interger to string data type
  Serial.print("No of persons: "); //print soil moisture on serial monitor
  Serial.println(c); //print soil moisture value on serial monitor

  String Light_status1=String(l); //convert interger to string data type
  Serial.print("Light status: "); //print soil moisture on serial monitor
  Serial.println(l); //print soil moisture value on serial monitor
  updateTS(counter1,Light_status1); //call the function to update ThingSpeak channel
  delay(3000);
}

void updateTS(String C,String L){
  Serial.println("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", 1000, DEBUG);    
  delay(2000);
  String cmdlen;
  String cmd="GET /update?key=4VUEWVLH4WQ9JRV8&field1="+C+"&field2="+L+"\r\n"; // update the temprature and humidity values on thingspeak url,replace xxxxxxx with your write api key
   cmdlen = cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n", 2000, DEBUG);
   esp8266.print(cmd);
   Serial.println("");
   sendAT("AT+CIPCLOSE\r\n", 2000, DEBUG);
   Serial.println("");
   delay(1000);
   }


