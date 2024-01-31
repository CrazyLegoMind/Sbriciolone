#include "sbus.h"
bfs::SbusTx sbus_tx(&Serial2, 25, 27, true);
/* SBUS data */
bfs::SbusData data;

unsigned long ms_last_alive_sent = 0;
unsigned long ms_alive_spacing = 500;

const char eventsC = 'e';
const char statusChangeC = 'C';
const char servoC = 'S';
const char globalC = 'G';

const char earC = 'E';
const char eyeLidsC = 'L';
const char eyebrownsC = 'B';
const char mouthC = 'M';
const char snoutC = 'T';

const byte howmanyservo = 12;

int ch_to_sbus_table[] = {
  0,   //0
  1,   //1
  2,   //2
  3,   //3
  4,   //4
  5,   //5
  6,   //6
  -1,  //7
  -1,  //8
  7,   //9
  -1,  //10
  -1,  //11
  8,   //12
  9,   //13
  10,  //14
  -1,  //15
};

void setup() {
  //Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 19, 5);
  Serial1.setTimeout(20);
  sbus_tx.Begin();
}
int lp = 0;

void loop() {
  String message = Serial1.readStringUntil('\n');
  if (message.length() > 0) {
    //Serial.println(message);
    if (message.charAt(0) == 'r') {
      message = message.substring(1);
    }
    //delay(30);
    int lenghtMessage = getLenghtBeforeCheckSum(message, ';');
    int numberSeparators = homManySeparator(message, ';');
    int checksum = getValueStringSplitter(message, ';', numberSeparators).toInt();

    char bufferChar[lenghtMessage];
    message.toCharArray(bufferChar, lenghtMessage);

    int sum = 0;
    for (int i = 0; i < lenghtMessage; i++) {
      sum += bufferChar[i];
    }

    int myCheckSum = sum % 100;
    if (myCheckSum == checksum) {
      char sector_c = message.charAt(0);
      switch (sector_c) {
        case earC:
          earMotorMessage(message);
          break;
        case eyeLidsC:
          eyelidsMessage(message);
          break;
        case eyebrownsC:
          eyeBrowMessage(message);
          break;
        case snoutC:
          noseMessage(message);
          break;
        case mouthC:
          mouthMessage(message);
          break;
        case globalC:
          globalMessage(message);
          break;
      }
    }
  }
  sbus_tx.data(data);
  sbus_tx.Write();
  delay(2);
  deadManButton();
}


void globalMessage(String message) {
  String indexString = getValueStringSplitter(message, ';', 1);
  int index = indexString.toInt();
  switch (index) {
    case 0:
      for (int i = 0; i < howmanyservo; i++) {
        data.ch[i] = 0;
      }
      break;
  }
}

void noseMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    index = ch_to_sbus_table[index];
    //Serial.println(index);
    if(index < 0) {
      return;
    }
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    data.ch[index] = analogToMsConversion(value);
  }
}

void eyelidsMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    index = ch_to_sbus_table[index];
    //Serial.println(index);
    if(index < 0) {
      return;
    }
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    data.ch[index] = analogToMsConversion(value);
  }
}



void eyeBrowMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    index = ch_to_sbus_table[index];
    //Serial.println(index);
    if(index < 0) {
      return;
    }
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    data.ch[index] = analogToMsConversion(value);
  }
}


void mouthMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    index = ch_to_sbus_table[index];
    //Serial.println(index);
    if(index < 0) {
      return;
    }
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    data.ch[index] = analogToMsConversion(value);
  }
}

void earMotorMessage(String message) {
  if (message.charAt(1) == servoC) {
    String indexString = getValueStringSplitter(message, ';', 1);
    int index = indexString.toInt();
    index = ch_to_sbus_table[index];
    //Serial.println(index);
    if(index < 0) {
      return;
    }
    String valueString = getValueStringSplitter(message, ';', 2);
    int value = valueString.toInt();
    data.ch[index] = analogToMsConversion(value);
  }
}

int analogToMsConversion(int analogValue) {
  int res = map(analogValue, 0, 1023, 100, 2000);
  res = constrain(res, 100, 2000);
  return res;
}

int homManySeparator(String data, char separator) {
  int s = 0;
  for (int i = 0; i < data.length(); i++)
    if (data.charAt(i) == separator)
      s++;
  return s;
}

int getLenghtBeforeCheckSum(String data, char separator) {
  int l = -1;
  for (int i = 0; i < data.length(); i++)
    if (data.charAt(i) == separator)
      l = i;
  return (l + 1);
}

String getValueStringSplitter(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void deadManButton() {
  unsigned long current_time = millis();
  if (current_time - ms_last_alive_sent > ms_alive_spacing) {
    ms_last_alive_sent = current_time;
    Serial1.println("ALIVE");
    //Serial.println("alive");
  }
}