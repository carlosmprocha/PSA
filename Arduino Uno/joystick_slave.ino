#include <SoftwareSerial.h>
#include <Wire.h>
# define I2C_SLAVE_ADDRESS 11

SoftwareSerial BT(2, 3); // RX, TX  NECESSÁRIO?

//ANTES DE CADA #define DIZER QUAL A POSIÇÃO TIPO BACK LEFT / FRONT RIGHT

#define enB 11
#define in3 13
#define in4 12

#define enA 10
#define in1 9
#define in2 8

#define enB1 3
#define in31 2
#define in41 4

#define enA1 5
#define in11 6
#define in21 7

int n = 0;
int a = 0;
int b = 0;
int numBytes;

int xAxis=140, yAxis=140;

int motorSpeedA = 0;
int motorSpeedB = 0;

void setup() {
  
Wire.begin(I2C_SLAVE_ADDRESS);
Serial.begin(9600);
Serial.println("-------------------------------------I am Slave1");  

//Wire.onRequest(requestEvents);
//Wire.onReceive(receiveEvents);
  
pinMode(enA, OUTPUT);
pinMode(enB, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
pinMode(enA1, OUTPUT);
pinMode(enB1, OUTPUT);
pinMode(in11, OUTPUT);
pinMode(in21, OUTPUT);
pinMode(in31, OUTPUT);
pinMode(in41, OUTPUT);

delay(500);
}

void loop() {
  // Default value - no movement when the Joystick stays in the center
  //xAxis = 140;
  //yAxis = 140;

  // RECEÇÃO DE DADOS DO MASTER -- A FAZER
  // COMO FAZER? MASTER - SLAVE: Y___ + X___ ? || APP - MASTER: Y___ + X___ ?
  while (BT.available() >= 2) {
    xAxis = BT.read();
    delay(10);
    yAxis = BT.read();
   Serial.print(xAxis);
   Serial.print(",");
   Serial.println(yAxis);
  }
  
  delay(10);
  
  // Makes sure we receive corrent values

if (xAxis > 130 && xAxis <150 && yAxis > 130 && yAxis <150){Stop();}


if (yAxis > 130 && yAxis <150){    

if (xAxis < 130){turnRight();
motorSpeedA = map(xAxis, 130, 60, 0, 255);
motorSpeedB = map(xAxis, 130, 60, 0, 255);    
}

if (xAxis > 150) {turnLeft();
motorSpeedA = map(xAxis, 150, 220, 0, 255);
motorSpeedB = map(xAxis, 150, 220, 0, 255); 
}

}else{

if (xAxis > 130 && xAxis <150){   

if (yAxis < 130){forword();}
if (yAxis > 150){backword();}

if (yAxis < 130){
motorSpeedA = map(yAxis, 130, 60, 0, 255);
motorSpeedB = map(yAxis, 130, 60, 0, 255); 
}

if (yAxis > 150){
motorSpeedA = map(yAxis, 150, 220, 0, 255);
motorSpeedB = map(yAxis, 150, 220, 0, 255);
 }
 
}else{

if (yAxis < 130){forword();}
if (yAxis > 150){backword();}

if (xAxis < 130){
motorSpeedA = map(xAxis, 130, 60, 255, 50);
motorSpeedB = 255; 
 }
 
if (xAxis > 150){
motorSpeedA = 255;
motorSpeedB = map(xAxis, 150, 220, 255, 50); 
  }
  
 } 
}

   //Serial.print(motorSpeedA);
   //Serial.print(",");
   //Serial.println(motorSpeedA);

analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
analogWrite(enA1, motorSpeedA); // Send PWM signal to motor A1
analogWrite(enB1, motorSpeedB); // Send PWM signal to motor B1
}


void forword(){Serial.println("forword");
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW); 
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
digitalWrite(in11, LOW);
digitalWrite(in21, HIGH); 
digitalWrite(in31, HIGH);
digitalWrite(in41, LOW);
}

void backword(){Serial.println("backword");
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH); 
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
digitalWrite(in11, HIGH);
digitalWrite(in21, LOW); 
digitalWrite(in31, LOW);
digitalWrite(in41, HIGH);                
}

//A ASSOCIAR INPUTS CORRETOS
void turnRight(){Serial.println("turnRight");
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
digitalWrite(in11, HIGH);
digitalWrite(in21, LOW); 
digitalWrite(in31, HIGH);
digitalWrite(in41, LOW);                 
}

//A ASSOCIAR INPUTS CORRETOS
void turnLeft(){Serial.println("turnLeft");
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH); 
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
digitalWrite(in11, HIGH);
digitalWrite(in21, LOW); 
digitalWrite(in31, HIGH);
digitalWrite(in41, LOW);                 
}

void Stop(){
digitalWrite(in1, LOW);
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
digitalWrite(in11, LOW);
digitalWrite(in21, LOW); 
digitalWrite(in31, LOW);
digitalWrite(in41, LOW);
Serial.println("stop");
}



