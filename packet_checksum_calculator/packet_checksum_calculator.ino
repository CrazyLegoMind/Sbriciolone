void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(1000);
  Serial.println(checkSumFunction("G;0;"));

}

int checkSumFunction(String SCS) {
  char bufferChar[SCS.length()];
  SCS.toCharArray(bufferChar, SCS.length());

  int sum = 0;
  for (int i = 0; i < SCS.length(); i++) {
    sum += bufferChar[i];
  }

  return sum % 100;
}