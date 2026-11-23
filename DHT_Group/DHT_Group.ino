// 实验1 档位版：<26°C 慢档，26~28.9°C 中档，>=29°C 快档
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define LED_PIN 13       // 板载 LED；外接 LED 可改到 9（支持 PWM）
#define GROUP_NUM 91     // 你们组的组号

// 档位分界温度
const float TEMP_MID = 27.0;
const float TEMP_FAST = 30.0;

// 三档闪烁间隔
const unsigned long INTERVAL_SLOW = 2000;
const unsigned long INTERVAL_MID = 1000;
const unsigned long INTERVAL_FAST = 10;

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastRead = 0;
unsigned long lastBlink = 0;
unsigned long ledOnAt = 0;
unsigned long blinkInterval = INTERVAL_SLOW;
bool ledOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  Serial.print("Group ");
  Serial.println(GROUP_NUM);
}

void loop() {
  unsigned long now = millis();

  if (now - lastRead >= 2000) {
    lastRead = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.print("Group ");
      Serial.print(GROUP_NUM);
      Serial.println(": DHT read failed");
    } else {
      int gear;

      if (t >= TEMP_FAST) {
        gear = 2;
        blinkInterval = INTERVAL_FAST;
      } else if (t >= TEMP_MID) {
        gear = 1;
        blinkInterval = INTERVAL_MID;
      } else {
        gear = 0;
        blinkInterval = INTERVAL_SLOW;
      }

      const char *gearName = (gear == 2) ? "FAST" : (gear == 1) ? "MID" : "SLOW";

      Serial.print("Group ");
      Serial.print(GROUP_NUM);
      Serial.print(": Temp=");
      Serial.print(t, 1);
      Serial.print(" C, Humidity=");
      Serial.print(h, 1);
      Serial.print(" %, Gear=");
      Serial.print(gear);
      Serial.print("(");
      Serial.print(gearName);
      Serial.print(")  [");

      // 串口温度条：23~32°C，每 1°C 一格
      int bars = constrain((int)(t - 23), 0, 10);
      for (int i = 0; i < bars; i++) Serial.print('#');
      for (int i = bars; i < 10; i++) Serial.print(' ');
      Serial.println("]");
    }
  }

  // 每 blinkInterval 亮 60ms，形成清晰的“闪一下”
  if (!ledOn && now - lastBlink >= blinkInterval) {
    lastBlink = now;
    ledOnAt = now;
    ledOn = true;
    digitalWrite(LED_PIN, HIGH);
  }
  if (ledOn && now - ledOnAt >= 60) {
    ledOn = false;
    digitalWrite(LED_PIN, LOW);
  }
}
