/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

String str="";
byte flag = 0;

//    const uint16_t port = 80;
//    const char * host = "192.168.1.1"; // ip or dns
const uint16_t port = 5000;
const char * host = "10.10.141.69"; // ip or dns

// Serial.print("Connecting to ");
// Serial.println(host);

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("iot0", "iot00000");

    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);

    client.connect(host,port);
    delay(300);
    client.print("[4:PASSWD]\n");
}


void loop()
{
  int maxloops = 0;

  //wait for the server's reply to become available
  if(client.available())
  {
    while(client.available())
    {
      
      char myChar = (char)client.read();  //mySerial int 값을 char 형식으로 변환
      str+=myChar;   //수신되는 문자를 myString에 모두 붙임 (1바이트씩 전송되는 것을 연결)
      delay(5);           //수신 문자열 끊김 방지
    }
      if(!str.equals(""))
    {
      Serial.println(str);
      str="";
    }
    client.print(readBytesUntil("\n"));
  }
}