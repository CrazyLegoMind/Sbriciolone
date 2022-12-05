
#include <HardwareSerial.h>

HardwareSerial SerialBT(1);

void setup() {
  SerialBT.begin(9600, SERIAL_8N1, 16, 17);  //rx - tx
  Serial.begin(115200);
  digitalWrite(32,HIGH);
  delay(100);
  // AT+NAMEXXX where XXX is the new name
  //SerialBT.write("AT+NAME=Mouth\n\r");


  // AT+BAUDX where X from 1 to 8
  // 1 -> 1200 Bauds
  // 2 -> 2400 Bauds
  // 3 -> 4800 Bauds
  // 4 -> 9600 Bauds
  // 5 -> 19200 Bauds
  // 6 -> 38400 Bauds
  // 7 -> 57600 Bauds
  // 8 -> 115200 Bauds
  //Serial.print("AT+BAUD8\n\r");
}

void loop() {
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  if (Serial.available()) {
    char c = Serial.read();
     Serial.write(c);
    SerialBT.write(c);
  }
}
