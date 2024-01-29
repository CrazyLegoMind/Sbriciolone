const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'G';

const char earC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char snoutC = 'T';


struct ServoValues {
  bool reverse;  //reverse min and max control
  bool stopAndGo;
  int minValue = 4000;  //min micros*4
  int maxValue = 8000;  //max micros*4
  int channel;          //channel on the pololu maestro
  int lastPosition = 512;
  int counterShutDown;
  int shutDownWhen;
};


unsigned long ms_last_alive_sent = 0;
unsigned long ms_alive_spacing = 500;
const byte howmanyservo = 16;
ServoValues servoList[howmanyservo];
#include <PololuMaestro.h>


int aliveCounter = 0;
const byte aliveTrigger = 10;
bool standby = false;
/* On boards with a hardware serial port available for use, use
  that port to communicate with the Maestro. For other boards,
  create a SoftwareSerial object using pin 10 to receive (RX) and
  pin 11 to transmit (TX). */
#ifdef SERIAL_PORT_HARDWARE_OPEN
#define maestroSerial SERIAL_PORT_HARDWARE_OPEN
#else
#include <SoftwareSerial.h>
SoftwareSerial maestroSerial(10, 11);
#endif

#include "sbus.h"

bfs::SbusRx sbus_rx(&Serial2);
/* SBUS data */
bfs::SbusData data;
/* Next, create a Maestro object using the serial port.
  Uncomment one of MicroMaestro or MiniMaestro below depending
  on which one you have. */
//MicroMaestro maestro(maestroSerial);
MiniMaestro maestro(maestroSerial);

void setup() {

  /* setTarget takes the channel number you want to control, and
    the target position in units of 1/4 microseconds. A typical
    RC hobby servo responds to pulses between 1 ms (4000) and 2
    ms (8000). */
  maestroSerial.begin(115200);

  //NOME SERVO "Lab_D";
  servoList[0].minValue = 5184;
  servoList[0].maxValue = 6528;
  servoList[0].channel = 0;
  servoList[0].reverse = false;
  servoList[0].stopAndGo = false;
  servoList[0].shutDownWhen = 1000;

  //NOME SERVO "Naso";
  servoList[1].minValue = 3968;
  servoList[1].maxValue = 7168;
  servoList[1].channel = 1;
  servoList[1].reverse = false;
  servoList[1].stopAndGo = false;
  servoList[1].shutDownWhen = 1000;

  //NOME SERVO "Cig_C_Sx";
  servoList[2].minValue = 3968;
  servoList[2].maxValue = 6400;
  servoList[2].channel = 2;
  servoList[2].reverse = false;
  servoList[2].stopAndGo = false;
  servoList[2].shutDownWhen = 1000;

  //NOME SERVO "Cig_C_Dx";
  servoList[3].minValue = 4864;
  servoList[3].maxValue = 8000;
  servoList[3].channel = 3;
  servoList[3].reverse = true;
  servoList[3].stopAndGo = false;
  servoList[3].shutDownWhen = 1000;

  //NOME SERVO "Orecchie";
  servoList[4].minValue = 4672;
  servoList[4].maxValue = 7104;  //
  servoList[4].channel = 4;
  servoList[4].reverse = false;
  servoList[4].stopAndGo = false;
  servoList[4].shutDownWhen = 1000;

  //NOME SERVO "Lab_S";
  servoList[5].minValue = 5632;
  servoList[5].maxValue = 8000;
  servoList[5].channel = 5;
  servoList[5].reverse = true;
  servoList[5].stopAndGo = false;
  servoList[5].shutDownWhen = 1000;

  //NOME SERVO "Baf_S";
  servoList[6].minValue = 5184;
  servoList[6].maxValue = 8000;  //
  servoList[6].channel = 6;
  servoList[6].reverse = false;
  servoList[6].stopAndGo = false;
  servoList[6].shutDownWhen = 1000;

  //NOME SERVO "Cig_E_Sx";
  servoList[7].minValue = 3968;
  servoList[7].maxValue = 6080;
  servoList[7].channel = 7;
  servoList[7].reverse = false;
  servoList[7].stopAndGo = false;
  servoList[7].shutDownWhen = 1000;

  //NOME SERVO "Cig_E_Dx";
  servoList[8].minValue = 5632;
  servoList[8].maxValue = 8000;
  servoList[8].channel = 8;
  servoList[8].reverse = true;
  servoList[8].stopAndGo = false;
  servoList[8].shutDownWhen = 1000;

  //NOME SERVO "Baf_D";
  servoList[9].minValue = 4096;
  servoList[9].maxValue = 6912;
  servoList[9].channel = 9;
  servoList[9].reverse = true;
  servoList[9].stopAndGo = false;
  servoList[9].shutDownWhen = 1000;

  //NOME SERVO "Gua_S";
  servoList[10].minValue = 5696;
  servoList[10].maxValue = 7040;
  servoList[10].channel = 10;
  servoList[10].reverse = false;
  servoList[10].stopAndGo = false;
  servoList[10].shutDownWhen = 1000;

  //NOME SERVO "Gua_D";
  servoList[11].minValue = 5120;
  servoList[11].maxValue = 6528;
  servoList[11].channel = 11;
  servoList[11].reverse = true;
  servoList[11].stopAndGo = false;
  servoList[11].shutDownWhen = 1000;

  //NOME SERVO "Occ_D";
  servoList[12].minValue = 4608;
  servoList[12].maxValue = 5824;
  servoList[12].channel = 12;
  servoList[12].reverse = true;
  servoList[12].stopAndGo = false;
  servoList[12].shutDownWhen = 1000;

  //NOME SERVO "Occ_S";
  servoList[13].minValue = 6208;
  servoList[13].maxValue = 7360;
  servoList[13].channel = 13;
  servoList[13].reverse = false;
  servoList[13].stopAndGo = false;
  servoList[13].shutDownWhen = 1000;

  //NOME SERVO "Bocc_D";
  servoList[14].minValue = 3968;
  servoList[14].maxValue = 8000;
  servoList[14].channel = 14;
  servoList[14].reverse = true;
  servoList[14].stopAndGo = false;
  servoList[14].shutDownWhen = 1000;

  //NOME SERVO "Bocc_S";
  servoList[15].minValue = 3968;
  servoList[15].maxValue = 8000;
  servoList[15].channel = 15;
  servoList[15].reverse = false;
  servoList[15].stopAndGo = false;
  servoList[15].shutDownWhen = 1000;

  for (int i = 0; i < howmanyservo; i++)
    maestro.setTarget(servoList[i].channel, 0);
}

int sbus_to_ch_table[] = {
  0,   //0
  1,   //1
  2,   //2
  3,   //3
  4,   //4
  5,   //5
  6,   //6
  9,  //7
  12,  //8
  13,   //9
};

int msServoConversion(int msValue, ServoValues& servo,bool reverse = false) {
  int res = 0;
  if (msValue < 50) {
    return res;
  }
  
  res = map(msValue, 100, 2000, servo.minValue, servo.maxValue);
  if (servo.reverse != reverse)
    res = map(msValue, 100, 2000, servo.maxValue, servo.minValue);
  res = constrain(res, servo.minValue, servo.maxValue);
  return res;
}

void loop() {
  if (sbus_rx.Read()) {
    /* Grab the received data */
    data = sbus_rx.data();
    /* Display the received data */
    for (int8_t i = 0; i < 12; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
      switch (i) {
        case 0:
          maestro.setTarget(servoList[0].channel, msServoConversion(data.ch[i], servoList[0]));
          break;
        case 1:
        maestro.setTarget(servoList[1].channel, msServoConversion(data.ch[i], servoList[1]));
          break;
        case 2:
        maestro.setTarget(servoList[2].channel, msServoConversion(data.ch[i], servoList[2]));
        maestro.setTarget(servoList[7].channel, msServoConversion(data.ch[i], servoList[7]));
        maestro.setTarget(servoList[10].channel, msServoConversion(data.ch[i], servoList[10]));//rev
          break;
        case 3:
        maestro.setTarget(servoList[3].channel, msServoConversion(data.ch[i], servoList[3]));
        maestro.setTarget(servoList[8].channel, msServoConversion(data.ch[i], servoList[8]));
        maestro.setTarget(servoList[11].channel, msServoConversion(data.ch[i], servoList[11]));//rev
          break;
        case 4:
        maestro.setTarget(servoList[4].channel, msServoConversion(data.ch[i], servoList[4]));
          break;
        case 5:
        maestro.setTarget(servoList[5].channel, msServoConversion(data.ch[i], servoList[5]));
          break;
        case 6:
        maestro.setTarget(servoList[6].channel, msServoConversion(data.ch[i], servoList[6]));
          break;
        case 7:
        maestro.setTarget(servoList[9].channel, msServoConversion(data.ch[i], servoList[9]));
          break;
        case 8:
        maestro.setTarget(servoList[12].channel, msServoConversion(data.ch[i], servoList[12]));
          break;
        case 9:
        maestro.setTarget(servoList[13].channel, msServoConversion(data.ch[i], servoList[13]));
          break;
        case 10:
        maestro.setTarget(servoList[14].channel, msServoConversion(data.ch[i], servoList[14]));
        maestro.setTarget(servoList[15].channel, msServoConversion(data.ch[i], servoList[15]));
          break;
        case 11:
          break;
        default:
          break;
      }
    }
    /* Display lost frames and failsafe data */
    Serial.print(data.lost_frame);
    Serial.print("\t");
    Serial.println(data.failsafe);
  }
  deadManButton();
}


void deadManButton() {
  unsigned long current_time = millis();
  if (current_time - ms_last_alive_sent > ms_alive_spacing) {
    ms_last_alive_sent = current_time;
    Serial3.println("ALIVE");
  }
}
