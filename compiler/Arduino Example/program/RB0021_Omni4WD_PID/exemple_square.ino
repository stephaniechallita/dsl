
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <EEPROM.h>
#define _NAMIKI_MOTOR // for Namiki 22CL-103501PG80:1
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <MotorWheel.h>
#include <Omni4WD.h>

irqISR(irq1, isr1);
MotorWheel wheel1(3, 2, 4, 5, &irq1);
irqISR(irq2, isr2);
MotorWheel wheel2(11, 12, 14, 15, &irq2);
irqISR(irq3, isr3);
MotorWheel wheel3(9, 8, 16, 17, &irq3);
irqISR(irq4, isr4);
MotorWheel wheel4(10, 7, 18, 19, &irq4);
Omni4WD Omni(&wheel1, &wheel2, &wheel3, &wheel4);

int speed = 30;
long dist_accel;
long delaying;

void setup()
{
    TCCR1B = TCCR1B & 0xf8 | 0x01; // Pin9,Pin10 PWM 31250Hz
    TCCR2B = TCCR2B & 0xf8 | 0x01; // Pin3,Pin11 PWM 31250Hz

    Omni.PIDEnable(0.31, 0.01, 0, 10);
}

long senseDistance()
{
    long duration;
    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);
    delayMicroseconds(2);
    digitalWrite(6, HIGH);
    delayMicroseconds(5);
    digitalWrite(6, LOW);
    pinMode(6, INPUT);
    duration = pulseIn(6, HIGH);
    delay(100);

    return duration / 29 / 2 * 10;
}

void loop()
{
    entry();
}
void entry()
{
    speed = 150 / 5;
    long count = 0;
    long eval = 1;
    while ((count < 5))
    {
        count = (count + 1);
        square();
    }
}

void square()
{

    dist_accel = speed * 2.9940119760479043;
    delaying = (167 * (300 - dist_accel)) / speed;
    Omni.setCarAdvance(speed);
    Omni.setCarSpeedMMPS(speed, 500);
    delay(delaying);
    Omni.setCarSlow2Stop(500);
    delay(500);
    dist_accel = speed * 2.9940119760479043;
    delaying = (167 * ((90 * 2.4638813) - dist_accel)) / speed;
    Omni.setCarRotateRight(speed);
    Omni.setCarSpeedMMPS(speed, 500);
    delay(delaying);
    Omni.setCarSlow2Stop(500);
    delay(500);

    dist_accel = speed * 2.9940119760479043;
    delaying = (167 * (300 - dist_accel)) / speed;
    Omni.setCarAdvance(speed);
    Omni.setCarSpeedMMPS(speed, 500);
    delay(delaying);
    Omni.setCarSlow2Stop(500);
    delay(500);
    dist_accel = speed * 2.9940119760479043;
    delaying = (167 * ((90 * 2.4638813) - dist_accel)) / speed;
    Omni.setCarRotateRight(speed);
    Omni.setCarSpeedMMPS(speed, 500);
    delay(delaying);
    Omni.setCarSlow2Stop(500);
    delay(500);

    dist_accel = speed * 2.9940119760479043;
    delaying = (167 * (300 - dist_accel)) / speed;
    Omni.setCarAdvance(speed);
    Omni.setCarSpeedMMPS(speed, 500);
    delay(delaying);
    Omni.setCarSlow2Stop(500);
    delay(500);
    dist_accel = speed * 2.9940119760479043;
    delaying = (167 * ((90 * 2.4638813) - dist_accel)) / speed;
    Omni.setCarRotateRight(speed);
    Omni.setCarSpeedMMPS(speed, 500);
    delay(delaying);
    Omni.setCarSlow2Stop(500);
    delay(500);

    dist_accel = speed * 2.9940119760479043;
    delaying = (167 * (300 - dist_accel)) / speed;
    Omni.setCarAdvance(speed);
    Omni.setCarSpeedMMPS(speed, 500);
    delay(delaying);
    Omni.setCarSlow2Stop(500);
    delay(500);
    dist_accel = speed * 2.9940119760479043;
    delaying = (167 * ((90 * 2.4638813) - dist_accel)) / speed;
    Omni.setCarRotateRight(speed);
    Omni.setCarSpeedMMPS(speed, 500);
    delay(delaying);
    Omni.setCarSlow2Stop(500);
    delay(500);
}
