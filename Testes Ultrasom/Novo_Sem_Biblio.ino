//NewPing Library https://github.com/livetronic/Arduino-NewPing//
//Servo Library https://github.com/arduino-libraries/Servo.git //

//#include <NewPing.h>
#include <Servo.h>

// -----CONFIRMAR--------
// Pinos
#define TRIG_PIN 4
#define ECHO_PIN 7
#define ME 6            // Pino_Velocidade Motor Esquerda ( 0 a 255);
#define MD 5             //Pino_Velocidade Motor Direita ( 0 a 255);
#define dirE 12           //Pino_Direção do Motor Esquerdo: Para frente / Para trás (HIGH ou LOW);
#define ndirE 11
#define dirD 8          //Pino_Direção do Motor Direito: Para frente / Para trás (HIGH ou LOW);
#define ndirD 13

// Constantes
#define MAX_DISTANCE 300
#define MIN_DISTANCE 15
#define MIN_DISTANCE_INT 11
#define MAX_DISTANCE_INT 25
#define OBST_DISTANCE 50

#define MAX_SPEED 100
#define MAX_SPEED_OFFSET 10
#define MIN_SPEED 60
#define speedTurn 250

#define timeTurn 100
#define timeStop 100
#define timeReverse 100

#define MIN_TURN 20
#define MAX_TURN 145
#define SERVO_CENTRO 60



//NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo servo;

boolean goesForward = false;
boolean obstacle = false;
int distance = 100;
int speedSet = 0;

void setup() {

  Serial.begin(9600);

  pinMode(ME, OUTPUT);
  pinMode(MD, OUTPUT);
  pinMode(dirE, OUTPUT);
  pinMode(dirD, OUTPUT);
  pinMode(ndirE, OUTPUT);
  pinMode(ndirD, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servo.attach(10);


  //------- O QUE FAZ? ----------

  servo.write(SERVO_CENTRO);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
  int distanceR = 0;
  int distanceL =  0;
  delay(40);

  //------------ SEGUIMENTO PAREDE-----------

  if (obstacle == true) {
    
    Serial.println("OBST");
    moveStop();
    delay(timeStop);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if (distanceR >= distanceL && distanceL <= MIN_DISTANCE_INT) {
      Serial.println("TURNR");
      turnRight();
      moveStop();
    }

    else if (distanceR >= distanceL && distanceL >= MAX_DISTANCE_INT) {
      Serial.println("TURNL");
      turnLeft();
      moveStop();
    }

    else if (distanceR <= distanceL && distanceL <= MIN_DISTANCE_INT) {
      turnLeft();
      moveStop();
    }

    else if (distanceR <= distanceL && distanceL >= MAX_DISTANCE_INT) {
      turnRight();
      moveStop();
    }
    else {
      moveForward();
    }
    // --------- IF BB ----------

  }

  else if (distance <= MIN_DISTANCE && obstacle == false) {

    obstacle = true;
    moveStop();
    delay(timeStop);
    moveBackward();
    delay(timeReverse);
    moveStop();
    delay(timeStop);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if (distanceR >= distanceL && distanceL <= MIN_DISTANCE_INT) {
      turnRight();
      moveStop();
    }

    else if (distanceR >= distanceL && distanceL >= MAX_DISTANCE_INT) {
      turnLeft();
      moveStop();
    }

    else if (distanceR <= distanceL && distanceL <= MIN_DISTANCE_INT) {
      turnLeft();
      moveStop();
    }

    else if (distanceR <= distanceL && distanceL >= MAX_DISTANCE_INT) {
      turnRight();
      moveStop();
    }
    else {
      moveForward();
    }

    // --------- IF BB ----------


  }
  else {

    // ------ SEGUIMENTO LINHA -----------

    obstacle = false;
    moveForward();
  }

  //-------- VARRIMENTO? -------------

  distance = readPing();
}

// ------ CONFIRMAR DIR E ESQ ---------

int lookRight() {
  servo.write(MAX_TURN);
  delay(500);
  int distance = readPing();
  delay(100);
  servo.write(SERVO_CENTRO);
  Serial.println("R");
  Serial.println(distance);
  return distance;
}

int lookLeft() {
  servo.write(MIN_TURN);
  delay(500);
  int distance = readPing();
  delay(100);
  servo.write(SERVO_CENTRO);
  Serial.println("L");
  Serial.println(distance);
  return distance;
  delay(100);
}

int readPing() {
//  delay(70);
//  int cm = sonar.ping_cm();
////  if (cm == 0) {
////    cm = 250;
////  }
//  return cm;





 int duration, cm;
  digitalWrite(TRIG_PIN, HIGH);
  delay(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  cm = (duration / 2) / 29.1;
  Serial.println(cm);
  return cm;







}

void moveStop() {
  analogWrite(ME, 0);
  analogWrite(MD, 0);
}

void moveForward() {
  if (!goesForward)  {
    goesForward = true;
    digitalWrite(dirE, HIGH);
    digitalWrite(dirD, HIGH);
    digitalWrite(ndirE, LOW);
    digitalWrite(ndirD, LOW);
    for (speedSet = MIN_SPEED; speedSet < MAX_SPEED; speedSet += MAX_SPEED_OFFSET) { // slowly bring the speed up to avoid loading down the batteries too quickly
      analogWrite(ME, speedSet); // Ambos motores ligam na mesma velocidade
      analogWrite(MD, speedSet);
      delay(5);
    }
  }
}

void moveBackward() {
  goesForward = false;
  digitalWrite(dirE, LOW);
  digitalWrite(dirD, LOW);
  digitalWrite(ndirE, HIGH);
  digitalWrite(ndirD, HIGH);
  for (speedSet = MIN_SPEED; speedSet < MAX_SPEED; speedSet += MAX_SPEED_OFFSET) {// slowly bring the speed up to avoid loading down the batteries too quickly
    analogWrite(ME, speedSet); // Ambos motores ligam na mesma velocidade
    analogWrite(MD, speedSet);
    delay(5);
  }
}

void turnRight() {
  digitalWrite(dirE, HIGH);
  digitalWrite(dirD, LOW);
  digitalWrite(ndirE, LOW);
  digitalWrite(ndirD, HIGH);
  analogWrite(ME, speedTurn); //O motor esquerdo fica ligado
  analogWrite(MD, speedTurn);
  delay(timeTurn);
}

void turnLeft() {
  digitalWrite(dirE, LOW);
  digitalWrite(dirD, HIGH);
  digitalWrite(ndirE, HIGH);
  digitalWrite(ndirD, LOW);
  analogWrite(ME, speedTurn); //O motor esquerdo fica ligado
  analogWrite(MD, speedTurn);
  delay(timeTurn);
}
