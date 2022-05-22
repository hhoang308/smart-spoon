#include <Wire.h>
#include <Servo.h>
const int MPU_addr=0x68; // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float delta_t = 0.005;
float pitchAcc,rollAcc, pitch, roll, pitched;
float P_CompCoeff= 0.98;
// ============= INITIAL SETUP ===========================================
Servo myservo1, myservo2;
void setup(){
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B); // PWR_MGMT_1 register
Wire.write(0); // set to zero (wakes up the MPU-6050)
Wire.endTransmission(true);
Serial.begin(9600);
myservo1.attach(9);
myservo2.attach(10);
//myservo1 is the servo next to breadboard
//myservo2 is the others
//myservo1.write(100);
//myservo2.write(100);
}
// ============= MAIN LOOP ===============================================
void loop(){
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true); // request a total of 14 registers
AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
//Complementary filter
long squaresum_P=((long)GyY*GyY+(long)AcY*AcY);
long squaresum_R=((long)GyX*GyX+(long)AcX*AcX);
pitch+=((-AcY/40.8f)*(delta_t));
roll+=((-AcX/45.8f)*(delta_t)); //32.8 LSB
//Serial.print("\n pitch before filter = ");
//Serial.print(pitch);
//Serial.print("\nroll before filter = ");
//Serial.print(roll);
pitchAcc= atan((AcY/sqrt(squaresum_P))*RAD_TO_DEG);
rollAcc =atan((AcX/sqrt(squaresum_R))*RAD_TO_DEG);
pitch =(P_CompCoeff*pitch + (1.0f-P_CompCoeff)*pitchAcc);//pitch=P_CompCoeff*pitch + (1.0f-P_CompCoeff)*pitchAcc;
roll =(P_CompCoeff*roll + (1.0f-P_CompCoeff)*rollAcc);
//Serial.print("\nAcX = ");
//Serial.print(AcX);
//Serial.print("AcY = ");
//Serial.print(AcY);
//Serial.print(" AcZ = ");
//Serial.print(AcZ);
//Serial.print(" GyX = ");
//Serial.print(GyX);
//Serial.print(" GyY = ");
//Serial.print(GyY);
//Serial.print(" GyZ = ");
//Serial.print(GyZ);
//Serial.print("\npitchAcc after filter = ");
//Serial.print(pitchAcc);
//Serial.print("\nrollAcc after filter = ");
//Serial.print(rollAcc);
//Serial.print("\npitch after filter = ");
//Serial.print(pitch);
//Serial.print("\nroll after filter = ");
//Serial.print(roll);
//Serial.println();
myservo2.write(100 - pitch);
myservo1.write(100 + roll);
delay(10);
/*
if-statements to make the roll command go to where it is meant to go,
i.e clockwise/counterclockwise rotation
*/
//if (pitch < -158)
//{
//pitched = abs(pitch + 158);
//pitched = pitched - 158;
//}
//else if (pitch > -156)
//{
//pitched = abs(156 + pitch);
//pitched = -156 - pitched;
//}
////locked movement for upward direction of pitch
//if (pitched < -240)
//{
//pitched = -240;
//}
////Servo commands, roll/pitch + nr, where nr is compensation for mounting to start horizontally
//myservo1.write((roll + 120));
//myservo2.write(pitched + 340);
}
