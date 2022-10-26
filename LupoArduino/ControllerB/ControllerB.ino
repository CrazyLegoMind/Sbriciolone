#include <C:\Users\Admin\Documents\worspaces\animatronica\git_repo\Sbriciolone\LupoArduino\common.h>


const byte analogFilter = 6;
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
  listaMotori[0].pinH = 0;

  listaMotori[1].sector = eyebrownsC;  //sopracciglio int sx
  listaMotori[1].port = A1;
  listaMotori[1].pinH = 1;

  listaMotori[2].sector = eyebrownsC;  //sopracciglio int dx
  listaMotori[2].port = A2;
  listaMotori[2].pinH = 2;

  listaMotori[3].sector = eyebrownsC;  //sopracciglio ext dx
  listaMotori[3].port = A3;
  listaMotori[3].pinH = 3;

  listaMotori[4].sector = noseC;  //borsa sx
  listaMotori[4].port = A4;
  listaMotori[4].pinH = 4;

  listaMotori[5].sector = noseC;  //borsa dx
  listaMotori[5].port = A5;
  listaMotori[5].pinH = 5;

  listaMotori[6].sector = mouthC;  //bocca apertura
  listaMotori[6].port = A6;
  listaMotori[6].pinH = 6;

  tailMot.sector = tailC;  //motore coda
  tailMot.pinH = 9;
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
      sendMotor(listaMotori[slider], sliderVal);
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
    listaMotori[slider].oldValue = sliderVal;
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
    Serial.println("event--------------------");
    sendMotor(tailMot, 1023);
  } else if (!rightState) {
    rightTrigger = false;
  }

  if (leftState && !leftTrigger) {
    leftTrigger = true;
    sendMotor(tailMot, 0);
    Serial.println("event--------------------");
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