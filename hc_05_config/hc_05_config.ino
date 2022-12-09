#include <SoftwareSerial.h>

/* ---- PINS bt -> arduino

  EN/key -> 7
  vcc    -> 5v (connect after turning on the arduino)
  gnd    -> gnd
  txd    -> 3
  rxd    -> 2
*/

SoftwareSerial mySerial(3, 2);  // RX, TX

/* ---- VARIOUS SERIAL COMMANDS
https://s3-sa-east-1.amazonaws.com/robocore-lojavirtual/709/HC-05_ATCommandSet.pdf
AT                             // check connection
AT+UART=Param1, Param2,Param3  // baud,stop = 0,parity = 0
AT+BAUD8                       // old ba7udrate, 8 = 115200
AT+RMAAD                       //clear paired
AT+ROLE=Param1                 // 0 slve, 1 master
*/


void setup() {
  // questo pin è connesso al relativo pin 34 (pin KEY) del HC-05
  pinMode(7, OUTPUT); 
  // che portato a HIGH permette di passare alla modalità AT
  digitalWrite(7, HIGH);
  
  mySerial.begin(38400);
  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  mySerial.println("AT");
}

void loop() {
  if (Serial.available()) {
    int i = 0;
    char at_cmd[100];
    while (Serial.available()) {
      at_cmd[i] = Serial.read();
      i++;
      delay(10);
    }
    at_cmd[i] = '\r';
    at_cmd[i + 1] = '\n';
    at_cmd[i + 2] = '\0';
    Serial.write("sending_cmd: ");
    Serial.write(at_cmd);
    mySerial.write(at_cmd);
  }

  if (mySerial.available()) {
    byte a = mySerial.read();
    Serial.write(a);
  }
}