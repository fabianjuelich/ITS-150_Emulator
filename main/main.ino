#include "signal.h"
#include <RCSwitch.h>

#define KEYS_ANAL A0
#define KEYS_DIGI 2
#define RF_TRANS  4

char keys[] = {'A', 'B', 'C'};
short keys_val[] = {0, 150, 300};
short keys_func[] = {1, -1, 0};
unsigned long last_time = 0;
RCSwitch mySwitch = RCSwitch();

void setup() {
  // Setup pins
  pinMode(KEYS_ANAL, INPUT_PULLUP);
  pinMode(KEYS_DIGI, INPUT);

  //Serial.begin(115200);

  // Setup RCSwitch
  mySwitch.enableTransmit(RF_TRANS);
  mySwitch.setRepeatTransmit(6);

  // disable ADC
  ADC_Enable(0);

  // Setup deep sleep (Power-down mode) and interrupt
  attachInterrupt(digitalPinToInterrupt(KEYS_DIGI), adc, FALLING);
  SMCR |= 1<<2; // mode 2
  SMCR |= 1; // sleep enable
  __asm__ __volatile__("sleep"); // inline assembly for setting to sleep
}

void loop() {}

void adc() {
  if ((micros() - last_time) < 250000) return;
  ADC_Enable(1);
  short digi = analogRead(KEYS_ANAL);
  //float anal = (float(digi)/1024) * 5;
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
  //Serial.println(digi);
  //Serial.println(keys[key]);
  if (keys[key] == 'A' || keys[key] == 'C') sendSignaltoAll(keys_func[key], mySwitch);
  else if (keys[key] == 'B') custom(mySwitch);
  ADC_Enable(0);
}

void ADC_Enable(bool val) {
  if (val) ADCSRA |= 1<<7;
  else ADCSRA &= ~(1<<7);
}
