#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40); //board 1
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41); // board 2
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define SERVOMID  375 // servo middle position
#define SERVOPLAY1 350 // servo start of note position
#define SERVOPLAY2BOARD1  440 // servo end of note position for board 1
#define SERVOPLAY2BOARD2  260 // servo end of note position for board 2
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
int servosch1 = 10; // number of note playing servos in first channel (board)
int servosch2 = 10; // number of note playing servos in second channel (board)
Servo myServo; // crank servo


int servoOffset[] = {-5, 15, 0, 10, 0, -5, 5, 0, 5, 20, 0, 5, 10, 10, 0, 10, 17, 5, 30, 0}; //offset for each individual servo starting position
void setup() {
  Serial.begin(9600);
  pinMode(11,OUTPUT); // crank servo directly connected to arduino pin 11
  myServo.attach(11); 
  pwm1.begin();
  pwm2.begin();
  pwm1.setOscillatorFrequency(27000000);
  pwm2.setOscillatorFrequency(27000000);
  pwm1.setPWMFreq(SERVO_FREQ);
  pwm2.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  for(int i=0; i < servosch1; i++){ // initialize position for first board
    pwm1.setPWM(i, 0, (SERVOPLAY1 + servoOffset[i]));
  }
  for(int i=0; i < servosch2; i++){ // intitialize position for second board
    pwm2.setPWM(i, 0, (SERVOPLAY1 - servoOffset[i+10]));
  }
}
void playNote1(int servo){            // plays note on first servo board
  pwm1.setPWM(servo, 0, (SERVOPLAY2BOARD1 + servoOffset[servo]));
  delay(100);
  pwm1.setPWM(servo, 0, SERVOPLAY1);
}
void playNote2(int servo){            // plays note on second servo board
  pwm2.setPWM(servo, 0, (SERVOPLAY2BOARD2 - servoOffset[servo + 10]));
  delay(100);
  pwm2.setPWM(servo, 0, SERVOPLAY1);
}
void loop() {
  myServo.writeMicroseconds(1560);
  if(Serial.available() == 2){
    byte inByte1 = Serial.read();
    byte inByte2 = Serial.read();
    Serial.println(inByte1);
    Serial.println(inByte2);
    if(inByte1 < servosch1 && inByte2 > 0){  // pins falls on first board
      playNote1(inByte1);
    }
    if(inByte1 >= servosch1 && inByte2 > 0){  // pins on second board
      playNote2(inByte1 - servosch1);
    }
  }

}
