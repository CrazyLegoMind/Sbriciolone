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
const byte analogFilter = 6;
const byte closeEyesButtonPin = 7;
int closeEyesState = 0;
int oldCloseEyesState = 0;


int aliveCounter = 0;
const byte aliveTrigger = 10;


const byte howmanyanalog = 9;  //Sono 8 invero
Motor listaMotori[howmanyanalog];
Motor eyeSXX;
Motor eyelidBottom;
LedSwc labbraSingleSwc;   // verde
LedSwc labbraSinchroSwc;  //rosso

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


  labbraSingleSwc.pin = 5;
  labbraSingleSwc.led = 6;
  labbraSingleSwc.sector = eyesC;
  labbraSingleSwc.value = false;


  labbraSinchroSwc.pin = 3;
  labbraSinchroSwc.led = 4;
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
  listaMotori[7].pinH = 4;

  listaMotori[8].sector = eyeLidsC;  //palpebre sup
  listaMotori[8].port = A8;
  listaMotori[8].pinH = 17;  //anchen 19

  eyeSXX.sector = eyesC;
  eyeSXX.port = -1;
  eyeSXX.pinH = 15;

  eyelidBottom.sector = eyeLidsC;
  eyelidBottom.port = -1;
  eyelidBottom.pinH = 18;
  Serial.begin(115200);
}


void loop() {

  readLedSwc(labbraSingleSwc);
  readLedSwc(labbraSinchroSwc);
  handleSliders();

  deadManButton();
  delay(delayLoop);
}

void handleSliders() {
  for (int slider = 0; slider < howmanyanalog; slider++) {
    volatile Motor linkedMot = listaMotori[slider];
    int sliderch = linkedMot.pinH;

    if (labbraSingleSwc.value && (sliderch == 0 || sliderch == 3 || sliderch == 5)) {
      continue;  //skip loop if motors are mirrored
    }
    if (labbraSinchroSwc.value && (sliderch == 3 || sliderch == 0)) {
      continue;  //skip loop if motors are mirrored
    }
    int sliderVal = analogRead(linkedMot.port);

    if (abs(sliderVal - linkedMot.oldValue) > analogFilter) {
      
      sendMotor(listaMotori[slider], sliderVal);
      if (sliderch == listaMotori[1].pinH)
        sendMotor(eyeSXX, sliderVal);

      if (sliderch == listaMotori[8].pinH)
        sendMotor(eyelidBottom, sliderVal);
      
      if (labbraSingleSwc.value && sliderch == 2) {
        sendMotor(listaMotori[5], 1023-sliderVal);
        sendMotor(listaMotori[3], sliderVal);
        
        sendMotor(listaMotori[2], sliderVal);
      }else if (labbraSinchroSwc.value) {
        switch (sliderch) {
          case 2:
            sendMotor(listaMotori[2], sliderVal);
            break;
          case 5:
            sendMotor(listaMotori[3], 1023-sliderVal);
            break;
        }
      }
    }
  }
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
  m.oldValue = sensorValue;
}

void deadManButton() {
  if (aliveCounter % aliveTrigger == 0)
    Serial.println("ALIVE");

  aliveCounter++;
}
