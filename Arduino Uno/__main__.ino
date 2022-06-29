#include "Timer.h"
#include <SoftwareSerial.h>
#include <Wire.h>
# define I2C_SLAVE_ADDRESS 11

SoftwareSerial BT(2, 3); // RX, TX  NECESSÁRIO?

//LEFT
#define enB 6
#define in3 11
#define in4 12

//RIGHT
#define enA 5
#define in1 13
#define in2 8

//Definição dos pinos dos sensores
#define pin_SE A0
#define pin_SD A1
int SensorE = 0;
int SensorD = 0;

bool flag1 = LOW;
boolean flagButton = false;

int n = 0;
char buf[9] = "";
String msg;
int numBytes;

int xAxis = 150, yAxis = 150;

int motorSpeedA = 0;
int motorSpeedB = 0;

int velocidade_1 = 110;
int velocidade_2 = 200;

bool AUTO = 0;

int color_threshold = 250;

int t_search = 200;

Timer timer;

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

  pinMode(pin_SE, INPUT);
  pinMode(pin_SD, INPUT);

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
    n = Wire.read();
    //buf[9] = Wire.read();
    Serial.print(numBytes);
    Serial.println(F("bytes recieved"));
    Serial.print(F("recieved value : "));
    //    Serial.println(n);
    // Serial.println("buf: " + buf);
    Serial.println("n: " + n);
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
  if (n == 10) {
    AUTO = true;
  }

  if (AUTO == true) {

    //Neste processo armazenamos o valor lido pelo sensor na variável que armazena tais dados.
    SensorE = analogRead(pin_SE);
    SensorD = analogRead(pin_SD);
    Serial.println(SensorE);
    Serial.println(SensorD);
    //Aqui está toda a lógica de comportamento do robô:
    if ((SensorE < color_threshold ) && (SensorD < color_threshold) && timer.read() < 4 * t_search) { //Ambos os Sensores detetam cor branca, podem andar para a frente
      timer.start();
      timer.stop();
      straight (velocidade_1);
      Serial.println("bb"); // informar que estão brancos os dois
      Serial.println(timer.read());
      delay(100);
    }

    else if ((SensorE < color_threshold ) && (SensorD > color_threshold) && timer.read() < 4 * t_search) { // Esquerdo branco e Direito preto
      timer.start();
      timer.stop();
      spin_left(velocidade_1); // O motor direito fica ligado, fazendo assim o carrinho virar
      Serial.println("bp");
    }

    else if ((SensorE > color_threshold) && (SensorD < color_threshold) && timer.read() < 4 * t_search) { // Esquerdo preto e Direito branco
      timer.start();
      timer.stop();
      spin_right(velocidade_1); // O motor direito desliga, fazendo assim o carrinho virar no outro sentido
      Serial.println("pb");
    }

    else if ((SensorE > color_threshold) && (SensorD > color_threshold) && timer.read() == 0 ) { // Ambos os sensores detetam preto
      timer.start();
      spin_left(velocidade_2); // O motor direito fica ligado, fazendo assim o carrinho virar
      Serial.println("pp");
      Serial.println("VIRAR ESQUERDA");
    }

    // else if ((SensorE > color_threshold) && (SensorD > color_threshold) && timer.read() != 0 ) {

    Serial.println(timer.read());
    //delay(100);
    if (timer.read() > t_search && timer.read() < 2 * t_search) {
      spin_right(velocidade_2); // O motor direito desliga, fazendo assim o carrinho virar no outro sentido
      Serial.println("VIRAR DIREITA");
    }
    if (timer.read() > 3 * t_search  && timer.read() < 4 * t_search) {
      Serial.println("PARAR");
      brake();
      delay(500);
      }
    
     } 
   if (timer.read() > 4 * t_search) {  
      
      spin_right(velocidade_2);
      if ((SensorE > color_threshold) && (SensorD < color_threshold)) {
      brake();
      timer.start();
      timer.stop();
      AUTO = false;
      
    }
  }
} 

  if (AUTO == false) {

    if (n == 1) {
      Serial.println("ANDAR PARA FRENTE");
      xAxis = 150;
      yAxis = 0;
    }
    if (n == 5) {
      Serial.println("ANDAR PARA TRAS");
      xAxis = 150;
      yAxis = 300;
    }
    if (n == 9) {
      Serial.println("PARAR");
      xAxis = 150;
      yAxis = 150;
    }
    if (n == 3) {
      Serial.println("RODAR PARA DIREITA");
      xAxis = 300;
      yAxis = 150;
    }
    if (n == 7) {
      Serial.println("RODAR PARA ESQUERDA");
      xAxis = 0;
      yAxis = 150;
    }
    if (n == 2) {
      Serial.println("ANDAR PARA NE");
      xAxis = 300;
      yAxis = 0;
    }

    if (n == 4) {
      Serial.println("ANDAR PARA SE");
      xAxis = 300;
      yAxis = 300;
    }
    if (n == 8) {
      Serial.println("ANDAR PARA NO");
      xAxis = 0;
      yAxis = 0;
    }
    if (n == 6) {
      Serial.println("ANDAR PARA SO");
      xAxis = 0;
      yAxis = 300;
    }

    if (xAxis > 130 && xAxis < 170 && yAxis > 130 && yAxis < 170) {
      Stop();
    }

    if (yAxis > 130 && yAxis < 170) {


      // 130 - 115 | 60 - ?
      if (xAxis < 130) {
        turnLeft();
        motorSpeedA = map(xAxis, 130, 0, 100, 255);
        motorSpeedB = map(xAxis, 130, 0, 100, 255);
      }

      if (xAxis > 170) {
        turnRight();
        motorSpeedA = map(xAxis, 170, 300, 100, 255);
        motorSpeedB = map(xAxis, 170, 300, 100, 255);
      }

    } else {

      if (xAxis > 130 && xAxis < 170) {

        if (yAxis < 130) {
          forword();
        }
        if (yAxis > 170) {
          backword();
        }

        if (yAxis < 130) {
          motorSpeedA = map(yAxis, 130, 0, 50, 255);
          motorSpeedB = map(yAxis, 130, 0, 50, 255);
        }

        if (yAxis > 170) {
          motorSpeedA = map(yAxis, 170, 300, 50, 255);
          motorSpeedB = map(yAxis, 170, 300, 50, 255);
        }

      } else {

        if (yAxis < 130) {
          forword();
        }
        if (yAxis > 170) {
          backword();
        }

        if (xAxis < 130) {
          motorSpeedA = map(xAxis, 130, 0, 200, 100);
          motorSpeedB = 255;
        }

        if (xAxis > 170) {
          motorSpeedA = 255;
          motorSpeedB = map(xAxis, 170, 300, 200, 100);
        }
      }
    }

    //Serial.print(motorSpeedA);
    //Serial.print(",");
    //Serial.println(motorSpeedA);

    analogWrite(enA, motorSpeedA); // Send PWM signal to motor A1
    analogWrite(enB, motorSpeedB); // Send PWM signal to motor B1
  }
}

void forword() {
  Serial.println("forword");
  Serial.println("A: " + motorSpeedA);
  Serial.println("B: " + motorSpeedB);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void backword() {
  Serial.println("backword");
  Serial.println("A: " + motorSpeedA);
  Serial.println("B: " + motorSpeedB);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  Serial.println("turnRight");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  Serial.println("turnLeft");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
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

int straight(int speed) {
 digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, speed); // Ambos motores ligam na mesma velocidade
  analogWrite(enA, speed);
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
