#include <SoftwareSerial.h>
#include <PWMServo.h>

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

SoftwareSerial BTSerial(2, 3);
PWMServo tailServo;
int tailServoPos = 1500;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(115200);
  BTSerial.setTimeout(20);
  tailServo.attach(SERVO_PIN_A);  // attaches the servo on pin 9 to the servo object
  //tailServo.attach(SERVO_PIN_A, 1000, 2000); // some motors need min/max setting
}


void loop() {
  String message = BTSerial.readStringUntil('\n');
  if (message.length() > 0)
  {

    bool doIt = true;
    if (message.charAt(0) != 'r')
    {
      
      long randNumber = random(100);

      if (randNumber < 20)
      {
        doIt = false;
      }
    }
    else
    {
      message = message.substring(1);
    }
    if (doIt)
    {

      //delay(30);
      int lenghtMessage = getLenghtBeforeCheckSum(message, ';');
      int numberSeparators = homManySeparator(message, ';');
      int checksum = getValueStringSplitter(message, ';', numberSeparators).toInt();

      char bufferChar[lenghtMessage];
      message.toCharArray(bufferChar, lenghtMessage);

      int sum = 0;
      for (int i = 0; i <  lenghtMessage; i++)
      {
        sum += bufferChar[i];
      }


      int myCheckSum = sum % 100;

      if (myCheckSum == checksum)
      {
        if (message.charAt(0) == tailC)
        {
          handleTail(message);
        }
      }
    }
  }
  deadManButton();
  //shutDownMotors();
  //delay(20);
}

void handleTail(String message){
  if (message.charAt(1) == eventsC){
    Serial.println("tail event");
  }else{
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    int angle = map(value,0,1023,30,150);
    tailServo.write(angle);
  }
}

int analogServoConversion(int analogValue, ServoValues & servo)
{
  if (servo.mirror)
    return map(analogValue, 1023, 0, servo.minValue, servo.maxValue);
  return map(analogValue, 0, 1023, servo.minValue, servo.maxValue);
}


int getLenghtBeforeCheckSum(String data, char separator)
{
  int l = -1;

  for (int i = 0; i < data.length(); i++)
    if (data.charAt(i) == separator)
      l = i;
  return (l + 1);
}

int homManySeparator(String data, char separator)
{
  int s = 0;
  for (int i = 0; i < data.length(); i++)
    if (data.charAt(i) == separator)
      s++;
  return s;
}

String getValueStringSplitter(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
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

void deadManButton()
{
  if (aliveCounter % aliveTrigger == 0)
    BTSerial.println("ALIVE");

  aliveCounter++;
}
