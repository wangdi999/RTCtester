// 实验2：DS1307 实时时钟 + TM1637 显示“组号:分钟” + 串口带组号
#include "TM1637.h"
#include "RTClib.h"

#define CLK 10
#define DIO 9
#define GROUP_NUM 91     // 你们组的组号（0-99）

TM1637 tm1637(CLK, DIO);
RTC_DS1307 RTC;

void setup() {
  Serial.begin(9600);

  if (!RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running, set to compile time");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  tm1637.init();
  tm1637.set(7);          // 亮度 7（最亮，便于实验观察）
  tm1637.point(POINT_ON); // 点亮中间的冒号

  Serial.print("Group ");
  Serial.println(GROUP_NUM);
}

void loop() {
  DateTime now = RTC.now();
  int group = GROUP_NUM % 100;
  int minute = now.minute();

  // 左两位 = 组号，右两位 = 分钟，中间冒号 => “组号:分钟”
  tm1637.display(0, group / 10);
  tm1637.display(1, group % 10);
  tm1637.display(2, minute / 10);
  tm1637.display(3, minute % 10);
  tm1637.point(POINT_ON);

  // 串口数据前加组号
  Serial.print("Group ");
  Serial.print(GROUP_NUM);
  Serial.print(": ");
  Serial.print(now.year());
  Serial.print('-');
  if (now.month() < 10) Serial.print('0');
  Serial.print(now.month());
  Serial.print('-');
  if (now.day() < 10) Serial.print('0');
  Serial.print(now.day());
  Serial.print(' ');
  if (now.hour() < 10) Serial.print('0');
  Serial.print(now.hour());
  Serial.print(':');
  if (now.minute() < 10) Serial.print('0');
  Serial.print(now.minute());
  Serial.print(':');
  if (now.second() < 10) Serial.print('0');
  Serial.println(now.second());

  delay(1000);
}
