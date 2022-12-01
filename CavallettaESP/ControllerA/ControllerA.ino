const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'A';


const char eyesC = 'E';
const char tongueC = 'T';
const char antennasC = 'A';
const char mouthC = 'M';
const char jawsC = 'J';


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

const byte delayLoop = 20;
const byte analogFilter = 10;

int aliveCounter = 0;
const byte aliveTrigger = 10;


const byte howmanyanalog = 9;
Motor listaMotori[howmanyanalog];
Motor eyeSXX;
Motor eyelidBottom;
LedSwc antennasSinchroSwc;  // verde
LedSwc labbraSinchroSwc;    //rosso

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





  antennasSinchroSwc.pin = 5;
  antennasSinchroSwc.led = 6;
  antennasSinchroSwc.sector = eyesC;
  antennasSinchroSwc.value = false;


  labbraSinchroSwc.pin = 3;
  labbraSinchroSwc.led = 4;
  labbraSinchroSwc.sector = tongueC;
  labbraSinchroSwc.value = false;

  pinMode(antennasSinchroSwc.pin, INPUT);
  pinMode(antennasSinchroSwc.led, OUTPUT);
  pinMode(labbraSinchroSwc.pin, INPUT);
  pinMode(labbraSinchroSwc.led, OUTPUT);

  listaMotori[0].sector = tongueC;  // lingua y
  listaMotori[0].port = A1;
  listaMotori[0].pinH = 2;

  listaMotori[1].sector = tongueC;  // lingua x
  listaMotori[1].port = A0;
  listaMotori[1].pinH = 3;

  listaMotori[2].sector = jawsC;  //labbra sup dx -> antenna l
  listaMotori[2].port = A5;
  listaMotori[2].pinH = 7;

  listaMotori[3].sector = jawsC;  //labbra inf dx -> antenna r
  listaMotori[3].port = A4;
  listaMotori[3].pinH = 8;

  listaMotori[4].sector = antennasC;  //labbra inf sx ->mascella r
  listaMotori[4].port = A3;
  listaMotori[4].pinH = 0;

  listaMotori[5].sector = antennasC;  //labbra sup sx ->mascella l
  listaMotori[5].port = A2;
  listaMotori[5].pinH = 1;

  listaMotori[6].sector = mouthC;  //naso -> lab sup
  listaMotori[6].port = A6;
  listaMotori[6].pinH = 5;

  listaMotori[7].sector = mouthC;  //orecchie -> lbb inf
  listaMotori[7].port = A7;
  listaMotori[7].pinH = 6;

  listaMotori[8].sector = tongueC;  //lingua z
  listaMotori[8].port = A8;
  listaMotori[8].pinH = 4;

  Serial.begin(115200);
  checkSumForEvent1 = checkSumFunction("LC;1;");
  checkSumForEvent0 = checkSumFunction("A;0;");
  //Serial.println(checkSumForEvent0); //72
}


void loop() {

  readLedSwc(antennasSinchroSwc);
  readLedSwc(labbraSinchroSwc);
  handleSliders();

  deadManButton();
  delay(delayLoop);
}

void handleSliders() {
  for (int slider = 0; slider < howmanyanalog; slider++) {
    volatile Motor linkedMot = listaMotori[slider];
    int sliderch = linkedMot.pinH;

    if (antennasSinchroSwc.value && (sliderch == 1)) {
      continue;  //skip loop if motors are mirrored
    }
    if (labbraSinchroSwc.value && (sliderch == 6)) {
      continue;  //skip loop if motors are mirrored
    }
    int sliderVal = analogRead(linkedMot.port);

    if (abs(sliderVal - linkedMot.oldValue) > analogFilter) {

      sendMotor(listaMotori[slider], sliderVal);

      if (antennasSinchroSwc.value && sliderch == 0) {
        sendMotor(listaMotori[5], sliderVal);
      }
      if (labbraSinchroSwc.value && sliderch == 5) {

        sendMotor(listaMotori[7], 1023 - sliderVal);
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
