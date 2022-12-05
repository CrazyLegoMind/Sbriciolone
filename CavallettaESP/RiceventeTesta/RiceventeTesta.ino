const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'A';

const char eyesC = 'E';
const char tongueC = 'T';
const char antennasC = 'A';
const char mouthC = 'M';
const char jawsC = 'J';


struct ServoValues {
  int minValue;
  int maxValue;
  int channel;
  String servoName;
  bool mirror;
  int lastPosition = 512;
  int counterShutDown;
  bool stopAndGo;
  int shutDownWhen;
};



const byte howmanyservo = 9;
ServoValues servoList[howmanyservo];
#include <PololuMaestro.h>
unsigned long nextPalpebre = 0;
byte contatoreOcchi = 0;
const byte limiteOcchi = 8;
const int minOcchiValue = 995;
const int maxOcchiValue = 190;


const int rangeBoccaMediani = 300;


enum statiOcchi { APERTI,
                  INCHIUSURA,
                  MANUAL,
                  CHIUSMANUAL };

statiOcchi sitOcchi = MANUAL;

const int eyeLidsSpeed = 64;


const byte alive_trigger = 10;
bool alive_waiting = false;
unsigned long alive_ms = 0;

bool standby = false;
const char maestro_pin = 22;

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
HardwareSerial ESP32MaestroSerial(2);


MicroMaestro maestro(ESP32MaestroSerial, Maestro::noResetPin, 12, true);

void setup() {

  /* setTarget takes the channel number you want to control, and
    the target position in units of 1/4 microseconds. A typical
    RC hobby servo responds to pulses between 1 ms (4000) and 2
    ms (8000). */
  //Serial.begin(115200);

  ESP32MaestroSerial.begin(115200, SERIAL_8N1, -1, maestro_pin);  //rx - tx

  SerialBT.begin("MNK-Head2");
  Serial.begin(115200);

  servoList[0].minValue = 4992;
  servoList[0].maxValue = 8000;
  servoList[0].channel = 0;
  servoList[0].servoName = "AntennaR";
  servoList[0].mirror = false;
  servoList[0].stopAndGo = false;
  servoList[0].shutDownWhen = 1000;

  servoList[1].minValue = 4000;
  servoList[1].maxValue = 7232;
  servoList[1].channel = 1;
  servoList[1].servoName = "AntennaL";
  servoList[1].mirror = false;
  servoList[1].stopAndGo = false;
  servoList[1].shutDownWhen = 1000;

  servoList[2].minValue = 4000;
  servoList[2].maxValue = 8000;
  servoList[2].channel = 2;
  servoList[2].servoName = "LinguaYd";
  servoList[2].mirror = false;
  servoList[2].stopAndGo = false;
  servoList[2].shutDownWhen = 1000;

  servoList[3].minValue = 4000;
  servoList[3].maxValue = 8000;
  servoList[3].channel = 3;
  servoList[3].servoName = "LinguaXd";
  servoList[3].mirror = true;
  servoList[3].stopAndGo = false;
  servoList[3].shutDownWhen = 1000;

  servoList[4].minValue = 5376;
  servoList[4].maxValue = 8000;  //
  servoList[4].channel = 4;
  servoList[4].servoName = "LinguaZ";
  servoList[4].mirror = true;
  servoList[4].stopAndGo = false;
  servoList[4].shutDownWhen = 1000;

  servoList[5].minValue = 4000;
  servoList[5].maxValue = 4300;
  servoList[5].channel = 5;
  servoList[5].servoName = "LabbroSup";
  servoList[5].mirror = true;
  servoList[5].stopAndGo = false;
  servoList[5].shutDownWhen = 1000;

  servoList[6].minValue = 4800;
  servoList[6].maxValue = 8000;  //
  servoList[6].channel = 6;
  servoList[6].servoName = "LabbroInf";
  servoList[6].mirror = false;
  servoList[6].stopAndGo = false;
  servoList[6].shutDownWhen = 1000;

  servoList[7].minValue = 4000;
  servoList[7].maxValue = 5632;
  servoList[7].channel = 7;
  servoList[7].servoName = "MascellaR";
  servoList[7].mirror = false;
  servoList[7].stopAndGo = false;
  servoList[7].shutDownWhen = 1000;

  servoList[8].minValue = 6400;
  servoList[8].maxValue = 8000;
  servoList[8].channel = 8;
  servoList[8].servoName = "MascellaL";
  servoList[8].mirror = true;
  servoList[8].stopAndGo = false;
  servoList[8].shutDownWhen = 1000;


  for (int i = 0; i < howmanyservo; i++)
    maestro.setTarget(servoList[i].channel, 0);
}
String bt_msg;
bool bt_msg_ready;

void loop() {
    if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n') {
      if (bt_msg_ready) {

        bt_msg = String(incomingChar);
        bt_msg_ready = false;
      } else {
        bt_msg += String(incomingChar);
      }
    } else if(!bt_msg_ready) {
      bt_msg_ready = true;
      Serial.println(bt_msg);
    }
    //Serial.write(incomingChar);
  }
  if (bt_msg.length()>0 && bt_msg_ready) {

    bool doIt = true;
    if (bt_msg.charAt(0) == 'r') {
      bt_msg = bt_msg.substring(1);
    }
    if (doIt) {
      //delay(30);
      int lenghtMessage = getLenghtBeforeCheckSum(bt_msg, ';');
      int numberSeparators = homManySeparator(bt_msg, ';');
      int checksum = getValueStringSplitter(bt_msg, ';', numberSeparators).toInt();

      char bufferChar[lenghtMessage];
      bt_msg.toCharArray(bufferChar, lenghtMessage);

      int sum = 0;
      for (int i = 0; i < lenghtMessage; i++) {
        sum += bufferChar[i];
      }


      int myCheckSum = sum % 100;

      if (myCheckSum == checksum) {
        if (bt_msg.charAt(0) == eyesC) {
          eyesMotorMessage(bt_msg);

        } else if (bt_msg.charAt(0) == tongueC) {
          eyelidsMessage(bt_msg);

        } else if (bt_msg.charAt(0) == antennasC) {
          eyeBrowMessage(bt_msg);

        } else if (bt_msg.charAt(0) == jawsC) {
          noseMessage(bt_msg);

        } else if (bt_msg.charAt(0) == mouthC) {
          mouthMessage(bt_msg);
        } else if (bt_msg.charAt(0) == globalC) {
          //Serial.println("got global cmd");
          globalMessage(bt_msg);
        }
      }
    }
  }
  deadManButton();
  shutDownMotors();
  //delay();
}

void globalMessage(String message) {
  String indexString = getValueStringSplitter(message, ';', 1);
  int index = indexString.toInt();

  Serial.println("got global");
  switch (index) {
    case 0:
      delay(20);
      for (int i = 0; i < howmanyservo; i++) {
        delay(5);
        maestro.setTarget(servoList[i].channel, 0);
      }
      break;
  }
}

void noseMessage(String message) {
  if (message.charAt(1) == servoC) {

    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    servoList[index].lastPosition = value;
    servoList[index].counterShutDown = 0;
    maestro.setTarget(servoList[index].channel, analogServoConversion(value, servoList[index]));
  }
}

void eyelidsMessage(String message) {
  if (message.charAt(1) == servoC)
    palpebraMotorMessage(message);
}

void palpebraMotorMessage(String message) {
  String indexString = getValueStringSplitter(message, ';', 1);
  int index = indexString.toInt();
  String valueString = getValueStringSplitter(message, ';', 2);
  int value = valueString.toInt();
  servoList[index].lastPosition = value;
  servoList[index].counterShutDown = 0;
  maestro.setTarget(servoList[index].channel, analogServoConversion(value, servoList[index]));
}


void eyeBrowMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    servoList[index].lastPosition = value;
    servoList[index].counterShutDown = 0;
    maestro.setTarget(servoList[index].channel, analogServoConversion(value, servoList[index]));
  }
}


void mouthMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    servoList[index].lastPosition = value;
    servoList[index].counterShutDown = 0;
    maestro.setTarget(servoList[index].channel, analogServoConversion(value, servoList[index]));
    if (index == 12) {
      delay(2);
      maestro.setTarget(servoList[13].channel, analogServoConversion(1023 - value, servoList[13]));
    }
  }
}

void eyesMotorMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    servoList[index].lastPosition = value;
    servoList[index].counterShutDown = 0;
    maestro.setTarget(servoList[index].channel, analogServoConversion(value, servoList[index]));
  }
}


int analogServoConversion(int analogValue, ServoValues& servo) {
  if (servo.mirror)
    return map(analogValue, 0, 1023, servo.maxValue, servo.minValue);
  return map(analogValue, 0, 1023, servo.minValue, servo.maxValue);
}

int homManySeparator(String data, char separator) {
  int s = 0;
  for (int i = 0; i < data.length(); i++)
    if (data.charAt(i) == separator)
      s++;
  return s;
}


int getLenghtBeforeCheckSum(String data, char separator) {
  int l = -1;

  for (int i = 0; i < data.length(); i++)
    if (data.charAt(i) == separator)
      l = i;
  return (l + 1);
}


String getValueStringSplitter(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void shutDownMotors() {
  for (int i = 0; i < howmanyservo; i++) {
    if (!servoList[i].stopAndGo)
      continue;
    if (servoList[i].counterShutDown >= servoList[i].shutDownWhen) {
      maestro.setTarget(servoList[i].channel, 0);
      servoList[i].counterShutDown = servoList[i].shutDownWhen;
    } else {
      servoList[i].counterShutDown = servoList[i].counterShutDown + 1;
    }
  }
}


void deadManButton() {
  if (!alive_waiting) {
    alive_ms = millis();
    alive_waiting = true;
  } else {
    if (millis() - alive_ms > alive_trigger * 100) {
      SerialBT.println("ALIVE");
      alive_waiting = false;
    }
  }
}
