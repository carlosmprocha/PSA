#include <Wire.h>
# define I2C_SLAVE_ADDRESS 11 // 12 pour l'esclave 2 et ainsi de suite

//https://circuitdigest.com/microcontroller-projects/arduino-i2c-tutorial-communication-between-two-arduino
//https://microcontrollerslab.com/arduino-i2c-tutorial-communication-between-arduino-boards/
//https://github.com/linorobot/kinematics

int n = 0;
int numBytes;

//MOTOR
#define INB_PIN 10
#define IN3_PIN 11
#define IN4_PIN 12
#define INA_PIN 9
#define IN1_PIN 13
#define IN2_PIN 8



void setup()
{
  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(9600);
  Serial.println("-------------------------------------I am Slave1");
  //delay(1000);
  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);

  pinMode(INB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(INA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
}


void loop() {
  
  delay(500);
  Serial.println(F("---> recieved events"));
  n = Wire.read();
  Serial.print(numBytes);
  Serial.println(F("bytes recieved"));
  Serial.print(F("recieved value : "));
  Serial.println(n);

  if (n == 1) {
    Serial.println("ANDAR PARA FRENTE");
    analogWrite(INB_PIN, 200); // Velocidade do motor
    analogWrite(INA_PIN, 200); // Velocidade do motor
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN1_PIN, LOW);
  }

  if (n == 5) {
    Serial.println("ANDAR PARA TRAS");
    analogWrite(INB_PIN, 175); // Velocidade do motor
    analogWrite(INA_PIN, 175); // Velocidade do motor
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN1_PIN, HIGH);
  }

  if (n == 9) {
    Serial.println("PARAR");
    analogWrite(INB_PIN, 0); // Velocidade do motor
    analogWrite(INA_PIN, 0); // Velocidade do motor
  }

  if (n == 3) {
    Serial.println("RODAR PARA DIREITA");
    analogWrite(INB_PIN, 200); // Velocidade do motor
    analogWrite(INA_PIN, 200); // Velocidade do motor
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN1_PIN, HIGH);
  }

  if (n == 7) {
    Serial.println("RODAR PARA ESQUERDA");
    analogWrite(INB_PIN, 200); // Velocidade do motor
    analogWrite(INA_PIN, 200);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN1_PIN, LOW);
  }

  if (n == 2) {
    Serial.println("ANDAR PARA NE");
    analogWrite(INB_PIN, 250); // Velocidade do motor
    analogWrite(INA_PIN, 80);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN1_PIN, LOW);
  }

  if (n == 4) {
    Serial.println("ANDAR PARA SE");
    analogWrite(INB_PIN, 250); // Velocidade do motor
    analogWrite(INA_PIN, 80); // Velocidade do motor
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN1_PIN, HIGH);



  }
  if (n == 8) {
    Serial.println("ANDAR PARA NO");
    analogWrite(INB_PIN, 80); // Velocidade do motor
    analogWrite(INA_PIN, 250);
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);
    digitalWrite(IN2_PIN, HIGH);
    digitalWrite(IN1_PIN, LOW);
  }
  if (n == 6) {
    Serial.println("ANDAR PARA SO");
    analogWrite(INB_PIN, 250); // Velocidade do motor
    analogWrite(INA_PIN, 80);
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN1_PIN, HIGH);
  }
}


void requestEvents()
{
  Serial.println(F("---> recieved request"));
  Serial.print(F("sending value : "));
  Serial.println(n);
  Wire.write(n);

}


void receiveEvents(int numBytes)
{
}
