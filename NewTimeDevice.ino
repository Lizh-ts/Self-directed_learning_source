#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AHTX0.h>
#include <LiquidCrystal_I2C.h>
Adafruit_AHTX0 aht;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4);
const int inputPin = 2;
int pushed = 0;
DateTime start;
void thetime(DateTime dt){
  lcd.setCursor(0, 0);
  lcd.print(dt.year());
  lcd.print('/');
  addzero(dt.month());
  lcd.print(dt.month());
  lcd.print('/');
  addzero(dt.day());
  lcd.print(dt.day());
  lcd.print("  ");
  addzero(dt.hour());
  lcd.print(dt.hour());
  lcd.print(':');
  addzero(dt.minute());
  lcd.print(dt.minute());
  lcd.print(':');
  addzero(dt.second());
  lcd.print(dt.second());
}
void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(inputPin, INPUT_PULLUP);
  //LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.setCursor(0, 3);
  //AHT10
  if (!aht.begin()) {
    lcd.print("AHT10 died");
    delay(10000);
  }
  //DS3231
  if (!rtc.begin()) {
    lcd.print("DS3231 died");
    delay(10000);
  }
  if (rtc.lostPower()) {
    lcd.setCursor(0, 3);
    Serial.println("元件掉電，初始化後輸入新時間");
    lcd.print("Time Reset");
    rtc.adjust(DateTime(2000, 1, 1, 0, 0, 0));
    Serial.println("YYYY MM DD HH MM SS");
    delay(10000);
  }
}

void loop() {
  //Line0 DATE TIME NOW
  DateTime now = rtc.now();
  thetime(now);
  //Line1 TIMER
  lcd.setCursor(0, 1);
  lcd.print("UPTIME");
  lcd.setCursor(12, 1);
  if(digitalRead(inputPin) == LOW){//PN2222A 接通
    if(pushed == 0){
      start = rtc.now();
    }
    pushed = 1;
    long useSeconds = now.unixtime() - start.unixtime();
    long hours = useSeconds / 3600;
    long minutes = (useSeconds % 3600) / 60;
    long seconds = useSeconds % 60;
    addzero(hours);
    lcd.print(hours);
    lcd.print(":");
    addzero(minutes);
    lcd.print(minutes);
    lcd.print(":");
    addzero(seconds);
    lcd.print(seconds);
  }else{
    pushed = 0;
  }
  //Line2 TEMPERATURE HUMIDITY
  lcd.setCursor(0, 2);
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  lcd.print("TEMP:");
  lcd.print((int)temp.temperature);
  lcd.print("C     ");
  lcd.print("RH:");
  lcd.print((int)humidity.relative_humidity);
  lcd.print("%");
  delay(500);
  //Time Update
  if (Serial.available()) {
    String input = Serial.readString();
    input.trim();
    int year, month, day, hour, minute, second;
    sscanf(input.c_str(), "%d %d %d %d %d %d", &year, &month, &day, &hour, &minute, &second);
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
    lcd.setCursor(0, 3);
    Serial.println("Updated");
  }
  if (now.hour()<=21 and now.hour()>=6) {
    lcd.backlight();
  }else{
    lcd.noBacklight();
  }
}
void addzero(int n) {
  if (n < 10) {
    lcd.print('0');
  }
}