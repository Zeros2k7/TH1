#include <Arduino.h>
#ifdef USE_WIFI
#include <WiFi.h>
#endif
// Bai 3: ADC 12-bit + trung binh truot + LEDC
// Bien tro: 3V3 - wiper - GND ; LED -> GPIO18 (qua 220R)
#ifdef USE_ADC2
#define ADC_PIN 15   // ADC2_CH3 (strapping pin!)
#else
#define ADC_PIN 34   // ADC1_CH6 (input-only)
#endif
#define LED_PIN 18
#define N 8

int buf[N]; int idx = 0; bool primed = false;

int loc(int m) {
  if (!primed) { for (int i = 0; i < N; i++) buf[i] = m; primed = true; }
  buf[idx] = m; idx = (idx + 1) % N;
  long t = 0; for (int i = 0; i < N; i++) t += buf[i];
  return t / N;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  ledcAttach(LED_PIN, 5000, 8);
#ifdef USE_WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin("test", "test12345");   // bat Wi-Fi de thay ADC2 hong
#endif
}

void loop() {
  int tho = analogRead(ADC_PIN);
  int l = loc(tho);
  ledcWrite(LED_PIN, map(l, 0, 4095, 0, 255));
  Serial.print(">tho:"); Serial.println(tho);
  Serial.print(">loc:"); Serial.println(l);
  delay(20);
}
