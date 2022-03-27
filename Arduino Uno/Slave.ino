#include <Wire.h>
# define I2C_SLAVE_ADDRESS 11 // 12 pour l'esclave 2 et ainsi de suite
#define PAYLOAD_SIZE 2


//MOTOR
#define INB_PIN 11
#define IN3_PIN 13
#define IN4_PIN 12
#define INA_PIN 10
#define IN1_PIN 9
#define IN2_PIN 8
#define INB1_PIN 3
#define IN31_PIN 2
#define IN41_PIN 4
#define INA1_PIN 5
#define IN11_PIN 6
#define IN21_PIN 7



void setup()
{
Wire.begin(I2C_SLAVE_ADDRESS);
Serial.begin(9600);
Serial.println("-------------------------------------I am Slave1");
delay(1000);
Wire.onRequest(requestEvents);
Wire.onReceive(receiveEvents);

  pinMode(INB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(INA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(INB1_PIN, OUTPUT);
  pinMode(IN31_PIN, OUTPUT);
  pinMode(IN41_PIN, OUTPUT);
  pinMode(INA1_PIN, OUTPUT);
  pinMode(IN11_PIN, OUTPUT);
  pinMode(IN21_PIN, OUTPUT);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN31_PIN, HIGH);
  digitalWrite(IN41_PIN, LOW);
  digitalWrite(IN21_PIN, HIGH);
  digitalWrite(IN11_PIN, LOW);

}


void loop(){

  delay(5000);
  
  analogWrite(INB_PIN, 255); // Velocidade do motor
  analogWrite(INA_PIN, 255); // Velocidade do motor
  analogWrite(INB1_PIN, 255); // Velocidade do motor
  analogWrite(INA1_PIN, 255); // Velocidade do motor


  
}



int n = 0;
void requestEvents()
{
Serial.println(F("---> recieved request"));
Serial.print(F("sending value : "));
Serial.println(n);
Wire.write(n);

}


void receiveEvents(int numBytes)
{
Serial.println(F("---> recieved events"));
n = Wire.read();
Serial.print(numBytes);
Serial.println(F("bytes recieved"));
Serial.print(F("recieved value : "));
Serial.println(n);

}
