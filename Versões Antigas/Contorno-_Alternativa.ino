/////// Contorno de Obstáculos - Alternativa //////

boolean IV_Front = false; // Infravermelho que estará na ponta frontal lateral
boolean IV_Back = false; // Infravermelho que estará na ponta traseira lateral
boolean IV_Cor = false;  // Infravermelhos da frente que detetam a linha
boolean obstacle = false; // Um obstáculo é detetado pelo US

// não sei fazer esta parte //

void setup() {
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //   O Ultrassom deteta um obstáculo
  if (obstacle == true) {

    // Parar
    Serial.println("OBST");
    moveStop();
    delay(timeStop);

    //  Rodar para a direita
    turnRight();
    if (IV_Back == true){
      moveStop();
    }

    // Andar para a Frente
    moveForward();
    if (IV_Back == false){
      moveStop();
      delay(timeStop);
    }

    // Rodar para a Esquerda
    turnLeft();
    if(IV_Front == true){
      moveStop();
      delay(timeStop)
    }

    // Andar para a frente
    moveForward();
    if (IV_Back == false){
      moveStop();
      delay(timeStop);
   }

   // Rodar Para a Esquerda
   turnLeft();
   if(IV_Front == true){
      moveStop();
      delay(timeStop)
  }

    // Andar para a frente até detetar a linha
    moveForward();
    if (IV_Cor == True){
      moveStop();
      delay(timeStop);
   }

   // Rodar para a Direita
   turnRight();
   if (IV_Back == True){
    moveStop();
    delay(timeStop);
    moveForward();
   }
}
