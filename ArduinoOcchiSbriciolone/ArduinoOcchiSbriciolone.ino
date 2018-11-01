const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';

const char eyesC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char noseC = 'N';

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

const byte delayLettura = 4;
const byte delayLoop = 52;
const byte Analogfilter = 4;
const byte closeEyesButtonPin = 7;
int closeEyesState = 0;
int oldCloseEyesState = 0;


int aliveCounter = 0;
const byte aliveTrigger = 10;


const byte howmanyanalog = 8;//Sono 8 invero
Motor listaMotori[howmanyanalog];
Motor eyeSXX;
Motor eyeSXY;
ButtonLed mirrorButton;
ButtonLed palpebreButton;

void setup()
{
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

  listaMotori[0].sector = eyesC;//OcchioDXY
  listaMotori[0].port = A1;//
  listaMotori[0].pinH = 17;

  listaMotori[1].sector = eyesC;//OcchioDXX
  listaMotori[1].port = A0;
  listaMotori[1].pinH = 16;

  listaMotori[2].sector = eyeLidsC;//PalpebraSinistra
  listaMotori[2].port = A3;
  listaMotori[2].pinH = 18;

  listaMotori[3].sector = eyebrownsC;//Sopraciglio DXD
  listaMotori[3].port = A4;//
  listaMotori[3].pinH = 10;

  listaMotori[4].sector = eyebrownsC;//Sopraciglio DXC
  listaMotori[4].port = A5;
  listaMotori[4].pinH = 9;

  listaMotori[5].sector = eyebrownsC;//Sopraciglio SXS
  listaMotori[5].port = A6;//
  listaMotori[5].pinH = 12;

  listaMotori[6].sector = eyebrownsC;//Sopraciglio SXC
  listaMotori[6].port = A7;
  listaMotori[6].pinH = 11;

  listaMotori[7].sector = eyeLidsC;//PalpebraDestra
  listaMotori[7].port = A2;
  listaMotori[7].pinH = 15;

  eyeSXX.sector = eyesC;
  eyeSXX.port = -1;
  eyeSXX.pinH = 13;

  eyeSXY.sector = eyesC;
  eyeSXY.port = -1;
  eyeSXY.pinH = 14;

  readButtonLed(mirrorButton);
  readButtonLedAndSend(palpebreButton);

  Serial.begin(9600);
}


void loop() {

  for (int i = 0; i < howmanyanalog; i++)
  {
    if (listaMotori[i].sector == eyeLidsC)
      if (palpebreButton.value)
        continue;

    readWriteMotor(listaMotori[i]);
  }



  readCloseEyesButton();
  readButtonLed(mirrorButton);
  readButtonLedAndSend(palpebreButton);

  deadManButton();
  delay(delayLoop);
}


void readCloseEyesButton()
{
  closeEyesState = digitalRead(closeEyesButtonPin);

  if (closeEyesState != oldCloseEyesState)
    if (closeEyesState == LOW)
    {
      Serial.println("Le");
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
    Serial.print(button.sector);
    Serial.print(button.event);
    Serial.print(';');
    Serial.println('1');
    delay(delayLettura);
  }
  else if (lettura == LOW && button.value != false)
  {
    button.value = false;
    digitalWrite(button.led, LOW);
    Serial.print(button.sector);
    Serial.print(button.event);
    Serial.print(';');
    Serial.println('0');
    delay(delayLettura);
  }
}


int mirrorEye(int value)
{
  return map(value, 0, 1023, 1023, 0);
}


void sendMotor(Motor& m, int sensorValue)
{
  Serial.print(m.sector);
  Serial.print(m.event);
  Serial.print(';');
  Serial.print(m.pinH);
  Serial.print(';');
  Serial.println(sensorValue);
  delay(delayLettura);

}

void deadManButton()
{
  if (aliveCounter % aliveTrigger == 0)
    Serial.println("ALIVE");

  aliveCounter++;
}
