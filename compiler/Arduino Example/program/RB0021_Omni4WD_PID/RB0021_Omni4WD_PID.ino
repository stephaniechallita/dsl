#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <EEPROM.h>
#define _NAMIKI_MOTOR	 //for Namiki 22CL-103501PG80:1
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <MotorWheel.h>
#include <Omni4WD.h>

//#include <fuzzy_table.h>
//#include <PID_Beta6.h>

/*

            \                    /
   wheel1   \                    /   wheel4
   Left     \                    /   Right


                              power switch

            /                    \
   wheel2   /                    \   wheel3
   Right    /                    \   Left

*/

/*
  irqISR(irq1,isr1);
  MotorWheel wheel1(5,4,12,13,&irq1);

  irqISR(irq2,isr2);
  MotorWheel wheel2(6,7,14,15,&irq2);

  irqISR(irq3,isr3);
  MotorWheel wheel3(9,8,16,17,&irq3);

  irqISR(irq4,isr4);
  MotorWheel wheel4(10,11,18,19,&irq4);
*/

irqISR(irq1, isr1);
MotorWheel wheel1(3, 2, 4, 5, &irq1);

irqISR(irq2, isr2);
MotorWheel wheel2(11, 12, 14, 15, &irq2);

irqISR(irq3, isr3);
MotorWheel wheel3(9, 8, 16, 17, &irq3);

irqISR(irq4, isr4);
MotorWheel wheel4(10, 7, 18, 19, &irq4);


Omni4WD Omni(&wheel1, &wheel2, &wheel3, &wheel4);

const int pingPin = 6;

void setup() {
  //TCCR0B=TCCR0B&0xf8|0x01;    // warning!! it will change millis()
  TCCR1B = TCCR1B & 0xf8 | 0x01; // Pin9,Pin10 PWM 31250Hz
  TCCR2B = TCCR2B & 0xf8 | 0x01; // Pin3,Pin11 PWM 31250Hz

  Omni.PIDEnable(0.31, 0.01, 0, 10);

  //Serial.begin(9600);

}

void loop() {
  //Omni.demoActions(30,1500,500,false);
  
  //long dist = senseDistance();
  //if(dist < 40){
    //Omni.setCarSlow2Stop(500);
   // Omni.setCarRotateLeft(30);
    //Omni.setCarSpeedMMPS(30, 500);
  //} else {
  //  Omni.setCarAdvance(30);
   // Omni.setCarSpeedMMPS(30, 500);
  //}
  //delay(500);

  avanceRobotDeMerde(400);
  delay(500);
  tourneRobotDeMerde(90);
  delay(500);
  avanceRobotDeMerde(400);
  delay(500);
  tourneRobotDeMerde(90);
  delay(500);
  avanceRobotDeMerde(400);
  delay(500);
  tourneRobotDeMerde(90);
  delay(500);
  avanceRobotDeMerde(400);
  delay(500);
  tourneRobotDeMerde(90);
  delay(5000);
  // &Omni4WD::setCarAdvance,
  // 	&Omni4WD::setCarBackoff,
  // 	&Omni4WD::setCarLeft,
  // 	&Omni4WD::setCarRight,
  // 	&Omni4WD::setCarUpperLeft,
  // 	&Omni4WD::setCarLowerRight,
  // 	&Omni4WD::setCarLowerLeft,
  // 	&Omni4WD::setCarUpperRight,
  // 	&Omni4WD::setCarRotateLeft,
  // 	&Omni4WD::setCarRotateRight

}

void avanceRobotDeMerde(long mm){
  long t = 500;
  long v = 30;
  long dist_accel = (t*v) / 167;
  long remaining = mm - dist_accel;
  long delaying = 167*remaining / v;
  //Serial.print(dist_accel);
  //Serial.print(" mm, ");
  //Serial.print(delaying);
  //Serial.print(" ms");
  //Serial.println();
  
  Omni.setCarAdvance(v);
  Omni.setCarSpeedMMPS(v, t);
  delay(delaying);
  Omni.setCarSlow2Stop(t);
}

void tourneRobotDeMerde(long angle){
  double mmbydeg = 2.4638813;
  long t = 500;
  long v = 30;
  long dist_accel = (t*v) / 167;
  long remaining = (angle*mmbydeg) - dist_accel;
  long delaying = 167*remaining / v;
  //Serial.print(remaining);
  //Serial.print(" mm, ");
  //Serial.print(delaying);
  //Serial.print(" ms");
  //Serial.println();
  
  Omni.setCarRotateLeft(v);
  Omni.setCarSpeedMMPS(v, t);
  delay(delaying);
  Omni.setCarSlow2Stop(t);
}

long senseDistance(){
  long duration;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  delay(100);

  // convert the time into a distance
  return microsecondsToCentimeters(duration);
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
