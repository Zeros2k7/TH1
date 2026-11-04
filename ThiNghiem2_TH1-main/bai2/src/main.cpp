#include <Arduino.h>
// Bai 2: Debounce bang millis() trong ISR. Mach giu nguyen Bai 1.
#define BUTTON_PIN 4
#define LED_PIN    2
#ifndef DEBOUNCE_MS
#define DEBOUNCE_MS 50
#endif

volatile uint32_t debouncedCount    = 0;  // so lan nhan THUC
volatile uint32_t rawEdgeCount      = 0;  // so suon xuong THO
volatile uint32_t lastInterruptTime = 0;

void IRAM_ATTR handleInterrupt() {
  rawEdgeCount++;
  uint32_t now = millis();
  if (now - lastInterruptTime >= DEBOUNCE_MS) {
    debouncedCount++;
    lastInterruptTime = now;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
  Serial.printf("Bai 2: DEBOUNCE_MS = %d\n", DEBOUNCE_MS);
}

void loop() {
  static uint32_t lastShown = 0;
  uint32_t d = debouncedCount, r = rawEdgeCount;
  if (d != lastShown) {
    lastShown = d;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // doi LED moi lan nhan that
    Serial.printf("raw=%lu  debounced=%lu\n", (unsigned long)r, (unsigned long)d);
  }
}
