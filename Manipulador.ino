////////////////////////////////////////////////
// MANIPULADOR
///////////////////////////////////////////////
#include<Servo.h>

Servo Ponta;
Servo Meio;
Servo Base;

int posvel = 100; // velocidade positiva
int negvel = 80; // velocidade negativa
int zerovel = 90; // velocidade nula

//Ponta
#define Pino_Ponta 1 // Pino do Servo do Manipulador que está na ponta
#define Pino_Meio 2 // Pino do Servo do Manipulador que está no meio
#define Pino_Base 3 // Pino do Servo do Manipulador que está na base
#define Pino_Rele 4 // Pino do Eletroíman 

void setup() {


  Ponta.attach(Pino_Ponta);
  Meio.attach(Pino_Meio);
  Base.attach(Pino_Base);

  pinMode(Pino_Ponta, OUTPUT);
  pinMode(Pino_Meio, OUTPUT);
  pinMode(Pino_Base, OUTPUT);
  pinMode(Pino_Rele, OUTPUT);
}
void loop() {

  ///////// PONTA //////////
  if (currentLine.endsWith("GET /CE")) {
    Ponta.write(posvel);
  }
  elseif(currentLine.endsWith("GET /CD")) {
    Ponta.write(negvel);
  }
  elseif(currentLine.endsWith("GET /CS")) {
    Ponta.write(zerovel);
  }

  ////////// MEIO /////////
  if (currentLine.endsWith("GET /ME")) {
    Meio.write(posvel);
  }
  elseif (currentLine.endsWith("GET /MD")) {
    Meio.write(negvel);
  }
  elseif (currentLine.endsWith("GET /MS")) {
    Meio.write(zerovel);
  }

  ////////// Base /////////
  if (currentLine.endsWith("GET /BE")) {
    Base.write(posvel);
  }
  elseif (currentLine.endsWith("GET /BD")) {
    Base.write(negvel);
  }
  elseif (currentLine.endsWith("GET /BS")) {
    Base.write(zerovel);
  }

  //////// Eletroiman /////////
  if (currentLine.endsWith("GET /IU")) {
    digitalWrite(Pino_Rele, 1);
  }
  elseif(currentLine.endsWith("GET /IZ")) {
    digitalWrite(Pino_Rele, 0);
  }

}
