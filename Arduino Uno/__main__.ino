#include <NewPing.h>
#include <Servo.h>
#include "Timer.h"
#include <Wire.h>

# define I2C_SLAVE_ADDRESS 11

//const byte interruptPinA = 2;
//const byte interruptPinB = 3;
//double count = 1;
//long protectedCount = 0;
//long previousCount = 0;
//#define readA bitRead (PIND,2) //fastest than digitalRead()
//#define readB bitRead (PIND,3) //fastest than digitalRead()

//SoftwareSerial BT(2, 3); // RX, TX  NECESSÁRIO?

//LEFT
#define enB 6
#define in3 11
#define in4 12

//RIGHT
#define enA 5
#define in1 13
#define in2 8

#define pin_SE A0
#define pin_SD A1

#define pin_ST A2

#define TRIG_PIN 4
#define ECHO_PIN 7

#define MAX_DISTANCE 300
#define MIN_DISTANCE 15
#define MIN_DISTANCE_INT 440
#define MAX_DISTANCE_INT 470
#define LONGE 495

#define MIN_TURN 20
#define MAX_TURN 145
#define SERVO_CENTRO 60

#define velocidade_1 = 150;
#define velocidade_2 = 255;
#define velocidade_c = 200;


#define timeTurn 50
#define timeStop 100
#define timeReverse 10
#define timeTrans 50

int SensorE = 0;
int SensorD = 0;

bool flag = 0;
bool flag1 = LOW;
boolean flagButton = false;
int check = 0;

int n = 0;
char buf[9] = "";
String msg;
int numBytes;

int xAxis = 150, yAxis = 150;

int motorSpeedA = 0;
int motorSpeedB = 0;



int MODO = 0;

int color_threshold = 250;

int t_search = 200;

Timer timer;

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo servo;

void setup() {

  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(9600);
  Serial.println("-------------------------------------I am Slave1");

  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);

  servo.attach(10);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(pin_SE, INPUT);
  pinMode(pin_SD, INPUT);
  pinMode(pin_ST, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //  //Serial.begin (9600);
  //  pinMode (interruptPinA,INPUT_PULLUP);
  //  pinMode (interruptPinB,INPUT_PULLUP);
  //  attachInterrupt(digitalPinToInterrupt(interruptPinA), isrA, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(interruptPinB), isrB, CHANGE);

  //delay(500);

  servo.write(SERVO_CENTRO);

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

    // Serial.println(F("---> recieved events"));
    n = Wire.read();
    //buf[9] = Wire.read();
    //Serial.print(numBytes);
    //Serial.println(F("bytes recieved"));
    //Serial.print(F("recieved value : "));
    //    Serial.println(n);
    //    Serial.println("buf: " + buf[]);
    //    Serial.println("n: " + n);
    msg = String(buf);
    Serial.println(msg);
    if (msg.indexOf("GET") > 0) {
      msg = "X150Y150";
    }

    if (msg.indexOf("Y500") > 0) {
      msg = "X150Y150";
      MODO = 1;
    }


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

  SensorE = analogRead(pin_SE);
  SensorD = analogRead(pin_SD);

  if (MODO == 1) {


    //Neste processo armazenamos o valor lido pelo sensor na variável que armazena tais dados.

    Serial.println(SensorE);
    Serial.println(SensorD);


    //Aqui está toda a lógica de comportamento do robô:
    if ((SensorE < color_threshold ) && (SensorD < color_threshold) && timer.read() < 4 * t_search) { //Ambos os Sensores detetam cor branca, podem andar para a frente
      timer.start();
      timer.stop();
      straight (velocidade_1, velocidade_1);
      Serial.println("bb"); // informar que estão brancos os dois
    }

    else if ((SensorE < color_threshold ) && (SensorD > color_threshold) && timer.read() < 4 * t_search) { // Esquerdo branco e Direito preto
      timer.start();
      timer.stop();
      spin_left(velocidade_2); // O motor direito fica ligado, fazendo assim o carrinho virar
      Serial.println("bp");
    }

    else if ((SensorE > color_threshold) && (SensorD < color_threshold) && timer.read() < 4 * t_search) { // Esquerdo preto e Direito branco
      timer.start();
      timer.stop();
      spin_right(velocidade_2); // O motor direito desliga, fazendo assim o carrinho virar no outro sentido
      Serial.println("pb");
    }

    else if ((SensorE > color_threshold) && (SensorD > color_threshold) && timer.read() == 0 ) { // Ambos os sensores detetam preto
      timer.start();
      spin_left(velocidade_2); // O motor direito fica ligado, fazendo assim o carrinho virar
      Serial.println("pp");
      Serial.println("VIRAR ESQUERDA");
    }


    if (timer.read() > t_search && timer.read() < 2 * t_search) {
      spin_right(velocidade_2); // O motor direito desliga, fazendo assim o carrinho virar no outro sentido
      Serial.println("VIRAR DIREITA");
    }
    if (timer.read() > 3 * t_search  && timer.read() < 4 * t_search) {
      Serial.println("PARAR");
      brake();
      delay(500);
    }


    if (timer.read() > 4 * t_search) {

      spin_right(velocidade_2);
      if ((SensorE > color_threshold) && (SensorD < color_threshold)) {
        brake();
        timer.start();
        timer.stop();
        MODO = 0;

      }
    }













    // ULTRASOM
//
//    distance = readPing();
//    delay(50);
//
//    if (distance <= MIN_DISTANCE) {
//
//      timer.start();
//      timer.stop();
//      brake();
//      MODO = 2;
//      delay(timeStop);
//      reverse(velocidade_1, velocidade_1);
//      delay(timeReverse);
//      brake();
//      delay(timeStop);
//
//      int US = 1;
//    }

    // ULTRASOM


















  }

  if (MODO == 0) {

    //    if (n == 1) {
    //      Serial.println("ANDAR PARA FRENTE");
    //      xAxis = 150;
    //      yAxis = 0;
    //    }
    //    if (n == 5) {
    //      Serial.println("ANDAR PARA TRAS");
    //      xAxis = 150;
    //      yAxis = 300;
    //    }
    //    if (n == 9) {
    //      Serial.println("PARAR");
    //      xAxis = 150;
    //      yAxis = 150;
    //    }
    //    if (n == 3) {
    //      Serial.println("RODAR PARA DIREITA");
    //      xAxis = 300;
    //      yAxis = 150;
    //    }
    //    if (n == 7) {
    //      Serial.println("RODAR PARA ESQUERDA");
    //      xAxis = 0;
    //      yAxis = 150;
    //    }
    //    if (n == 2) {
    //      Serial.println("ANDAR PARA NE");
    //      xAxis = 300;
    //      yAxis = 0;
    //    }
    //
    //    if (n == 4) {
    //      Serial.println("ANDAR PARA SE");
    //      xAxis = 300;
    //      yAxis = 300;
    //    }
    //    if (n == 8) {
    //      Serial.println("ANDAR PARA NO");
    //      xAxis = 0;
    //      yAxis = 0;
    //    }
    //    if (n == 6) {
    //      Serial.println("ANDAR PARA SO");
    //      xAxis = 0;
    //      yAxis = 300;
    //    }

    if (xAxis > 130 && xAxis < 170 && yAxis > 130 && yAxis < 170) {
      brake();
    }

    if (yAxis > 130 && yAxis < 170) {

      // 130 - 115 | 60 - ?
      if (xAxis < 130) {

        motorSpeedA = map(xAxis, 130, 0, 100, 255);
        motorSpeedB = map(xAxis, 130, 0, 100, 255);
        spin_left(motorSpeedA);
      }

      if (xAxis > 170) {

        motorSpeedA = map(xAxis, 170, 300, 100, 255);
        motorSpeedB = map(xAxis, 170, 300, 100, 255);
        spin_right(motorSpeedA);
      }

    } else {

      if (xAxis > 130 && xAxis < 170) {

        if (yAxis < 130) {
          motorSpeedA = map(yAxis, 130, 0, 50, 255);
          motorSpeedB = map(yAxis, 130, 0, 50, 255);
          straight(motorSpeedA, motorSpeedB);
        }
        if (yAxis > 170) {
          motorSpeedA = map(yAxis, 170, 300, 50, 255);
          motorSpeedB = map(yAxis, 170, 300, 50, 255);
          reverse(motorSpeedA, motorSpeedB);
        }

      } else {

        if (xAxis < 130) {
          motorSpeedA = map(xAxis, 130, 0, 200, 100);
          motorSpeedB = 255;
        }

        if (xAxis > 170) {
          motorSpeedA = 255;
          motorSpeedB = map(xAxis, 170, 300, 200, 100);
        }

        if (yAxis < 130) {
          straight(motorSpeedA, motorSpeedB);
        }
        if (yAxis > 170) {
          reverse(motorSpeedA, motorSpeedB);
        }
      }
    }

    //Serial.print(motorSpeedA);
    //Serial.print(",");
    //Serial.println(motorSpeedA);

    //    analogWrite(enA, motorSpeedA); // Send PWM signal to motor A1
    //    analogWrite(enB, motorSpeedB); // Send PWM signal to motor B1
  }






















  // ULTRASOM
//
//
//  if (MODO == 2) {
//
//    SensorT = analogRead(pin_ST);
//
//    if (US == 1) {
//
//      //     if (flag == 0) {
//
//      //spin_right(velocidade_c);
//
//      //CONTAR ENCODER VIRAR Á DIREITA
//
//      //brake();
//
//      //flag = 1;
//      //   }
//
//
//      if (SensorT <= MIN_DISTANCE_INT) {
//        spin_right(velocidade_2);
//        delay(timeTurn);
//        brake();
//        delay(timeTurn);
//        straight(velocidade_1, velocidade_1);
//      }
//
//      else if (LONGE >= SensorT && SensorT >= MAX_DISTANCE_INT) {
//        spin_left(velocidade_2);
//        delay(timeTurn);
//        brake();
//        delay(timeTurn);
//        straight(velocidade_1, velocidade_1);
//      }
//
//      else if (LONGE <= SensorT) {
//        brake();
//        US = 2;
//        flag = 0;
//      }
//    }
//
//    else if (US == 2) {
//
//
//      //     if (flag == 0) {
//
//      //spin_left(velocidade_c);
//
//      //CONTAR ENCODER VIRAR Á ESQUERDA
//
//      //brake();
//
//      //straight(velocidade_1, velocidade_1);
//      //delay(timTrans);
//      //flag = 1;
//      // }
//
//
//
//      if (SensorT <= MIN_DISTANCE_INT) {
//        spin_right(velocidade_2);
//        delay(timeTurn);
//        brake();
//        delay(timeTurn);
//        straight(velocidade_1, velocidade_1);
//      }
//
//      else if (LONGE >= SensorT && SensorT >= MAX_DISTANCE_INT) {
//        spin_left(velocidade_2);
//        delay(timeTurn);
//        brake();
//        delay(timeTurn);
//        straight(velocidade_1, velocidade_1);
//      }
//
//      else if (LONGE <= SensorT) {
//        brake();
//        US = 3;
//        flag = 0;
//      }
//    }
//
//    else if (US == 3) {
//
//
//
//      //     if (flag == 0) {
//
//      //spin_left(velocidade_c);
//
//      //CONTAR ENCODER VIRAR Á ESQUERDA
//
//      //brake();
//
//      //straight(velocidade_1, velocidade_1);
//      //flag = 1;
//      // }
//
//
//
//      if ((SensorE < color_threshold) || (SensorD < color_threshold) &&  check == 0) {
//        delay(50);
//        brake();
//        delay(50);
//        spin_right(velocidade_2);
//        check = 1;
//      }
//
//      if (check == 1) {
//
//        if ((SensorE > color_threshold) && (SensorD < color_threshold)) {
//          brake();
//          timer.start();
//          timer.stop();
//          MODO = 1;
//
//        }
//
//
//      }
//    }
//  }


  // ULTRASOM






















}

//void forword() {
//  Serial.println("forword");
//  Serial.println("A: " + motorSpeedA);
//  Serial.println("B: " + motorSpeedB);
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, HIGH);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, HIGH);
//}
//
//void backword() {
//  Serial.println("backword");
//  Serial.println("A: " + motorSpeedA);
//  Serial.println("B: " + motorSpeedB);
//  digitalWrite(in1, HIGH);
//  digitalWrite(in2, LOW);
//  digitalWrite(in3, HIGH);
//  digitalWrite(in4, LOW);
//}
//
//void turnRight() {
//  Serial.println("turnRight");
//  digitalWrite(in1, HIGH);
//  digitalWrite(in2, LOW);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, HIGH);
//}
//
//void turnLeft() {
//  Serial.println("turnLeft");
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, HIGH);
//  digitalWrite(in3, HIGH);
//  digitalWrite(in4, LOW);
//}
//
//void Stop() {
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, LOW);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, LOW);
//  Serial.println("stop");
//}

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

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}


int straight(int speedA, int speedB) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speedB); // Ambos motores ligam na mesma velocidade
  analogWrite(enA, speedA);
}

int reverse(int speedA, int speedB) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speedB); // Ambos motores ligam na mesma velocidade
  analogWrite(enA, speedA);
}

int spin_right (int speed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speed); //O motor esquerdo fica ligado
  analogWrite(enA, speed);
}

int spin_left (int speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, speed); //O motor esquerdo fica ligado
  analogWrite(enA, speed);
}

int brake() {
  analogWrite(enB, 0); //O motor esquerdo fica ligado
  analogWrite(enA, 0);
}




//void isrA() {
//  if (readB != readA) {
//    count = count + 0.25;
//  } else {
//    count = count - 0.25;
//  }
//}
//
//void isrB() {
//  if (readA == readB) {
//    count = count + 0.25;
//  } else {
//    count = count - 0.25;
//  }
//}
