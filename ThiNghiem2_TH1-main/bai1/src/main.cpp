#include <Arduino.h>
#define BUTTON_PIN 4
#define LED_PIN    2
#define LED_FREQ   5000
#define LED_BITS   8     

#ifdef USE_INTERRUPT
volatile bool     buttonPressed = false;
volatile uint32_t pressCount    = 0;
volatile uint32_t lastIsrMs     = 0;

void IRAM_ATTR handleInterrupt() {          // ISR ngan gon: chi dem + dat co
  uint32_t now = millis();
  if (now - lastIsrMs >= 30) {              // loc doi toi thieu 30 ms
    pressCount++;
    lastIsrMs = now;
    buttonPressed = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  ledcAttach(LED_PIN, LED_FREQ, LED_BITS);  // Arduino-ESP32 core 3.x
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
  Serial.println("VD2: CO NGAT - nhan nut LED se chop ngay");
}

void loop() {
  static int brightness = 0, step = 1;
  if (buttonPressed) {
    buttonPressed = false;
    ledcWrite(LED_PIN, 0);   delay(80);     // nhay tat
    ledcWrite(LED_PIN, 255); delay(80);     // nhay sang
    Serial.printf("Nut nhan, so lan = %lu\n", (unsigned long)pressCount);
  }
  brightness += step;
  if (brightness <= 0 || brightness >= 255) step = -step;
  ledcWrite(LED_PIN, brightness);
  delay(10);
}

#else
// ================= VD1: KHONG NGAT (polling) =================
uint32_t pressCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  ledcAttach(LED_PIN, LED_FREQ, LED_BITS);
  Serial.println("VD1: KHONG NGAT - nut chi duoc doc sau moi chu ky fade (~5 s)");
}

void loop() {
  for (int b = 0; b <= 255; b++) { ledcWrite(LED_PIN, b); delay(10); }
  for (int b = 255; b >= 0; b--)  { ledcWrite(LED_PIN, b); delay(10); }
  // Chi doc nut 1 lan sau ca chu ky -> nhieu lan nhan bi BO LO
  if (digitalRead(BUTTON_PIN) == LOW) {
    pressCount++;
    Serial.printf("button pressed, so lan = %lu\n", (unsigned long)pressCount);
  }
}
#endif
