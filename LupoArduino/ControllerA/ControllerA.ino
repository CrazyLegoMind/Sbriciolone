const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';

const char eyesC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char noseC = 'N';
const char tailC = 'T';

struct Motor {
  int port;
  char sector;
  int pinH;
  char event = servoC;
  int oldValue = 0;
};

struct ButtonLed {
  int pin;
  int led;
  char sector;
  char event = statusChangeC;
  boolean value;
};

const byte delayLettura = 7;
const byte delayLoop = 52;
const byte Analogfilter = 6;
const byte closeEyesButtonPin = 7;
int closeEyesState = 0;
int oldCloseEyesState = 0;


int aliveCounter = 0;
const byte aliveTrigger = 10;


const byte howmanyanalog = 9;//Sono 8 invero
Motor listaMotori[howmanyanalog];
Motor eyeSXX;
Motor eyeSXY;
ButtonLed mirrorButton;
ButtonLed palpebreButton;

int checkSumForEvent1;
int checkSumForEvent0;

int chesumTail = (eyeLidsC + eventsC + ';') % 100;

int checkSumFunction(String SCS)
{
  char bufferChar[SCS.length()];
  SCS.toCharArray(bufferChar, SCS.length());

  int sum = 0;
  for (int i = 0; i <  SCS.length(); i++)
  {
    sum += bufferChar[i];
  }

  return sum % 100;
}


void setup()
{


  checkSumForEvent1 = checkSumFunction("LC;1;");
  checkSumForEvent0 = checkSumFunction("LC;0;");
  chesumTail = checkSumFunction("Te;");


  mirrorButton.pin = 3;
  mirrorButton.led = 4;
  mirrorButton.sector = eyesC;
  mirrorButton.value = false;


  palpebreButton.pin = 5;
  palpebreButton.led = 6;
  palpebreButton.sector = eyeLidsC;
  palpebreButton.value = false;

  pinMode(mirrorButton.pin, INPUT);
  pinMode(mirrorButton.led, OUTPUT);
  pinMode(palpebreButton.pin, INPUT);
  pinMode(palpebreButton.led, OUTPUT);
  pinMode(closeEyesButtonPin, INPUT);

  listaMotori[0].sector = eyesC; //OcchiY
  listaMotori[0].port = A1;//
  listaMotori[0].pinH = 14;

  listaMotori[1].sector = eyesC; //OcchioDXX
  listaMotori[1].port = A0;
  listaMotori[1].pinH = 16;

  listaMotori[2].sector = eyeLidsC; //labbra inf sx
  listaMotori[2].port = A2;
  listaMotori[2].pinH = 5;

  listaMotori[3].sector = eyebrownsC; //labbra sup sx
  listaMotori[3].port = A3;//
  listaMotori[3].pinH = 2;

  listaMotori[4].sector = eyebrownsC; //labbra sup dx
  listaMotori[4].port = A4;
  listaMotori[4].pinH = 0;

  listaMotori[5].sector = eyebrownsC; //labbra inf dx
  listaMotori[5].port = A5;//
  listaMotori[5].pinH = 3;

  listaMotori[6].sector = eyelidsC; //palpebre sup
  listaMotori[6].port = A6;
  listaMotori[6].pinH = 17; // anche 18

  listaMotori[7].sector = noseC;//narici
  listaMotori[7].port = A7;
  listaMotori[7].pinH = 1;

  listaMotori[8].sector = eyebrownsC; //orecchie
  listaMotori[8].port = A8;
  listaMotori[8].pinH = 4;

  eyeSXX.sector = eyesC;
  eyeSXX.port = -1;
  eyeSXX.pinH = 15;

  readButtonLed(mirrorButton);
  readButtonLedAndSend(palpebreButton);

  Serial.begin(115200);
}


void loop() {

  for (int i = 0; i < howmanyanalog; i++)
  {
    if (listaMotori[i].sector == eyeLidsC)
      if (palpebreButton.value)
        if (i == 2)
          continue;


    readWriteMotor(listaMotori[i]);
  }



  readCloseEyesButton();
  readButtonLed(mirrorButton);
  readButtonLedAndSend(palpebreButton);

  deadManButton();
  delay(delayLoop);
}


void readCloseEyesButton() {
  closeEyesState = digitalRead(closeEyesButtonPin);

  if (closeEyesState != oldCloseEyesState)
    if (closeEyesState == LOW) {
      String SCS = "";
      SCS += 'T';
      SCS += 'e';
      SCS += ';';
      SCS += 0;
      SCS += ';';
      SCS += 1023;
      SCS += ';';

      char bufferChar[SCS.length()];
      SCS.toCharArray(bufferChar, SCS.length());

      int sum = 0;
      for (int i = 0; i < SCS.length(); i++) {
        sum += bufferChar[i];
      }

      int checkSum = sum % 100;

      // Serial.print(m.sector);
      // Serial.print(m.event);
      // Serial.print(';');
      // Serial.print(m.pinH);
      // Serial.print(';');
      // Serial.print(sensorValue);
      Serial.print(SCS);
      Serial.println(checkSum);
    }

  oldCloseEyesState = closeEyesState;
}

void readWriteMotor(Motor& m)
{
  int sensorValue = analogRead(m.port);

  if (abs(m.oldValue - sensorValue) > Analogfilter)
  {
    if (m.pinH == listaMotori[0].pinH)
      sendMotor(eyeSXY, sensorValue);
    else if (m.pinH == listaMotori[1].pinH)
    {
      if (mirrorButton.value)
        sendMotor(eyeSXX, mirrorEye(sensorValue));
      else
        sendMotor(eyeSXX, sensorValue);
    }

    if (m.pinH == listaMotori[7].pinH && palpebreButton.value)
      sendMotor(listaMotori[2], sensorValue);
      
    sendMotor(m, sensorValue);

  }
  m.oldValue = sensorValue;
  
}

void readButtonLed(ButtonLed& button)
{
  int lettura = digitalRead(button.pin);


  if (lettura == HIGH)
  {
    button.value = true;
    digitalWrite(button.led, HIGH);
  }
  else
  {
    button.value = false;
    digitalWrite(button.led, LOW);
  }
}



void readButtonLedAndSend(ButtonLed& button)
{
  int lettura = digitalRead(button.pin);
  if (lettura == HIGH && button.value != true)
  {
    button.value = true;
    digitalWrite(button.led, HIGH);
    /*
      Serial.print(button.sector);
      Serial.print(button.event);
      Serial.print(';');
      Serial.print('1');
      Serial.print(';');
      Serial.println(checkSumForEvent1);
      delay(delayLettura);
    */
  }
  else if (lettura == LOW && button.value != false)
  {
    button.value = false;
    digitalWrite(button.led, LOW);
    /*
      Serial.print(button.sector);
      Serial.print(button.event);
      Serial.print(';');
      Serial.print('0');
      Serial.print(';');
      Serial.println(checkSumForEvent0);
      delay(delayLettura);
    */
  }
}


int mirrorEye(int value)
{
  return map(value, 0, 1023, 1023, 0);
}


void sendMotor(Motor& m, int sensorValue)
{
  String SCS = "";
  SCS += m.sector;
  SCS +=  m.event;
  SCS +=  ';';
  SCS +=  m.pinH;
  SCS +=  ';';
  SCS += sensorValue;
  SCS += ';';

  //Serial.println(SCS);

  char bufferChar[SCS.length()];
  SCS.toCharArray(bufferChar, SCS.length());

  int sum = 0;
  for (int i = 0; i <  SCS.length(); i++)
  {
    sum += bufferChar[i];
  }

  int checkSum = sum % 100;

  Serial.print(m.sector);
  Serial.print(m.event);
  Serial.print(';');
  Serial.print(m.pinH);
  Serial.print(';');
  Serial.print(sensorValue);
  Serial.print(';');
  Serial.println(checkSum);
  delay(delayLettura);



}

void deadManButton()
{
  if (aliveCounter % aliveTrigger == 0)
    Serial.println("ALIVE");

  aliveCounter++;
}
