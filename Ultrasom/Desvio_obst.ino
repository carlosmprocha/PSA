#include<Servo.h>

#define ME 10 // Pino_Velocidade Motor Esquerda ( 0 a 255);
#define MD 9 //Pino_Velocidade Motor Direita ( 0 a 255);
#define dirE 12 //Pino_Direção do Motor Esquerdo: Para frente / Para trás (HIGH ou LOW);
#define ndirE 11
#define dirD 8 //Pino_Direção do Motor Direito: Para frente / Para trás (HIGH ou LOW);
#define ndirD 13

int trig = 8;
int echo = 9;
int dt = 10;
int flag = 0;
int dist_min = 5; //cm
bool alert = LOW;
int velocidade = 100;
int t_turn = 500;
int t_front = 500;
Servo servo;

//int distance,duration;
void setup() {

  pinMode(ME, OUTPUT);
  pinMode(MD, OUTPUT);
  pinMode(dirE, OUTPUT);
  pinMode(dirD, OUTPUT);
  pinMode(ndirE, OUTPUT);
  pinMode(ndirD, OUTPUT);

  //Setamos a direção inicial do motor como 0, isso fará com que ambos os motores girem para frente
  digitalWrite(dirE, HIGH);
  digitalWrite(dirD, HIGH);
  digitalWrite(ndirE, LOW);
  digitalWrite(ndirD, LOW);
  analogWrite(ME, 0);
  analogWrite(MD, 0);

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

      if (calc_dis <= dist_min) {
        int p_min = p;
        alert = HIGH;
        delay(100);
      }
    }
  }
  if (flag == 1) {
    for (int p = 165; p >= 25; p = p - 10)
    {
      servo.write(p);
      delay(1);

      calc_dis();
      flag = 0;

      if (calc_dis <= dist_min) {
        int p_min = p;
        alert = HIGH;
        delay(100);
      }
    }
  }

  if (alert == LOW) {
    analogWrite(ME, velocidade);
    analogWrite(MD, velocidade);
    Serial.println(NORMAL);
  }
  if (alert == HIGH) {
    analogWrite(ME, 0);
    analogWrite(MD, 0);
    delay(500);
    Serial.println(ALERTA);

    //TURN RIGHT
    digitalWrite(dirE, HIGH);
    digitalWrite(dirD, LOW);
    digitalWrite(ndirE, LOW);
    digitalWrite(ndirD, HIGH);
    analogWrite(ME, velocidade);
    analogWrite(MD, velocidade);
    Serial.println(DIREITA);
    delay(t_turn);

    analogWrite(ME, 0);
    analogWrite(MD, 0);
    delay(500);

    //STRAIGHT
    digitalWrite(dirE, HIGH);
    digitalWrite(dirD, HIGH);
    digitalWrite(ndirE, LOW);
    digitalWrite(ndirD, LOW);
    analogWrite(ME, velocidade);
    analogWrite(MD, velocidade);
    Serial.println(FRENTE);
    delay(t_front);

    analogWrite(ME, 0);
    analogWrite(MD, 0);
    delay(500);

    //TURN LEFT
    digitalWrite(dirE, LOW);
    digitalWrite(dirD, HIGH);
    digitalWrite(ndirE, HIGH);
    digitalWrite(ndirD, LOW);
    analogWrite(ME, velocidade);
    analogWrite(MD, velocidade);
    Serial.println(ESQUERDA);
    delay(t_turn);

    analogWrite(ME, 0);
    analogWrite(MD, 0);
    delay(500);

    //STRAIGHT
    digitalWrite(dirE, HIGH);
    digitalWrite(dirD, HIGH);
    digitalWrite(ndirE, LOW);
    digitalWrite(ndirD, LOW);
    analogWrite(ME, velocidade);
    analogWrite(MD, velocidade);
    Serial.println(FRENTE);
    delay(t_front);

    analogWrite(ME, 0);
    analogWrite(MD, 0);
    delay(500);

    //TURN LEFT
    digitalWrite(dirE, LOW);
    digitalWrite(dirD, HIGH);
    digitalWrite(ndirE, HIGH);
    digitalWrite(ndirD, LOW);
    analogWrite(ME, velocidade);
    analogWrite(MD, velocidade);
    Serial.println(ESQUERDA);
    delay(t_turn);

    analogWrite(ME, 0);
    analogWrite(MD, 0);
    delay(500);


    //STRAIGHT
    digitalWrite(dirE, HIGH);
    digitalWrite(dirD, HIGH);
    digitalWrite(ndirE, LOW);
    digitalWrite(ndirD, LOW);
    analogWrite(ME, velocidade);
    analogWrite(MD, velocidade);
    Serial.println(FRENTE);
    delay(t_front);

    analogWrite(ME, 0);
    analogWrite(MD, 0);
    delay(500);


    //TURN RIGHT
    digitalWrite(dirE, HIGH);
    digitalWrite(dirD, LOW);
    digitalWrite(ndirE, LOW);
    digitalWrite(ndirD, HIGH);
    analogWrite(ME, velocidade);
    analogWrite(MD, velocidade);
    Serial.println(DIREITA);
    delay(t_turn);

    analogWrite(ME, 0);
    analogWrite(MD, 0);
    delay(500);

    alert = LOW;
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
