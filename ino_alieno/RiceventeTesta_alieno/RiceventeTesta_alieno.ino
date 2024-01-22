const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'G';

const char earC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char snoutC = 'T';


struct ServoValues {
  bool reverse;  //reverse min and max control
  bool stopAndGo;
  int minValue = 4000;  //min micros*4
  int maxValue = 8000;  //max micros*4
  int channel;          //channel on the pololu maestro
  int lastPosition = 512;
  int counterShutDown;
  int shutDownWhen;
};



const byte howmanyservo = 16;
ServoValues servoList[howmanyservo];
#include <PololuMaestro.h>


int aliveCounter = 0;
const byte aliveTrigger = 10;
bool standby = false;
/* On boards with a hardware serial port available for use, use
  that port to communicate with the Maestro. For other boards,
  create a SoftwareSerial object using pin 10 to receive (RX) and
  pin 11 to transmit (TX). */
#ifdef SERIAL_PORT_HARDWARE_OPEN
#define maestroSerial SERIAL_PORT_HARDWARE_OPEN
#else
#include <SoftwareSerial.h>
SoftwareSerial maestroSerial(10, 11);
#endif

/* Next, create a Maestro object using the serial port.
  Uncomment one of MicroMaestro or MiniMaestro below depending
  on which one you have. */
//MicroMaestro maestro(maestroSerial);
MiniMaestro maestro(maestroSerial);

void setup() {

  /* setTarget takes the channel number you want to control, and
    the target position in units of 1/4 microseconds. A typical
    RC hobby servo responds to pulses between 1 ms (4000) and 2
    ms (8000). */
  //Serial.begin(115200);
  Serial3.begin(115200);
  maestroSerial.begin(115200);
  Serial3.setTimeout(20);


  //NOME SERVO "Lab_D";
  servoList[0].minValue = 4288;
  servoList[0].maxValue = 6528;
  servoList[0].channel = 0;
  servoList[0].reverse = false;
  servoList[0].stopAndGo = false;
  servoList[0].shutDownWhen = 1000;

  //NOME SERVO "Naso";
  servoList[1].minValue = 3968;
  servoList[1].maxValue = 7168;
  servoList[1].channel = 1;
  servoList[1].reverse = false;
  servoList[1].stopAndGo = false;
  servoList[1].shutDownWhen = 1000;

  //NOME SERVO "Cig_C_Sx";
  servoList[2].minValue = 3968;
  servoList[2].maxValue = 6400;
  servoList[2].channel = 2;
  servoList[2].reverse = false;
  servoList[2].stopAndGo = false;
  servoList[2].shutDownWhen = 1000;

  //NOME SERVO "Cig_C_Dx";
  servoList[3].minValue = 4864;
  servoList[3].maxValue = 8000;
  servoList[3].channel = 3;
  servoList[3].reverse = true;
  servoList[3].stopAndGo = false;
  servoList[3].shutDownWhen = 1000;

  //NOME SERVO "Orecchie";
  servoList[4].minValue = 4672;
  servoList[4].maxValue = 8000;  //
  servoList[4].channel = 4;
  servoList[4].reverse = false;
  servoList[4].stopAndGo = false;
  servoList[4].shutDownWhen = 1000;

  //NOME SERVO "Lab_S";
  servoList[5].minValue = 5632;
  servoList[5].maxValue = 8000;
  servoList[5].channel = 5;
  servoList[5].reverse = true;
  servoList[5].stopAndGo = false;
  servoList[5].shutDownWhen = 1000;

  //NOME SERVO "Baf_S";
  servoList[6].minValue = 5184;
  servoList[6].maxValue = 8000;  //
  servoList[6].channel = 6;
  servoList[6].reverse = false;
  servoList[6].stopAndGo = false;
  servoList[6].shutDownWhen = 1000;

  //NOME SERVO "Cig_E_Sx";
  servoList[7].minValue = 3968;
  servoList[7].maxValue = 6080;
  servoList[7].channel = 7;
  servoList[7].reverse = false;
  servoList[7].stopAndGo = false;
  servoList[7].shutDownWhen = 1000;

  //NOME SERVO "Cig_E_Dx";
  servoList[8].minValue = 5632;
  servoList[8].maxValue = 8000;
  servoList[8].channel = 8;
  servoList[8].reverse = true;
  servoList[8].stopAndGo = false;
  servoList[8].shutDownWhen = 1000;

  //NOME SERVO "Baf_D";
  servoList[9].minValue = 6912;
  servoList[9].maxValue = 8000;
  servoList[9].channel = 9;
  servoList[9].reverse = true;
  servoList[9].stopAndGo = false;
  servoList[9].shutDownWhen = 1000;

  //NOME SERVO "Gua_S";
  servoList[10].minValue = 3968;
  servoList[10].maxValue = 7040;
  servoList[10].channel = 10;
  servoList[10].reverse = false;
  servoList[10].stopAndGo = false;
  servoList[10].shutDownWhen = 1000;

  //NOME SERVO "Gua_D";
  servoList[11].minValue = 5120;
  servoList[11].maxValue = 8000;
  servoList[11].channel = 11;
  servoList[11].reverse = true;
  servoList[11].stopAndGo = false;
  servoList[11].shutDownWhen = 1000;

  //NOME SERVO "Occ_D";
  servoList[12].minValue = 4608;
  servoList[12].maxValue = 5824;
  servoList[12].channel = 12;
  servoList[12].reverse = true;
  servoList[12].stopAndGo = false;
  servoList[12].shutDownWhen = 1000;

  //NOME SERVO "Occ_S";
  servoList[13].minValue = 6208;
  servoList[13].maxValue = 7360;
  servoList[13].channel = 13;
  servoList[13].reverse = false;
  servoList[13].stopAndGo = false;
  servoList[13].shutDownWhen = 1000;

  //NOME SERVO "Bocc_D";
  servoList[14].minValue = 3968;
  servoList[14].maxValue = 8000;
  servoList[14].channel = 14;
  servoList[14].reverse = true;
  servoList[14].stopAndGo = false;
  servoList[14].shutDownWhen = 1000;

  //NOME SERVO "Bocc_S";
  servoList[15].minValue = 3968;
  servoList[15].maxValue = 8000;
  servoList[15].channel = 15;
  servoList[15].reverse = false;
  servoList[15].stopAndGo = false;
  servoList[15].shutDownWhen = 1000;

  for (int i = 0; i < howmanyservo; i++)
    maestro.setTarget(servoList[i].channel, 0);
}


void loop() {
  String message = Serial3.readStringUntil('\n');
  //if(message.length() > 0)
  
  if (message.length() > 0) {

    bool doIt = true;
    if (message.charAt(0) != 'r') {
      ;
    } else {
      message = message.substring(1);
    }
    if (doIt) {

      //delay(30);
      int lenghtMessage = getLenghtBeforeCheckSum(message, ';');
      int numberSeparators = homManySeparator(message, ';');
      int checksum = getValueStringSplitter(message, ';', numberSeparators).toInt();

      char bufferChar[lenghtMessage];
      message.toCharArray(bufferChar, lenghtMessage);

      int sum = 0;
      for (int i = 0; i < lenghtMessage; i++) {
        sum += bufferChar[i];
      }

      int myCheckSum = sum % 100;
      if (myCheckSum == checksum) {
        char sector_c = message.charAt(0);
        switch (sector_c) {
          case earC:
            earMotorMessage(message);
            break;
          case eyeLidsC:
            eyelidsMessage(message);
            break;
          case eyebrownsC:
            eyeBrowMessage(message);
            break;
          case snoutC:
            noseMessage(message);
            break;
          case mouthC:
            mouthMessage(message);
            break;
          case globalC:
            globalMessage(message);
            break;
        }
      }
    }
  }
  deadManButton();
}

void globalMessage(String message) {
  String indexString = getValueStringSplitter(message, ';', 1);
  int index = indexString.toInt();
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
    maestro.setTargetMiniSSC(servoList[index].channel, analogServoConversion(value, servoList[index]));
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
  maestro.setTargetMiniSSC(servoList[index].channel, analogServoConversion(value, servoList[index]));
}


void eyeBrowMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    servoList[index].lastPosition = value;
    servoList[index].counterShutDown = 0;
    maestro.setTargetMiniSSC(servoList[index].channel, analogServoConversion(value, servoList[index]));
  }
}


void mouthMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    if(index == 15){
      return;
    }
    servoList[index].lastPosition = value;
    servoList[index].counterShutDown = 0;
    maestro.setTargetMiniSSC(servoList[index].channel, analogServoConversion(value, servoList[index]));
    if (index == 14) {
      delay(2);
      maestro.setTargetMiniSSC(servoList[15].channel, analogServoConversion(value, servoList[15]));
    }
  }
}

void earMotorMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    servoList[index].lastPosition = value;
    servoList[index].counterShutDown = 0;
    maestro.setTargetMiniSSC(servoList[index].channel, analogServoConversion(value, servoList[index]));
  }
}

int analogServoConversion(int analogValue, ServoValues& servo) {
  if (servo.reverse)
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

void deadManButton() {
  if (aliveCounter % aliveTrigger == 0)
    Serial3.println("ALIVE");

  aliveCounter++;
}
