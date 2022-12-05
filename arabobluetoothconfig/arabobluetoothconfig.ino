#include <HardwareSerial.h>

HardwareSerial mySerial(1);

void setup() {
  // put your setup code here, to run once:
  pinMode(32, OUTPUT);  // questo pin è connesso al relativo pin 34 (pin KEY) del HC-05
  // che portato a HIGH permette di passare alla modalità AT
  digitalWrite(32, HIGH);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);  //rx - tx
  Serial.begin(9600);
  /*
  sendCommand("AT");
  sendCommand("AT+RMAAD");
  sendCommand("AT+ROLE0");
  sendCommand("AT+ADDR?");
  sendCommand("AT+UART=115200,0,0");
*/
  Serial.println("Enter AT commands:");
  mySerial.println("AT");
}

void loop() {
  if (Serial.available()) {
    int i = 0;
    char at_cmd[100];
    while(Serial.available()){
      at_cmd[i] = Serial.read();
      i++;
      delay(10);
    }
    at_cmd[i] = '\r';
    at_cmd[i+1] = '\n';
    at_cmd[i+2] = '\0';
    Serial.write("sending_cmd: ");
    Serial.write(at_cmd);
    mySerial.write(at_cmd);
  }

  if (mySerial.available()) {
    byte a = mySerial.read();
    Serial.write(a);
  }
}