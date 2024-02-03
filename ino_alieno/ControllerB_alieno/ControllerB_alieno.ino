const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'G';

const char earC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char snoutC = 'T';

const byte analogFilter = 10;
#define DELAY_LOOP 5
#define DELAY_SERVO 2
#define ANALOG_PORTS 6

unsigned long ms_last_alive_sent = 0;
unsigned long ms_alive_spacing = 500;

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

const byte analog_amount = ANALOG_PORTS+1;
Motor listaMotori[analog_amount];
LedSwc eyebagMirrorSwc;  //non deve mai inviare alla testa //pulsante blu

void setup() {
  Serial3.begin(115200);


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
  //Serial.begin(115200);
}

void loop() {
  readLedSwc(eyebagMirrorSwc);  //non invia nulla alla testa
  handleSliders();
  deadManButton();
  delay(DELAY_LOOP);
}


void handleSliders() {
  for (int slider = 0; slider < analog_amount; slider++) {
    int servoch = listaMotori[slider].servoCh;
    if (eyebagMirrorSwc.value && (slider == 1 || slider == 2)) {
      continue;  //skip loop if motors are mirrored
    }
    int sliderVal = analogRead(listaMotori[slider].arduinoPin);
    //Serial.print(sliderVal);
    //Serial.print(" ");
    if (abs(sliderVal - listaMotori[slider].oldValue) > analogFilter) {
      
      listaMotori[slider].oldValue = sliderVal;
      sendMotor(listaMotori[slider], sliderVal);
      if (eyebagMirrorSwc.value) {
        switch (slider) {
          case 5:
            sendMotor(listaMotori[1], sliderVal);
            break;
          case 4:
            sendMotor(listaMotori[2], sliderVal);
            break;
        }
      }
    }
  }
  //Serial.println();
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


  Serial3.print(m.sector);
  Serial3.print(m.event);
  Serial3.print(';');
  Serial3.print(m.servoCh);
  Serial3.print(';');
  Serial3.print(sensorValue);
  Serial3.print(';');
  Serial3.println(checkSum);
  delay(DELAY_SERVO);
  m.oldValue = sensorValue;
}

void deadManButton() {
  unsigned long current_time = millis();
  if(current_time - ms_last_alive_sent > ms_alive_spacing){
    ms_last_alive_sent = current_time;
    Serial3.println("ALIVE");
  }
}