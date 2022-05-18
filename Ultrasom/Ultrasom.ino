#include<Servo.h>

int trig = 8;
int echo = 9;
int dt = 10;
 int flag=0;
Servo servo;

//int distance,duration;
void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
  servo.attach(11);
}

void loop() {
//25-165
 
  if (flag == 0) {
    for (int p = 25; p <= 165; p = p + 10)
    {
      servo.write(p);
      delay(1);

      calc_dis();
      flag = 1;
      delay(100);
    }
  }
  if (flag == 1) {
    for (int p = 165; p >= 25; p = p - 10)
    {
      servo.write(p);
      delay(1);

      calc_dis();
      flag = 0;
      delay(100);
    }
  }
}

//This code is written to calculate the DISTANCE using ULTRASONIC SENSOR

int calc_dis()
{
  int duration, distance;
  digitalWrite(trig, HIGH);
  delay(dt);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.println(distance);
  return distance;
}
