# TH1 - GPIO nang cao, ngat ngoai (ESP32)
Chay: `pio run -e <env> -t upload && pio device monitor` trong tung thu muc bai.
- bai1: env `polling` (VD1), `interrupt` (VD2)
- bai2: doi `-DDEBOUNCE_MS=5/50/500` trong platformio.ini
- bai3: env `adc1`, `adc2`, `adc2_wifi`
