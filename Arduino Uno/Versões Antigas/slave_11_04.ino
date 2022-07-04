#include <Wire.h>
# define I2C_SLAVE_ADDRESS 11 // 12 pour l'esclave 2 et ainsi de suite
//#define PAYLOAD_SIZE 2
//https://circuitdigest.com/microcontroller-projects/arduino-i2c-tutorial-communication-between-two-arduino
//https://microcontrollerslab.com/arduino-i2c-tutorial-communication-between-arduino-boards/
//https://github.com/linorobot/kinematics

int n = 0;
int a = 0;
int b = 0;
int numBytes;

//MOTOR
//#define INB_PIN 9
//#define IN3_PIN 10
//#define IN4_PIN 8
//#define INA_PIN 11
//#define IN1_PIN 13
//#define IN2_PIN 12
#define INB1_PIN 13
#define IN31_PIN 11
#define IN41_PIN 12
#define INA1_PIN 8
#define IN11_PIN 9
#define IN21_PIN 10



void setup()
{
Wire.begin(I2C_SLAVE_ADDRESS);
Serial.begin(9600);
Serial.println("-------------------------------------I am Slave1");
//delay(1000);
Wire.onRequest(requestEvents);
Wire.onReceive(receiveEvents);

 // pinMode(INB_PIN, OUTPUT);
//  pinMode(IN3_PIN, OUTPUT);
//  pinMode(IN4_PIN, OUTPUT);
//  pinMode(INA_PIN, OUTPUT);
//  pinMode(IN1_PIN, OUTPUT);
 // pinMode(IN2_PIN, OUTPUT);
  pinMode(INB1_PIN, OUTPUT);
  pinMode(IN31_PIN, OUTPUT);
  pinMode(IN41_PIN, OUTPUT);
  pinMode(INA1_PIN, OUTPUT);
  pinMode(IN11_PIN, OUTPUT);
  pinMode(IN21_PIN, OUTPUT);


}


void loop(){

  delay(100);
  Serial.println(F("---> recieved events"));
n = Wire.read();
Serial.print(numBytes);
Serial.println(F("bytes recieved"));
Serial.print(F("recieved value : "));
Serial.println(n);
if(n==12){
 
  Serial.println("ANDAR PARA FRENTE - SDSDKVKDVKDMSVMSDMVDPVMPDMVPKSMDVKPMDSKVPDMVPMDPKVMKPVVSBF");
//  analogWrite(INB_PIN, 255); // Velocidade do motor
 // analogWrite(INA_PIN, 255); // Velocidade do motor
  analogWrite(INB1_PIN, 255); // Velocidade do motor
  analogWrite(INA1_PIN, 255); // Velocidade do motor
//  digitalWrite(IN3_PIN, HIGH);
 // digitalWrite(IN4_PIN, LOW);
 // digitalWrite(IN2_PIN, HIGH);
//  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN31_PIN, LOW);
  digitalWrite(IN41_PIN, HIGH);
  digitalWrite(IN21_PIN, LOW);
  digitalWrite(IN11_PIN, HIGH);
 // delay(5000);
 // analogWrite(INA_PIN, 0); // Velocidade do motor
 // analogWrite(INB1_PIN, 0); // Velocidade do motor
 // analogWrite(INA1_PIN, 0); // Velocidade do motor
//  analogWrite(INB_PIN, 0); // Velocidade do motor
//  Serial.println("STOP");
  }

  if(n==11){
   
    
  Serial.println("ANDAR PARA TRAS");
//  analogWrite(INB_PIN, 255); // Velocidade do motor
 // analogWrite(INA_PIN,255); // Velocidade do motor
  analogWrite(INB1_PIN, 255); // Velocidade do motor
  analogWrite(INA1_PIN, 255); // Velocidade do motor
 // digitalWrite(IN3_PIN, LOW);
 // digitalWrite(IN4_PIN, HIGH);
 // digitalWrite(IN2_PIN, LOW);
 // digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN31_PIN, HIGH);
  digitalWrite(IN41_PIN, LOW);
  digitalWrite(IN21_PIN, HIGH);
  digitalWrite(IN11_PIN, LOW);
 
 // delay(5000);
//  Serial.println("PARAR");
 // analogWrite(INA_PIN, 0); // Velocidade do motor
 // analogWrite(INB1_PIN, 0); // Velocidade do motor
 // analogWrite(INA1_PIN, 0); // Velocidade do motor
//  analogWrite(INB_PIN, 0); // Velocidade do motor
  }

  if(n==10){
   
  
  Serial.println("PARAR");
//  analogWrite(INA_PIN, 0); // Velocidade do motor
  analogWrite(INB1_PIN, 0); // Velocidade do motor
  analogWrite(INA1_PIN, 0); // Velocidade do motor
//  analogWrite(INB_PIN, 0); // Velocidade do motor
  }

if(n==6){
 
  Serial.println("ANDAR PARA DIREITA");
 // analogWrite(INB_PIN, 255); // Velocidade do motor
 // analogWrite(INA_PIN, 255); // Velocidade do motor
  analogWrite(INB1_PIN, 255); // Velocidade do motor
  analogWrite(INA1_PIN, 255); // Velocidade do motor
//  digitalWrite(IN3_PIN, HIGH);
 // digitalWrite(IN4_PIN, LOW);
 // digitalWrite(IN2_PIN, HIGH);
//  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN31_PIN, LOW);
  digitalWrite(IN41_PIN, HIGH);
  digitalWrite(IN21_PIN, HIGH);
  digitalWrite(IN11_PIN, LOW);
 // delay(5000);
 // analogWrite(INA_PIN, 0); // Velocidade do motor
 // analogWrite(INB1_PIN, 0); // Velocidade do motor
 // analogWrite(INA1_PIN, 0); // Velocidade do motor
//  analogWrite(INB_PIN, 0); // Velocidade do motor
//  Serial.println("STOP");
  }

if(n==5){
  Serial.println("ANDAR PARA ESQUERDA");
analogWrite(INB1_PIN, 255); // Velocidade do motor
  analogWrite(INA1_PIN, 255);  
digitalWrite(IN31_PIN, HIGH);
  digitalWrite(IN41_PIN, LOW);
  digitalWrite(IN21_PIN, LOW);
  digitalWrite(IN11_PIN, HIGH);



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
//Serial.println(F("---> recieved events"));
//n = Wire.read();
//Serial.print(numBytes);
//Serial.println(F("bytes recieved"));
//Serial.print(F("recieved value : "));
//Serial.println(n);
//if(n==12){
//  b=0;
//  while(b<50){
//    b=b+1;
//  Serial.println("ANDAR PARA FRENTE - SDSDKVKDVKDMSVMSDMVDPVMPDMVPKSMDVKPMDSKVPDMVPMDPKVMKPVVSBF");
//  analogWrite(INB_PIN, 255); // Velocidade do motor
//  analogWrite(INA_PIN, 255); // Velocidade do motor
//  analogWrite(INB1_PIN, 255); // Velocidade do motor
//  analogWrite(INA1_PIN, 255); // Velocidade do motor
//  digitalWrite(IN3_PIN, HIGH);
//  digitalWrite(IN4_PIN, LOW);
//  digitalWrite(IN2_PIN, LOW);
//  digitalWrite(IN1_PIN, HIGH);
//  digitalWrite(IN31_PIN, HIGH);
//  digitalWrite(IN41_PIN, LOW);
//  digitalWrite(IN21_PIN, HIGH);
//  digitalWrite(IN11_PIN, LOW);
//  }
//  analogWrite(INA_PIN, 0); // Velocidade do motor
//  analogWrite(INB1_PIN, 0); // Velocidade do motor
//  analogWrite(INA1_PIN, 0); // Velocidade do motor
//  analogWrite(INB_PIN, 0); // Velocidade do motor
//  }
//
//  if(n==11){
//   
//    
//  Serial.println("ANDAR PARA TRAS");
//  analogWrite(INB_PIN, 255); // Velocidade do motor
//  analogWrite(INA_PIN, 255); // Velocidade do motor
//  analogWrite(INB1_PIN, 255); // Velocidade do motor
//  analogWrite(INA1_PIN, 255); // Velocidade do motor
//  digitalWrite(IN3_PIN, LOW);
//  digitalWrite(IN4_PIN, HIGH);
//  digitalWrite(IN2_PIN, HIGH);
//  digitalWrite(IN1_PIN, LOW);
//  digitalWrite(IN31_PIN, LOW);
//  digitalWrite(IN41_PIN, HIGH);
//  digitalWrite(IN21_PIN, LOW);
//  digitalWrite(IN11_PIN, HIGH);
// 
//    delay(5000);
//    Serial.println("PARARRRRRRRRRRR");
//  analogWrite(INA_PIN, 0); // Velocidade do motor
//  analogWrite(INB1_PIN, 0); // Velocidade do motor
//  analogWrite(INA1_PIN, 0); // Velocidade do motor
//  analogWrite(INB_PIN, 0); // Velocidade do motor
//  }

}
