#include <SoftwareSerial.h>
#include <Wire.h>
# define I2C_SLAVE_ADDRESS 11

SoftwareSerial BT(2, 3); // RX, TX  NECESSÁRIO?

//LEFT
#define enB 13
#define in3 11
#define in4 12

//RIGHT
#define enA 8
#define in1 9
#define in2 10

bool flag1 = LOW;
boolean flagButton = false;

//char n[8] ;
char buf[9] = "";
String msg;
int numBytes;

int xAxis = 125, yAxis = 125;

int motorSpeedA = 0;
int motorSpeedB = 0;

void setup() {

  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(9600);
  Serial.println("-------------------------------------I am Slave1");

  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);


  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //delay(500);
}

void loop() {
  // Default value - no movement when the Joystick stays in the center
  //xAxis = 140;
  //yAxis = 140;

  // RECEÇÃO DE DADOS DO MASTER -- A FAZER
  // COMO FAZER? MASTER - SLAVE: Y___ + X___ ? || APP - MASTER: Y___ + X___ ?

  //  while (BT.available() >= 2) {
  //    xAxis = BT.read();
  //    delay(10);
  //    yAxis = BT.read();
  //    Serial.print(xAxis);
  //    Serial.print(",");
  //    Serial.println(yAxis);
  //  }

  if (flag1 == HIGH) {

    Serial.println(F("---> recieved events"));
    //buf[9] = Wire.read();
    Serial.print(numBytes);
    Serial.println(F("bytes recieved"));
    Serial.print(F("recieved value : "));
    //    Serial.println(n);
    Serial.println(buf);
    msg = String(buf);
    Serial.println(msg);

    int y_pos = msg.indexOf("Y");
    String y_value = msg.substring (y_pos + 1);
    int x_pos = msg.indexOf("X");
    String x_value = msg.substring (x_pos + 1, y_pos );

    xAxis = x_value.toInt();
    yAxis = y_value.toInt();

    delay(10);

    flag1 = LOW;
  }
  // Makes sure we receive corrent values

  if (xAxis > 115 && xAxis < 135 && yAxis > 115 && yAxis < 135) {
    Stop();
  }


  if (yAxis > 115 && yAxis < 135) {


    // 130 - 115 | 60 - ?
    if (xAxis < 115) {
      turnRight();
      motorSpeedA = map(xAxis, 115, 54, 0, 255);
      motorSpeedB = map(xAxis, 115, 54, 0, 255);
    }

    if (xAxis > 135) {
      turnLeft();
      motorSpeedA = map(xAxis, 135, 197, 0, 255);
      motorSpeedB = map(xAxis, 135, 197, 0, 255);
    }

  } else {

    if (xAxis > 115 && xAxis < 135) {

      if (yAxis < 115) {
        forword();
      }
      if (yAxis > 135) {
        backword();
      }

      if (yAxis < 115) {
        motorSpeedA = map(yAxis, 115, 54, 0, 255);
        motorSpeedB = map(yAxis, 115, 54, 0, 255);
      }

      if (yAxis > 135) {
        motorSpeedA = map(yAxis, 135, 197, 0, 255);
        motorSpeedB = map(yAxis, 135, 197, 0, 255);
      }

    } else {

      if (yAxis < 115) {
        forword();
      }
      if (yAxis > 135) {
        backword();
      }

      if (xAxis < 115) {
        motorSpeedA = map(xAxis, 115, 54, 255, 50);
        motorSpeedB = 255;
      }

      if (xAxis > 135) {
        motorSpeedA = 255;
        motorSpeedB = map(xAxis, 135, 197, 255, 50);
      }

    }
  }

  //Serial.print(motorSpeedA);
  //Serial.print(",");
  //Serial.println(motorSpeedA);

  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A1
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B1
}


void forword() {
  Serial.println("forword");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void backword() {
  Serial.println("backword");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  Serial.println("turnRight");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  Serial.println("turnLeft");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("stop");
}

void requestEvents() {

  if (flagButton == false)
  {
    Wire.write(' ');
    Serial.println("no call");
  }
}

void receiveEvents(int howMany) {

  for (int i = 0; i < howMany; i++) {
    buf[i] = Wire.read();
  }
  flag1 = HIGH;

}
