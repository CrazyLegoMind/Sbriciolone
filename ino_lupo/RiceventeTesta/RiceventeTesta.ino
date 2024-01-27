const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'A';

const char eyesC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char noseC = 'N';
const char tailC = 'T';


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



const byte howmanyservo = 19;
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

  servoList[0].minValue = 4032;
  servoList[0].maxValue = 6208;
  servoList[0].channel = 0;
  servoList[0].servoName = "LabSupSX";
  servoList[0].mirror = false;
  servoList[0].stopAndGo = false;
  servoList[0].shutDownWhen = 1000;

  servoList[1].minValue = 4032;
  servoList[1].maxValue = 6592;
  servoList[1].channel = 1;
  servoList[1].servoName = "Naso";
  servoList[1].mirror = false;
  servoList[1].stopAndGo = false;
  servoList[1].shutDownWhen = 1000;

  servoList[2].minValue = 4032;
  servoList[2].maxValue = 6400;
  servoList[2].channel = 2;
  servoList[2].servoName = "LabSupDX";
  servoList[2].mirror = false;
  servoList[2].stopAndGo = false;
  servoList[2].shutDownWhen = 1000;

  servoList[3].minValue = 6784;
  servoList[3].maxValue = 8000;
  servoList[3].channel = 3;
  servoList[3].servoName = "LabInfSX";
  servoList[3].mirror = true;
  servoList[3].stopAndGo = false;
  servoList[3].shutDownWhen = 1000;

  servoList[4].minValue = 4032;
  servoList[4].maxValue = 6848;  //
  servoList[4].channel = 4;
  servoList[4].servoName = "Orecchie";
  servoList[4].mirror = true;
  servoList[4].stopAndGo = false;
  servoList[4].shutDownWhen = 1000;

  servoList[5].minValue = 4416;
  servoList[5].maxValue = 8000;
  servoList[5].channel = 5;
  servoList[5].servoName = "LabInfDX";
  servoList[5].mirror = false;
  servoList[5].stopAndGo = false;
  servoList[5].shutDownWhen = 1000;

  servoList[6].minValue = 4032;
  servoList[6].maxValue = 6784;  //
  servoList[6].channel = 6;
  servoList[6].servoName = "SopracSS";
  servoList[6].mirror = false;
  servoList[6].stopAndGo = false;
  servoList[6].shutDownWhen = 1000;

  servoList[7].minValue = 4032;
  servoList[7].maxValue = 6400;
  servoList[7].channel = 7;
  servoList[7].servoName = "SopracSC";
  servoList[7].mirror = false;
  servoList[7].stopAndGo = false;
  servoList[7].shutDownWhen = 1000;

  servoList[8].minValue = 5824;
  servoList[8].maxValue = 8000;
  servoList[8].channel = 8;
  servoList[8].servoName = "SopracDC";
  servoList[8].mirror = true;
  servoList[8].stopAndGo = false;
  servoList[8].shutDownWhen = 1000;

  servoList[9].minValue = 5600;
  servoList[9].maxValue = 8000;
  servoList[9].channel = 9;
  servoList[9].servoName = "SopracDD";
  servoList[9].mirror = true;
  servoList[9].stopAndGo = false;
  servoList[9].shutDownWhen = 1000;

  servoList[10].minValue = 4992;
  servoList[10].maxValue = 6208;
  servoList[10].channel = 10;
  servoList[10].servoName = "BorsaS";
  servoList[10].mirror = false;
  servoList[10].stopAndGo = false;
  servoList[10].shutDownWhen = 1000;

  servoList[11].minValue = 5184;
  servoList[11].maxValue = 8000;
  servoList[11].channel = 11;
  servoList[11].servoName = "BorsaD";
  servoList[11].mirror = true;
  servoList[11].stopAndGo = false;
  servoList[11].shutDownWhen = 1000;

  servoList[12].minValue = 3712;
  servoList[12].maxValue = 6656;
  servoList[12].channel = 12;
  servoList[12].servoName = "MandibolaSX";
  servoList[12].mirror = false;
  servoList[12].stopAndGo = false;
  servoList[12].shutDownWhen = 1000;

  servoList[13].minValue = 4032;
  servoList[13].maxValue = 7232;
  servoList[13].channel = 13;
  servoList[13].servoName = "MandibolaDX";
  servoList[13].mirror = false;
  servoList[13].stopAndGo = false;
  servoList[13].shutDownWhen = 1000;

  servoList[14].minValue = 4224;
  servoList[14].maxValue = 7616;
  servoList[14].channel = 14;
  servoList[14].servoName = "OcchiY";
  servoList[14].mirror = true;
  servoList[14].stopAndGo = false;
  servoList[14].shutDownWhen = 1000;

  servoList[15].minValue = 5200;
  servoList[15].maxValue = 6650;
  servoList[15].channel = 15;
  servoList[15].servoName = "OcchioSX";
  servoList[15].mirror = true;
  servoList[15].stopAndGo = false;
  servoList[15].shutDownWhen = 1000;

  servoList[16].minValue = 4800;
  servoList[16].maxValue = 6400;
  servoList[16].channel = 16;
  servoList[16].servoName = "OcchioDX";
  servoList[16].mirror = true;
  servoList[16].stopAndGo = false;
  servoList[16].shutDownWhen = 1000;

  servoList[17].minValue = 4800;
  servoList[17].maxValue = 6400;
  servoList[17].channel = 17;
  servoList[17].servoName = "PalpebreSup";
  servoList[17].mirror = true;
  servoList[17].stopAndGo = false;
  servoList[17].shutDownWhen = 1000;

  servoList[18].minValue = 4800;
  servoList[18].maxValue = 5632;
  servoList[18].channel = 18;
  servoList[18].servoName = "PalpebreInf";
  servoList[18].mirror = true;
  servoList[18].stopAndGo = false;
  servoList[18].shutDownWhen = 1000;

  for (int i = 0; i < howmanyservo; i++)
    maestro.setTarget(servoList[i].channel,0);
}


void loop() {
  String message = Serial3.readStringUntil('\n');
  if (message.length() > 0) {

    bool doIt = true;
    if (message.charAt(0) != 'r') {

      long randNumber = random(100);

      if (randNumber < 20) {
        doIt = false;
      }
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
        if (message.charAt(0) == eyesC) {
          eyesMotorMessage(message);

        } else if (message.charAt(0) == eyeLidsC) {
          eyelidsMessage(message);

        } else if (message.charAt(0) == eyebrownsC) {
          eyeBrowMessage(message);

        } else if (message.charAt(0) == noseC) {
          noseMessage(message);

        } else if (message.charAt(0) == mouthC) {
          mouthMessage(message);
        }
        else if (message.charAt(0) == globalC) {
          //Serial.println("got global cmd");
          globalMessage(message);
        }
      }
    }
  }
  gestisciOcchi();
  deadManButton();
  shutDownMotors();
  //delay(20);
}

void globalMessage(String message){
  String indexString = getValueStringSplitter(message, ';', 1);
  int index = indexString.toInt();
  switch(index){
    case 0:
      delay(20);
      for (int i = 0; i < howmanyservo; i++){
        delay(5);
        maestro.setTarget(servoList[i].channel,0);
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
    if(index == 12){
      delay(2);
      maestro.setTarget(servoList[13].channel, analogServoConversion(1023-value, servoList[13]));
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





void gestisciOcchi() {
  if (sitOcchi == MANUAL) {
    maestro.setSpeed(servoList[15].channel, eyeLidsSpeed);
    maestro.setSpeed(servoList[18].channel, eyeLidsSpeed);
    return;
  }

  if (sitOcchi == APERTI) {
    maestro.setSpeed(servoList[15].channel, eyeLidsSpeed);
    maestro.setSpeed(servoList[18].channel, eyeLidsSpeed);
    if (millis() > nextPalpebre) {
      nextPalpebre = random(2000, 10000) + millis();
      sitOcchi = INCHIUSURA;
      contatoreOcchi = 0;
      maestro.setSpeed(servoList[15].channel, 0);
      maestro.setSpeed(servoList[18].channel, 0);
      maestro.setAcceleration(servoList[15].channel, 0);
      maestro.setAcceleration(servoList[18].channel, 0);
      servoList[15].counterShutDown = 0;
      servoList[18].counterShutDown = 0;
      maestro.setTarget(servoList[15].channel, analogServoConversion(minOcchiValue, servoList[15]));
      maestro.setTarget(servoList[18].channel, analogServoConversion(minOcchiValue, servoList[18]));
    }
    return;
  }
  if (sitOcchi == INCHIUSURA) {
    contatoreOcchi++;
    if (contatoreOcchi == limiteOcchi) {
      servoList[15].counterShutDown = 0;
      servoList[18].counterShutDown = 0;
      maestro.setTarget(servoList[15].channel, analogServoConversion(maxOcchiValue, servoList[15]));
      maestro.setTarget(servoList[18].channel, analogServoConversion(maxOcchiValue, servoList[18]));
      sitOcchi = APERTI;
    }
    return;
  }

  if (sitOcchi == CHIUSMANUAL) {
    contatoreOcchi++;
    if (contatoreOcchi == limiteOcchi) {
      servoList[15].counterShutDown = 0;
      servoList[18].counterShutDown = 0;
      maestro.setTarget(servoList[15].channel, analogServoConversion(maxOcchiValue, servoList[15]));
      maestro.setTarget(servoList[18].channel, analogServoConversion(maxOcchiValue, servoList[18]));
      sitOcchi = MANUAL;
    }
    return;
  }
}


int analogServoConversion(int analogValue, ServoValues& servo) {
  if (servo.mirror)
    return map(analogValue, 0, 1023, servo.maxValue,servo.minValue);
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
    if (servoList[i].counterShutDown >= servoList[i].shutDownWhen){
      maestro.setTarget(servoList[i].channel, 0);
      servoList[i].counterShutDown = servoList[i].shutDownWhen;
    }else{
      servoList[i].counterShutDown = servoList[i].counterShutDown + 1;
    }
  }
}


void deadManButton() {
  if (aliveCounter % aliveTrigger == 0)
    Serial3.println("ALIVE");

  aliveCounter++;
}