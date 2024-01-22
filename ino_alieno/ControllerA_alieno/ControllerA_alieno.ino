const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'G';

const char earC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char snoutC = 'T';


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
  char sector;
  char event = statusChangeC;
  boolean value;
};

const byte delayLoop = 20;
const byte analogFilter = 10;

int aliveCounter = 0;
const byte aliveTrigger = 10;

const char blink_dx_btn_pin = 5;
const char blink_sx_btn_pin = 6;
int right_eye = 0;
int left_eye = 0;


bool blinkSx = false;
bool blinkDx = false;

const byte howmanyanalog = 8;  //Sono 7 invero
Motor listaMotori[howmanyanalog];
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



  labbraSinchroSwc.pin = 3;
  labbraSinchroSwc.led = 4;
  labbraSinchroSwc.value = false;

  pinMode(labbraSinchroSwc.pin, INPUT);
  pinMode(labbraSinchroSwc.led, OUTPUT);
  pinMode(blink_dx_btn_pin, INPUT_PULLUP);
  pinMode(blink_sx_btn_pin, INPUT_PULLUP);

  listaMotori[0].sector = eyeLidsC; //occhiodx
  listaMotori[0].arduinoPin = A1;
  listaMotori[0].servoCh = 12;

  listaMotori[1].sector = eyeLidsC; // occhiosx
  listaMotori[1].arduinoPin = A0;
  listaMotori[1].servoCh = 13;

  listaMotori[2].sector = eyebrownsC; //guasx
  listaMotori[2].arduinoPin = A5;
  listaMotori[2].servoCh = 10;

  listaMotori[3].sector = eyebrownsC; //guadx
  listaMotori[3].arduinoPin = A4;
  listaMotori[3].servoCh = 11;

  listaMotori[4].sector = eyebrownsC; //edx
  listaMotori[4].arduinoPin = A3;
  listaMotori[4].servoCh = 8;

  listaMotori[5].sector = eyebrownsC; //cdx
  listaMotori[5].arduinoPin = A2;
  listaMotori[5].servoCh = 3;

  listaMotori[6].sector = eyebrownsC; //csx
  listaMotori[6].arduinoPin = A6;
  listaMotori[6].servoCh = 2;

  listaMotori[7].sector = eyebrownsC; //esx
  listaMotori[7].arduinoPin = A7;
  listaMotori[7].servoCh = 7;


  Serial.begin(115200);
  checkSumForEvent1 = checkSumFunction("LC;1;");
  checkSumForEvent0 = checkSumFunction("A;0;");
  //Serial.println(checkSumForEvent0); //72
}


void loop() {
  readLedSwc(labbraSinchroSwc);
  blinkDx = !digitalRead(blink_dx_btn_pin);
  blinkSx = !digitalRead(blink_sx_btn_pin);
  readJstick();
  handleSliders();
  deadManButton();
  delay(delayLoop);
}

void handleSliders() {
  for (int slider = 0; slider < howmanyanalog; slider++) {
    int servo_ch = listaMotori[slider].servoCh;

    if (labbraSinchroSwc.value && (servo_ch == 7 || servo_ch == 2|| servo_ch == 10)) {
      continue;  //skip loop if motors are mirrored
    }

    int sliderVal = analogRead(listaMotori[slider].arduinoPin);
    if (servo_ch ==12){
      if(blinkDx){
        sliderVal = 0;
      }else{
        sliderVal = right_eye;
      }
    }
    if (servo_ch ==13){
      if(blinkSx){
        sliderVal = 0;
      }else{
        sliderVal = left_eye;
      }
    }

    if (abs(sliderVal - listaMotori[slider].oldValue) > analogFilter) {
      sendMotor(listaMotori[slider], sliderVal);
      if (labbraSinchroSwc.value) {
        switch (servo_ch) {
          case 8:
            sendMotor(listaMotori[7], sliderVal);
            break;
          case 3:
            sendMotor(listaMotori[6], sliderVal );
            break;
          case 11:
            sendMotor(listaMotori[2], sliderVal );
            break;
        }
      }
    }
  }
}


void readJstick(){
  
  int udValue = analogRead(A1);
  int lrValue = analogRead(A0);
  // map the value to useful pwm-friendly ones
  int up = map(udValue, 0, 480,512, 0);
  int down = map(udValue, 520, 1023, 512, 0);
  int right = map(lrValue, 500, 0,0 ,512);
  int left = map(lrValue, 530, 1023, 0, 512);

  // correct the values in case of wrong pot limits on initialization and
  // so all of them will have the same weight in next computations
  right = constrain(right, 0, 512);
  left = constrain(left, 0, 512);
  up = constrain(up, 0, 512);
  down = constrain(down, 0, 512);
  // recorrect the data not to have more than max +PWM while non pivot-steering
  left_eye = constrain(down+up - right+ left, 0, 1023);
  right_eye = constrain(down+ up+ right - left, 0, 1023);
  //Serial.print(right_eye);
  //Serial.print(" ");
  //Serial.println(left_eye);
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
  Serial.print(m.servoCh);
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
