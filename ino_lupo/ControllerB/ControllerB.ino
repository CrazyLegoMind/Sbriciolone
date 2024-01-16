const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';

const char eyesC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char noseC = 'N';
const char tailC = 'T';

const byte analogFilter = 10;
const byte delayLettura = 2;
const byte delayLoop = 20;
int aliveCounter = 0;
const byte aliveTrigger = 10;

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
  boolean value;
};

const byte howmanyanalog = 7;  //9
Motor listaMotori[howmanyanalog];
Motor tailMot;
Motor mouthDxMot;

LedSwc eyebagMirrorSwc;    //non deve mai inviare alla testa //pulsante blu
LedSwc eyebrowsMirrorSwc;  //non deve mai inviare alla testa //pulsante aranci
int tailRightBtn = 2;
int tailLeftBtn = 3;
bool rightTrigger = false;
bool leftTrigger = false;

void setup() {
  Serial.begin(115200);


  eyebrowsMirrorSwc.pin = 5;
  eyebrowsMirrorSwc.led = 4;
  eyebrowsMirrorSwc.value = false;

  eyebagMirrorSwc.pin = 7;
  eyebagMirrorSwc.led = 6;
  eyebagMirrorSwc.value = false;

  pinMode(eyebrowsMirrorSwc.pin, INPUT);
  pinMode(eyebrowsMirrorSwc.led, OUTPUT);
  pinMode(eyebagMirrorSwc.pin, INPUT);
  pinMode(eyebagMirrorSwc.led, OUTPUT);
  pinMode(tailRightBtn, INPUT_PULLUP);
  pinMode(tailLeftBtn, INPUT_PULLUP);


  listaMotori[0].sector = eyebrownsC;  //sopracciglio ext sx
  listaMotori[0].port = A0;
  listaMotori[0].pinH = 9;

  listaMotori[1].sector = eyebrownsC;  //sopracciglio int sx
  listaMotori[1].port = A1;
  listaMotori[1].pinH = 8;

  listaMotori[2].sector = eyebrownsC;  //sopracciglio int dx
  listaMotori[2].port = A2;
  listaMotori[2].pinH = 7;

  listaMotori[3].sector = eyebrownsC;  //sopracciglio ext dx
  listaMotori[3].port = A3;
  listaMotori[3].pinH = 6;

  listaMotori[4].sector = noseC;  //borsa sx
  listaMotori[4].port = A4;
  listaMotori[4].pinH = 11;

  listaMotori[5].sector = noseC;  //borsa dx
  listaMotori[5].port = A5;
  listaMotori[5].pinH = 10;

  listaMotori[6].sector = mouthC;  //bocca apertura SX
  listaMotori[6].port = A6;
  listaMotori[6].pinH = 12;

  // mouthDxMot.sector = mouthC; //bocca apertura DX
  // mouthDxMot.port = -1;
  // mouthDxMot.pinH = 13;

  tailMot.sector = tailC;  //motore coda
  tailMot.pinH = -1;
  tailMot.event = eventsC;
}

void loop() {

  handlebtns();
  readLedSwc(eyebrowsMirrorSwc);  //non invia nulla alla testa
  readLedSwc(eyebagMirrorSwc);    //non invia nulla alla testa
  handleSliders();
  deadManButton();
  delay(delayLoop);
}


void handleSliders() {
  for (int slider = 0; slider < howmanyanalog; slider++) {
    if (eyebrowsMirrorSwc.value && (slider == 2 || slider == 3)) {
      continue;  //skip loop if motors are mirrored
    }
    if (eyebagMirrorSwc.value && slider == 5) {
      continue;  //skip loop if motors are mirrored
    }
    int sliderVal = analogRead(listaMotori[slider].port);
    if (abs(sliderVal - listaMotori[slider].oldValue) > analogFilter) {
      listaMotori[slider].oldValue = sliderVal;
      sendMotor(listaMotori[slider], sliderVal);
      // if(slider == 6){
      //   sendMotor(mouthDxMot, 1023-sliderVal);
      // }

      if (eyebrowsMirrorSwc.value) {
        switch (slider) {
          case 0:
            sendMotor(listaMotori[3], sliderVal);
            break;
          case 1:
            sendMotor(listaMotori[2], sliderVal);
            break;
        }
      }
      if (eyebagMirrorSwc.value && slider == 4) {
        sendMotor(listaMotori[5], sliderVal);
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


void handlebtns() {
  int rightState = !digitalRead(tailRightBtn);
  int leftState = !digitalRead(tailLeftBtn);
  if (rightState && !rightTrigger) {
    rightTrigger = true;
    //Serial.println("event--------------------");
    sendMotor(tailMot, 1023);
  } else if (!rightState) {
    rightTrigger = false;
  }

  if (leftState && !leftTrigger) {
    leftTrigger = true;
    sendMotor(tailMot, 0);
    //Serial.println("event--------------------");
  } else if (!leftState) {
    leftTrigger = false;
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
