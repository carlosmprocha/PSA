#include "Timer.h"
//Definição dos pinos de controlo do motor
#define ME 6               //Pino_Velocidade Motor Esquerda (0 a 255);
#define MD 5                 //Pino_Velocidade Motor Direita (0 a 255);
#define dirE 12              //Pino_Direção do Motor Esquerdo: Para frente HIGH;
#define ndirE 11             //Pino_Direção do Motor Esquerdo: Para frente LOW;
#define dirD 8               //Pino_Direção do Motor Direito: Para frente HIGH;
#define ndirD 13             //Pino_Direção do Motor Direito: Para frente LOW;

//Definição dos pinos dos sensores
#define pin_SE A0
#define pin_SD A1
int SensorE = 0;
int SensorD = 0;

//variáveis responsáveis por controlar a velocidade dos motores
int velocidade = 110;
int velocidade_min = 200;

//variável responsável por definir a diferença entre preto e branco
//menor que color_threshold -> branco; maior que color_threshold -> preto
int color_threshold = 250;

int t_search = 200;

Timer timer;
// for micro second resolution:

void setup() {
  Serial.begin(9600);

  //pinos de controle dos motores como saída
  pinMode(ME, OUTPUT);
  pinMode(MD, OUTPUT);
  pinMode(dirE, OUTPUT);
  pinMode(dirD, OUTPUT);
  pinMode(ndirE, OUTPUT);
  pinMode(ndirD, OUTPUT);

  //direção inicial do motor
  digitalWrite(dirE, HIGH);
  digitalWrite(dirD, HIGH);
  digitalWrite(ndirE, LOW);
  digitalWrite(ndirD, LOW);
  analogWrite(ME, 0); //O motor esquerdo fica ligado
  analogWrite(MD, 0);

  //pinos dos sensores como entrada
  pinMode(pin_SE, INPUT);
  pinMode(pin_SD, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Neste processo armazenamos o valor lido pelo sensor na variável que armazena tais dados.
  SensorE = analogRead(pin_SE);
  SensorD = analogRead(pin_SD);
  Serial.println(SensorE);
  Serial.println(SensorD);
  //Aqui está toda a lógica de comportamento do robô:
  if ((SensorE < color_threshold ) && (SensorD < color_threshold)) { //Ambos os Sensores detetam cor branca, podem andar para a frente
    timer.start();
    timer.stop();
    straight (velocidade);
    Serial.println("bb"); // informar que estão brancos os dois
    Serial.println(timer.read());
    delay(100);
  }

  else if ((SensorE < color_threshold ) && (SensorD > color_threshold)) { // Esquerdo branco e Direito preto
     timer.start();
    timer.stop();
    spin_left(velocidade); // O motor direito fica ligado, fazendo assim o carrinho virar
    Serial.println("bp");
  }

  else if ((SensorE > color_threshold) && (SensorD < color_threshold)) { // Esquerdo preto e Direito branco
     timer.start();
    timer.stop();
    spin_right(velocidade); // O motor direito desliga, fazendo assim o carrinho virar no outro sentido
    Serial.println("pb");
  }

  else if ((SensorE > color_threshold) && (SensorD > color_threshold) && timer.read() == 0 ) { // Ambos os sensores detetam preto
    timer.start();
    spin_left(velocidade_min); // O motor direito fica ligado, fazendo assim o carrinho virar
    Serial.println("pp");
    Serial.println("VIRAR ESQUERDA");
  }

 // else if ((SensorE > color_threshold) && (SensorD > color_threshold) && timer.read() != 0 ) {

    Serial.println(timer.read());
    //delay(100);
    if (timer.read() > t_search && timer.read() < 2 * t_search) {
      spin_right(velocidade_min); // O motor direito desliga, fazendo assim o carrinho virar no outro sentido
      Serial.println("VIRAR DIREITA");
    }
    if (timer.read() > 3 * t_search  && timer.read() < 4 * t_search) {
      Serial.println("PARAR");
      brake();
   // }
  }
}


int straight(int speed) {
  digitalWrite(dirE, HIGH);
  digitalWrite(dirD, HIGH);
  digitalWrite(ndirE, LOW);
  digitalWrite(ndirD, LOW);
  analogWrite(ME, speed); // Ambos motores ligam na mesma velocidade
  analogWrite(MD, speed);
}

int spin_right (int speed) {
  digitalWrite(dirE, HIGH);
  digitalWrite(dirD, LOW);
  digitalWrite(ndirE, LOW);
  digitalWrite(ndirD, HIGH);
  analogWrite(ME, speed); //O motor esquerdo fica ligado
  analogWrite(MD, speed);
}

int spin_left (int speed) {
  digitalWrite(dirE, LOW);
  digitalWrite(dirD, HIGH);
  digitalWrite(ndirE, HIGH);
  digitalWrite(ndirD, LOW);
  analogWrite(ME, speed); //O motor esquerdo fica ligado
  analogWrite(MD, speed);
}

int brake() {
  analogWrite(ME, 0); //O motor esquerdo fica ligado
  analogWrite(MD, 0);
}
