const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'A';

const char earC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char snoutC = 'T';

const byte analogFilter = 10;
const byte delayLoop = 20;
const byte delay_servo = 5;

int aliveCounter = 0;
const byte aliveTrigger = 10;

struct Motor {
  char sector;
  char event = servoC;
  int arduinoPin;
  int servoCh;
  int oldValue = 0;
};

struct LedSwc {
  int pin;
  int led;
  boolean value;
};

const byte howmanyanalog = 7;
Motor listaMotori[howmanyanalog];


LedSwc eyebagMirrorSwc;    //non deve mai inviare alla testa //pulsante blu

void setup() {
  Serial.begin(115200);


  eyebagMirrorSwc.pin = 5;
  eyebagMirrorSwc.led = 4;
  eyebagMirrorSwc.value = false;

  pinMode(eyebagMirrorSwc.pin, INPUT);
  pinMode(eyebagMirrorSwc.led, OUTPUT);

  listaMotori[0].sector = earC;  //orecch
  listaMotori[0].arduinoPin = A0;
  listaMotori[0].servoCh = 4;

  listaMotori[1].sector = mouthC;  //labbdx
  listaMotori[1].arduinoPin = A1;
  listaMotori[1].servoCh = 0;

  listaMotori[2].sector = snoutC;  //baffdx
  listaMotori[2].arduinoPin = A2;
  listaMotori[2].servoCh = 9;

  listaMotori[3].sector = snoutC;  //naso
  listaMotori[3].arduinoPin = A3;
  listaMotori[3].servoCh = 1;

  listaMotori[4].sector = snoutC;  //baffsx
  listaMotori[4].arduinoPin = A4;
  listaMotori[4].servoCh = 6;

  listaMotori[5].sector = mouthC;  //labbsx
  listaMotori[5].arduinoPin = A5;
  listaMotori[5].servoCh = 5;

  listaMotori[6].sector = mouthC;  //bocca
  listaMotori[6].arduinoPin = A6;
  listaMotori[6].servoCh = 14;

}

void loop() {
  readLedSwc(eyebagMirrorSwc);    //non invia nulla alla testa
  handleSliders();
  deadManButton();
  delay(delayLoop);
}


void handleSliders() {
  for (int slider = 0; slider < howmanyanalog; slider++) {
    int servoch = listaMotori[slider].servoCh;
    if (eyebagMirrorSwc.value && (slider == 4 || slider == 5)) {
      continue;  //skip loop if motors are mirrored
    }
    int sliderVal = analogRead(listaMotori[slider].arduinoPin);
    if (abs(sliderVal - listaMotori[slider].oldValue) > analogFilter) {
      listaMotori[slider].oldValue = sliderVal;
      sendMotor(listaMotori[slider], sliderVal);
      delay(delay_servo);
      if (eyebagMirrorSwc.value) {
        switch (slider) {
          case 1:
            sendMotor(listaMotori[5], sliderVal);
            delay(delay_servo);
            break;
          case 2:
            sendMotor(listaMotori[4], sliderVal);
            delay(delay_servo);
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
  SCS += m.servoCh;
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
  Serial.print(m.servoCh);
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
