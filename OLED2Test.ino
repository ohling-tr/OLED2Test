//OLED2Test.ino
// OLEDTest.ino
#include <U8g2lib.h> 
#include <Servo.h>

#define THROTTLE_0_PIN A0
int DIRCTN_SW_PIN = 2;
int ON_OFF_PIN = 4;

U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0);  
Servo Speed_cntl_0;
int I_THROTTLE_MAX = 500;
double D_THROTTLE_MAX = I_THROTTLE_MAX;
int iThrottle0Val = 0;
double dThrottle0Val = 0.0;
int iServoCenter = 1500;
int iServo0Val = iServoCenter;
int iDirctnVal = 0;
String sDirection = "Fwd";
int iOnSw = 0;
String sOnSw = "Off";

void setup(void) {
  u8g2.begin();
  Speed_cntl_0.attach(9);
  pinMode(DIRCTN_SW_PIN, INPUT_PULLUP);
  pinMode(ON_OFF_PIN, INPUT_PULLUP);
}

void loop(void) {
  
  /* 
  read speed control on/off switch
  set the appropriate literal
  and if speed control is off, read the direction switch
  DO NOT CHANGE MOTOR DIRECTION WHILE RUNNING
  */
  iOnSw = digitalRead(ON_OFF_PIN);
  if (iOnSw == LOW) {
    sOnSw = "Off";
    iDirctnVal = digitalRead(DIRCTN_SW_PIN);
  } else {
    sOnSw = "On";
  }

  /*
  read speed control potentiometer and convert to throttle range limit
  convert to double for calcs
  */
  iThrottle0Val = map(analogRead(THROTTLE_0_PIN),0,1023,0,I_THROTTLE_MAX);
  dThrottle0Val = iThrottle0Val;
 
  /*
  PWM speed control signal is in the range of 1000 to 2000
  where 1000 is full reverse, 1500 is stop, and 2000 is full forward
  use integers for the PWM control
  translate to range of 1 to -1 for use in WPILib speed range programming
  */
  if (iDirctnVal == LOW) {
    iServo0Val = iServoCenter + iThrottle0Val;
    dThrottle0Val = 0.0 - (dThrottle0Val / D_THROTTLE_MAX);
    sDirection = "Fwd";
  } else {
    iServo0Val = iServoCenter - iThrottle0Val;
    dThrottle0Val = 0.0 + (dThrottle0Val / D_THROTTLE_MAX);
    sDirection = "Rev";
  }
    
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0,15);
    String str1 = "S1 M1 ";
    u8g2.print(str1);
    u8g2.print(dThrottle0Val);
    u8g2.setCursor(0,32);
    u8g2.print(sDirection);
    u8g2.setCursor(0,48);
    u8g2.print(sOnSw);
  } while ( u8g2.nextPage() );
  if (sOnSw == "Off") {
    iServo0Val = iServoCenter;
  }
  Speed_cntl_0.writeMicroseconds(iServo0Val);
  //delay(200);
}