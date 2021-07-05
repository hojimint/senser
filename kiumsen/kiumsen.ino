// 참조유튜브 : https://youtu.be/JHngW6Gf4sU
// crc16 rtu를 사용하기 위해 다음 라이브러리 설치하세요.
// crc16 modbus
//  https://github.com/RobTillaart/CRC
//  AUTHOR: Rob Tillaart
#include <ArduinoJson.h>
#include <Ticker.h>
#include "CRC.h"
#include <SoftwareSerial.h>
Ticker ticker;
SoftwareSerial mySerial(D7, D4); // RX, TX

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

String inputString = "";         // 받은 문자열
int counter=0;

void tick()
{
  counter++;
  crd16Rtu();
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  ticker.attach(3, tick);  //0.1 초도 가능
  //ticker.detach();
}

void loop() {
  serialEvent();
}

void serialEvent() {
  if(mySerial.available() == false)
    return;
  while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    Serial.print(inChar);
    // add it to the inputString:
    inputString += inChar;
  }
  Serial.println("");
  if(inputString.length() >= 23) {
    
     Serial.println("");
     
    String tem= "온도 : ";
    String mo="습도 : ";
    String CO2= "CO2 : ";
    
    String a = inputString.substring(7,11);
    String b = inputString.substring(11,15);
    String c = inputString.substring(15,19);    

    String A = "0x" + a;
    String B = "0x" + b;
    String C = "0x" + c;

    float TEM = (A.toFloat()-200)/10;
    float M = (B.toFloat())/10;
    float CO = C.toFloat();

    String Z = tem+TEM;
    String E = mo+M;
    String L = CO2+CO;

    Serial.println(Z);
    Serial.println(E);
    Serial.println(L);
     inputString="";
    Serial.println("");

  } 
}

// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  char str[24] =  {0x3A,0x30,0x31,0x30,0x34,0x30,0x30,0x30,0x31,0x30,0x30,0x30,0x33,0x66,0x37,0x0D,0x0A};   
//  String x =  ":010400010003F7\r\n";
  String s;
  int len;

  for(int i=0;i<23;i++) {
    mySerial.print(str[i]);
    //Serial.println((int)str[i]);
  }
}
