#include "signal.h"
#include <RCSwitch.h>

#define KEYS_ANAL A0
#define KEYS_DIGI 2
#define RF_TRANS  9

char keys[] = {'A', 'B', 'C'};
short keys_val[] = {0, 128, 292};
short keys_func[] = {1, -1, 0};
unsigned long last_time = 0;
RCSwitch mySwitch = RCSwitch();

void setup() {
  pinMode(KEYS_ANAL, INPUT_PULLUP);
  pinMode(KEYS_DIGI, INPUT);
  attachInterrupt(digitalPinToInterrupt(KEYS_DIGI), adc, FALLING);
  Serial.begin(115200);
  mySwitch.enableTransmit(RF_TRANS);
}

void loop() {}

void adc() {
  if ((micros() - last_time) < 250000) return;
  short digi = analogRead(KEYS_ANAL);
  // float anal = (float(digi)/1024) * 5;
  short epsilon = pow(2, 10);
  char key;
  for (char k=0; k<(sizeof(keys)/sizeof(char)); k++) {
    short tmpEpsilon = abs(keys_val[k]-digi);
    if (tmpEpsilon < epsilon) {
      epsilon = tmpEpsilon;
      key = k;
    }
  }
  last_time = micros();
  // Serial.print(keys[key]);
  if (key == 'A' || key == 'C') sendSignaltoAll(keys_func[keys[key]], mySwitch);
  else custom();
}
