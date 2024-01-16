#include <SoftwareSerial.h>
#include <PWMServo.h>
//#define DEBUG

const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';

const char eyesC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char noseC = 'N';
const char tailC = 'T';

int aliveCounter = 0;
const byte aliveTrigger = 10;
bool servo_busy = false;
unsigned long current_time = 0;
unsigned long trigger_time = 0;
int sweep_delay = 100;
bool standby = false;
unsigned long standby_time = 0;

SoftwareSerial BTSerial(3, 2);
PWMServo tailServo;
int tailServoPos = 1500;
int tailSweepValue = 1500;
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("DEBUG READY");
#endif
  BTSerial.begin(38400);
  BTSerial.setTimeout(20);                    // attaches the servo on pin 9 to the servo object
}


void loop() {
  String message = BTSerial.readStringUntil('\n');
  int msglen = message.length();
  unsigned long us = micros();
  current_time = millis();

  if (message.length() > 0) {
    bool doIt = true;

    Serial.print(message);
    if (message.charAt(0) == 'r') {
      message = message.substring(1);
    }
    int pckLen = getLenghtBeforeCheckSum(message, ';');
    int numberSeparators = homManySeparator(message, ';');
    int checksum = getValueStringSplitter(message, ';', numberSeparators).toInt();
    char bufferChar[pckLen];
    message.toCharArray(bufferChar, pckLen);

    int sum = 0;
    for (int i = 0; i < pckLen - 1; i++) {
      sum += bufferChar[i];
    }
    int myCheckSum = sum % 100;
    if (myCheckSum != checksum) {
      doIt = false;
      //Serial.print(" csm fail: ");
      //Serial.println(myCheckSum);
    } else {
      //Serial.println(" Pass");
    }


    if (doIt && !servo_busy) {
      if (message.charAt(0) == tailC) {
        if (message.charAt(1) == eventsC) {
          String valueString = getValueStringSplitter(message, ';', 2);
          int value = valueString.toInt();
          tailSweepValue = map(value, 0, 1023, 1000, 2000);
        } else if (message.charAt(1) == servoC) {
          handleTail(message);
          ;
        }
      }
    }
  }
  handleServo();
  deadManButton();
  //shutDownMotors();
  delay(5);
  if (message.length() > 0) {
    unsigned long elapsed = micros() - us;
    //Serial.print("Check took: ");
    //Serial.println(elapsed);
  }
}
void handleTail(String message) {
  return;
  String valueString = getValueStringSplitter(message, ';', 2);
  int value = valueString.toInt();
  if (value < 300){
    tailSweepValue = 1000;
  }
}


void handleServo() {
  if (tailSweepValue != 1500 && !servo_busy) {
    standby = false;
    servo_busy = true;
    if(!tailServo.attached())
      tailServo.attach(SERVO_PIN_A, 1000, 2000);

    tailServo.write(microsToAngle(tailSweepValue));
    Serial.println(microsToAngle(tailSweepValue));
    if (trigger_time == 0){
      trigger_time = current_time;
    }
  }

  if (tailSweepValue != 1500) {
    if (current_time - trigger_time >= sweep_delay) {
      servo_busy = false;
      if (tailSweepValue > 1500) {
        tailSweepValue -= 20;
        if (tailSweepValue <= 1500) {
          tailSweepValue = 1500;
          trigger_time == 0;
        }
      } else if (tailSweepValue < 1500) {
        tailSweepValue += 20;
        if (tailSweepValue >= 1500) {
          tailSweepValue = 1500;
          trigger_time == 0;
        }
      }
    }
  }
  if (tailSweepValue == 1500) {
    if (standby_time == 0 && !standby) {
      standby_time = current_time;
    }
    if (current_time - standby_time >= 100) {
      tailServo.detach();
      standby_time == 0;
      standby = true;
    }
  }
}


int microsToAngle(int micros) {
  int res = map(micros, 1000, 2000, 0, 180);
  res = constrain(res,1,179);
  return res;
}


int getLenghtBeforeCheckSum(String data, char separator) {
  int l = -1;

  for (int i = 0; i < data.length(); i++)
    if (data.charAt(i) == separator)
      l = i;
  return (l + 1);
}

int homManySeparator(String data, char separator) {
  int s = 0;
  for (int i = 0; i < data.length(); i++)
    if (data.charAt(i) == separator)
      s++;
  return s;
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

/*
void shutDownMotors()
{
  for (int i = 0; i < howmanyservo; i++)
  {
    if (!servoList[i].stopAndGo)
      continue;
    if (servoList[i].counterShutDown > servoList[i].shutDownWhen)
      maestro.setTarget(servoList[i].channel, 0);
    else
      servoList[i].counterShutDown = servoList[i].counterShutDown + 1;
  }
}
*/

void deadManButton() {
  if (aliveCounter % aliveTrigger == 0)
    BTSerial.println("ALIVE");

  aliveCounter++;
}