#include <AHT10.h>
#define BLYNK_TEMPLATE_ID "TMPL60CRzSzDb"
#define BLYNK_TEMPLATE_NAME "Project"

#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#define USE_NODE_MCU_BOARD

#include "BlynkEdgent.h"
#include <Wire.h>
#include "OneButton.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
AHT10 myAHT10(AHT10_ADDRESS_0X38);

int valueTH = 0;
int valueTHo = 0;
int nutnhanmotor = 13;
int auton = 2;
int controlmode = 2;
int controlmotor = 3;
int val_threshold = 75;
int val;
String dongco = "Tat";
String mode = "Auto";
boolean bt1_state = HIGH;
boolean bt2_state = HIGH;

BlynkTimer timer;

WidgetLED LEDCONNECT(V0);
OneButton buttonmotor(nutnhanmotor, true);
OneButton buttonauto(auton, true);

#define DongCo V4
#define DOAM V1
#define CheDo V6

BLYNK_WRITE(V2) {
  int p = param.asInt();
  if (p == 1) {
    controlmode = 2;
  } else if (p == 0) {
    controlmode = 1;
  }
}

BLYNK_WRITE(V3) {
  val_threshold = param.asInt();
}

BLYNK_WRITE(V5) {
  int p = param.asInt();
  if (p == 1) {
    controlmotor = 2;
  } else if (p == 0) {
    controlmotor = 1;
  }
}

void setup() {
  lcd.init();
  lcd.backlight();

  buttonmotor.attachLongPressStart(nhan_giu);
  buttonauto.attachLongPressStart(nhan_giu_dongco);
  pinMode(14, OUTPUT);
  Serial.begin(115200);
  myAHT10.begin();

  BlynkEdgent.begin();
  timer.setInterval(1000L, updateBlynk);
}

void loop() {
  BlynkEdgent.run();
  timer.run();
  
  val = analogRead(A0);
  val = int(val * 100 / 1024);
  valueTH = myAHT10.readTemperature();
  valueTHo = myAHT10.readHumidity();
    int buttonStatusmotor = digitalRead(nutnhanmotor);
    int buttonStatusauto = digitalRead(auton);
    buttonmotor.tick();
    buttonauto.tick();
    if (controlmode == 1) {
      if (controlmotor == 1) {
        digitalWrite(14, LOW);
      } else if (controlmotor == 2) {
        digitalWrite(14, HIGH);
      }
    } else if (controlmode == 2) {
      if (val < val_threshold) {
        digitalWrite(14, HIGH);
        controlmotor = 2;
      } else {
        digitalWrite(14, LOW);
        controlmotor = 1;
      }
    }
    displayLCD();
    delay(5);
  }

void nhan_giu() {
  if (controlmode == 2) {
    controlmode = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Khong tu dong");
    delay(700);
  } else if (controlmode == 1) {
    controlmode = 2;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tu dong");
    delay(700);
  }
}
void nhan_giu_dongco()
{
  if (controlmode == 1) {
  if(controlmotor == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bat dong co");
    delay(700);
    controlmotor = 2;
  }
  else if(controlmotor == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tat dong co");
    delay(700);
    controlmotor = 1;

  }
  }
}
void updateBlynk() {
  if (LEDCONNECT.getValue())
    LEDCONNECT.off();
  else
    LEDCONNECT.on();

  Blynk.virtualWrite(DOAM, val);
  Blynk.virtualWrite(V7, valueTH);
  Blynk.virtualWrite(V8, valueTHo);
  if (controlmode == 2) {
    mode = "Auto";
    Blynk.virtualWrite(DongCo, mode);
  } else if (controlmode == 1) {
    mode = "No-Auto";
    Blynk.virtualWrite(DongCo, mode);
  }
  if (controlmotor == 2) {
    dongco = "Bat";
    Blynk.virtualWrite(CheDo, dongco);
  } else if (controlmotor == 1) {
    dongco = "Tat";
    Blynk.virtualWrite(CheDo, dongco);
  }
}

void displayLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Do Am: ");
  lcd.setCursor(7, 0);
  lcd.print(val);
  lcd.print(" %RH");
  lcd.setCursor(0, 1);
  lcd.print(valueTH);
  lcd.print(" C");
  lcd.print(" ");
  lcd.print(round(valueTHo));
  lcd.print(" %RH");
}
