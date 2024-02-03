const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'G';

const char earC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char snoutC = 'T';

const byte analogFilter = 5;
#define DELAY_LOOP 5
#define DELAY_SERVO 2
#define INPUT_CHANNELS 4


unsigned long ms_last_alive_sent = 0;
unsigned long ms_alive_spacing = 300;

enum in_type { ANL,
               PLP,
               DIG };

struct in_ch_t {
  in_type type = ANL;
  int arduinoPin;
  int val = 0;
  int servoCh;
};

struct servo_t {
  char sectorC;
  char eventC = servoC;
  int servoCh = 0;
  int sentValue = 0;
};

struct LedSwc {
  int pin;
  int led;
  char sectorC;
  char eventC = statusChangeC;
  boolean value;
};

in_ch_t inputList[INPUT_CHANNELS];
servo_t servoList[8];

int up = 0;
int down = 0;
int right = 0;
int left = 0;
int side_left = 0;
int side_right = 0;

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
  Serial.begin(115200);
  Serial1.begin(115200);

  inputList[0].arduinoPin = A0;  //face y
  inputList[0].servoCh = 12;

  inputList[1].arduinoPin = A1;  //face z
  inputList[1].servoCh = 13;

  inputList[2].arduinoPin = A2;  // eyelids
  inputList[2].servoCh = 10;

  inputList[3].arduinoPin = 3;  //simmetry_toggle
  inputList[3].type = PLP;
  inputList[3].servoCh = 8;

  for (char in = 0; in < INPUT_CHANNELS; in++) {
    in_ch_t ch = inputList[in];
    if (ch.type == PLP) {
      pinMode(ch.arduinoPin, INPUT_PULLUP);
    }
  }

  servoList[0].sectorC = eyeLidsC;  //occhiodx
  servoList[0].servoCh = 12;

  servoList[1].sectorC = eyeLidsC;  // occhiosx
  servoList[1].servoCh = 13;

  servoList[2].sectorC = eyebrownsC;  //guasx
  servoList[2].servoCh = 10;

  servoList[3].sectorC = eyebrownsC;  //guadx
  servoList[3].servoCh = 11;

  servoList[4].sectorC = eyebrownsC;  //edx
  servoList[4].servoCh = 8;

  servoList[5].sectorC = eyebrownsC;  //cdx
  servoList[5].servoCh = 3;

  servoList[6].sectorC = eyebrownsC;  //csx
  servoList[6].servoCh = 2;

  servoList[7].sectorC = eyebrownsC;  //esx
  servoList[7].servoCh = 7;
}


void loop() {
  handleInputs();
  deadManButton();
  delay(DELAY_LOOP);
}

void handleInputs() {
  for (short in = 0; in < INPUT_CHANNELS; in++) {
    in_ch_t ch = inputList[in];
    int value = 0;
    Serial.print(in);
    Serial.print(": ");
    switch (ch.type) {
      case ANL:
        value = analogRead(ch.arduinoPin);
        break;
      case PLP:
        value = !digitalRead(ch.arduinoPin);
        break;
    }
    Serial.print(value);
    Serial.print(", ");
    if (ch.type == ANL && abs(value - ch.val) < analogFilter) {
      continue;
    } else if (ch.type == PLP && value == ch.val) {
      continue;
    }
    inputList[in].val = value;
    switch (in) {
      case 0:
        up = map(value, 0, 480, 512, 0);
        down = map(value, 520, 1023, 512, 0);
        up = constrain(up, 0, 512);
        down = constrain(down, 0, 512);
        break;
      case 1:
        right = map(value, 480, 0, 0, 512);
        left = map(value, 520, 1023, 0, 512);
        right = constrain(right, 0, 512);
        left = constrain(left, 0, 512);
        break;
      case 2:
        sendServo(servoList[0],value);
        if(inputList[3].val == 0)
          sendServo(servoList[1],value);
        else
          sendServo(servoList[1],1023-value);
        break;
      case 3:
        break;
    }
  }
  int side_l_n = constrain(down + up - right + left, 0, 1023);
  int side_r_n = constrain(down + up + right - left, 0, 1023);
  if(abs(side_left-side_l_n ) > analogFilter){
    side_left = side_l_n;
    sendServo(servoList[2],1023-side_left);
    sendServo(servoList[4],side_left);
    sendServo(servoList[5],side_left);
  }
  if(abs(side_right-side_r_n ) > analogFilter){
    side_right = side_r_n;
    sendServo(servoList[3],1023-side_right);
    sendServo(servoList[6],side_right);
    sendServo(servoList[7],side_right);
  }
  Serial.print(side_left);
  Serial.print(" ");
  Serial.print(side_right);
  Serial.println();
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


void sendServo(servo_t& m, int val) {
  String SCS = "";
  SCS += m.sectorC;
  SCS += m.eventC;
  SCS += ';';
  SCS += m.servoCh;
  SCS += ';';
  SCS += val;
  SCS += ';';
  char bufferChar[SCS.length()];
  SCS.toCharArray(bufferChar, SCS.length());

  int sum = 0;
  for (int i = 0; i < SCS.length(); i++) {
    sum += bufferChar[i];
  }
  int checkSum = sum % 100;
  Serial1.print(m.sectorC);
  Serial1.print(m.eventC);
  Serial1.print(';');
  Serial1.print(m.servoCh);
  Serial1.print(';');
  Serial1.print(val);
  Serial1.print(';');
  Serial1.println(checkSum);
  delay(DELAY_SERVO);
  m.sentValue = val;
}

void deadManButton() {
  unsigned long current_time = millis();
  if (current_time - ms_last_alive_sent > ms_alive_spacing) {
    ms_last_alive_sent = current_time;
    Serial1.println("ALIVE");
  }
}