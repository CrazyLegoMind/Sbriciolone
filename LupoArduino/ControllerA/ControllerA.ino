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

struct LedSwc {
  int pin;
  int led;
  char sector;
  char event = statusChangeC;
  boolean value;
};

const byte delayLettura = 7;
const byte delayLoop = 20;
const byte Analogfilter = 6;
const byte closeEyesButtonPin = 7;
int closeEyesState = 0;
int oldCloseEyesState = 0;


int aliveCounter = 0;
const byte aliveTrigger = 10;


const byte howmanyanalog = 9;  //Sono 8 invero
Motor listaMotori[howmanyanalog];
Motor eyeSXX;
Motor eyelidBottom;
LedSwc labbraSingleSwc; // verde
LedSwc labbraSinchroSwc; //rosso

int checkSumForEvent1;
int checkSumForEvent0;

int checkSumFunction(String SCS) {
  char bufferChar[SCS.length()];
  SCS.toCharArray(bufferChar, SCS.length());

  int sum = 0;
  for (int i = 0; i < SCS.length(); i++) {
    sum += bufferChar[i];
  }

  return sum % 100;
}


void setup() {


  checkSumForEvent1 = checkSumFunction("LC;1;");
  checkSumForEvent0 = checkSumFunction("LC;0;");


  labbraSingleSwc.pin = 3;
  labbraSingleSwc.led = 4;
  labbraSingleSwc.sector = eyesC;
  labbraSingleSwc.value = false;


  labbraSinchroSwc.pin = 5;
  labbraSinchroSwc.led = 6;
  labbraSinchroSwc.sector = eyeLidsC;
  labbraSinchroSwc.value = false;

  pinMode(labbraSingleSwc.pin, INPUT);
  pinMode(labbraSingleSwc.led, OUTPUT);
  pinMode(labbraSinchroSwc.pin, INPUT);
  pinMode(labbraSinchroSwc.led, OUTPUT);
  pinMode(closeEyesButtonPin, INPUT);

  listaMotori[0].sector = eyesC;  //OcchiY
  listaMotori[0].port = A1;       //
  listaMotori[0].pinH = 14;

  listaMotori[1].sector = eyesC;  //OcchioDXX
  listaMotori[1].port = A0;
  listaMotori[1].pinH = 16;

  listaMotori[2].sector = mouthC;  //labbra sup dx
  listaMotori[2].port = A5;
  listaMotori[2].pinH = 0;

  listaMotori[3].sector = eyebrownsC;  //labbra inf dx
  listaMotori[3].port = A4;            //
  listaMotori[3].pinH = 3;

  listaMotori[4].sector = eyebrownsC;  //labbra inf sx
  listaMotori[4].port = A3;
  listaMotori[4].pinH = 2;

  listaMotori[5].sector = eyebrownsC;  //labbra sup sx
  listaMotori[5].port = A2;            //
  listaMotori[5].pinH = 5;

  listaMotori[6].sector = noseC;  //palpebre sup
  listaMotori[6].port = A6;
  listaMotori[6].pinH = 1;

  listaMotori[7].sector = eyebrownsC;  //orecchie
  listaMotori[7].port = A7;
  listaMotori[7].pinH =4;

  listaMotori[8].sector = eyeLidsC;  //palpebre sup
  listaMotori[8].port = A8;
  listaMotori[8].pinH = 17; //anchen 19

  eyeSXX.sector = eyesC;
  eyeSXX.port = -1;
  eyeSXX.pinH = 15;

  eyelidBottom.sector = eyeLidsC;
  eyelidBottom.port = -1;
  eyelidBottom.pinH = 18;

  readLedSwc(labbraSingleSwc);
  readLedSwc(labbraSinchroSwc);

  Serial.begin(115200);
}


void loop() {

  for (int i = 0; i < howmanyanalog; i++) {

    readWriteMotor(listaMotori[i]);
  }



  readLedSwc(labbraSingleSwc);
  readLedSwc(labbraSinchroSwc);

  deadManButton();
  delay(delayLoop);
}


void readWriteMotor(Motor& m) {
  int sensorValue = analogRead(m.port);

  if (abs(m.oldValue - sensorValue) > Analogfilter) {
    if (m.pinH == listaMotori[1].pinH)
      sendMotor(eyeSXX, sensorValue);

    if (m.pinH == listaMotori[8].pinH)
      sendMotor(eyelidBottom, sensorValue);

    sendMotor(m, sensorValue);
  }
  m.oldValue = sensorValue;
}

void readLedSwc(LedSwc& button) {
  int lettura = digitalRead(button.pin);


  if (lettura == HIGH) {
    button.value = true;
    digitalWrite(button.led, HIGH);
  } else {
    button.value = false;
    digitalWrite(button.led, LOW);
  }
}


void sendMotor(Motor& m, int sensorValue) {
  String SCS = "";
  SCS += m.sector;
  SCS += m.event;
  SCS += ';';
  SCS += m.pinH;
  SCS += ';';
  SCS += sensorValue;
  SCS += ';';

  //Serial.println(SCS);

  char bufferChar[SCS.length()];
  SCS.toCharArray(bufferChar, SCS.length());

  int sum = 0;
  for (int i = 0; i < SCS.length(); i++) {
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
}

void deadManButton() {
  if (aliveCounter % aliveTrigger == 0)
    Serial.println("ALIVE");

  aliveCounter++;
}
