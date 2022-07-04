#include <math.h>                                        // necesaria para utilizar función atan()
#define PI 3.1415926535897932384626433832795             // definición del número PI

int N = 20;                                              // nùmero de ranhuras del encoder
int contadorTicks = 1;                                  // número de ticks para cálculo de velocidad (recordar que entre menor sea el valor mayor ruido de la medida)
int tam = 10;                                           // tamaño del vector del calculo médio (Este valor depende del tamaño de los vectores de promedio vectorL y vectorR)
int k = 10;                                             // tiempo de muestreo

volatile unsigned muestreoActual = 0;                     // variables para definiciòn del tiempo de amostra
volatile unsigned muestreoAnterior = 0;
volatile unsigned deltaMuestreo = 0;

float error = 0;                                        // error variables
float Kp = 40;                                          // Contante proporcional control
int PWMr = 0;                                           // PWM de la roda direita (señal de control roda direita)
int PWMl = 0;                                           // PWM de la roda esquerda (señal de control roda esquerda)

int PWMmax=60;                                          // PWM màximo 
int PWMmin=30;                                           // PWM mìnimo


///------------------------------- Variables Posición del robot---------------------------------------------
float Cdistancia = 0;                                   // distancia recorrido punto central
float x = 0;                                            // distancia recorrida eje X
float y = 0;                                            // distancia recorrida eje Y
float phi = 0;                                          // posición angular

///------------------------------- Variables Posición deseada ---------------------------------------------
float Xd = 100;
float Yd = 100;
float Phid= atan2(Yd-y, Xd-x);

///------------------------------- Variables del robot  ---------------------------------------------

float diametro = 6.8;                                    // diametro de la roda cm
float longitud = 13.4;                                   // longitud del robot entre roda
float V = 0;                                             // Velocidad lineal del carro
float W = 0;                                             // Velocidad Angular del carro

///------------------------------- Variables de motor direito---------------------------------------------

volatile unsigned muestreoActualInterrupcionR = 0;        // variables para definiciòn del tiempo de interrupciòn y calculo de la velocidad motor direito
volatile unsigned muestreoAnteriorInterrupcionR = 0;
double deltaMuestreoInterrupcionR = 0;

int encoderR = 3;   // pin de conexiòn del encoder direito
int llantaR = 11;      // pin de conexiòn de roda direito   (pin de PWM)

double frecuenciaR = 0;                                  // frecuencia de interrupciòn roda R
double Wr = 0;                                           // Velocidad angular R
double Vr = 0;                                           // velocidad Lineal
int CR = 0;                                             // contador ticks
float vectorR[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // vector de almacenamiento de datos para promedio del tiempo de interrupciones

float Rdistancia = 0;                                    // distancia recorrida roda direito
int Rtick = 0;                                           // ticks del encoder direito
int RtickAnt = 0;                                        // ticks del encoder direito anteriores
int deltaRtick = 0;                                      // diferencia del encoder direito

//------------------------------  Variables de motor esquerdo ------------------------------------------------

volatile unsigned muestreoActualInterrupcionL = 0;        // variables para definiciòn del tiempo de interrupciòn y calculo de la velocidad motor Izquierdo
volatile unsigned muestreoAnteriorInterrupcionL = 0;
double deltaMuestreoInterrupcionL = 0;

int encoderL = 2;   // pin de conexiòn del encoder esquerdo
int llantaL = 10;      // pin de conexiòn de llanta esquerdo   (pin de PWM)

double frecuenciaL = 0;                                  // frecuencia de interrupcção roda esquerda
double Wl = 0;                                           // Velocidad angular L
double Vl = 0;                                           // velocidad Lineal
int CL = 0;                                              // contador Ticks
float vectorL[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // vector de almacenamiento de datos para promedio del tiempo de interrupcção

float Ldistancia = 0;                                    // distancia recorrida roda esquerdo
int Ltick = 0;                                           // ticks del encoder esquerdo
int LtickAnt = 0;                                        // ticks del encoder esquerdo anteriores
int deltaLtick = 0;                                      // diferencia del encoder esquerdo

void setup() {
  attachInterrupt(digitalPinToInterrupt(encoderR),REncoder,FALLING);                // linea para añadir una interrupcção a un PIN
  attachInterrupt(digitalPinToInterrupt(encoderL),LEncoder,FALLING);                // linea para añadir una interrupcção a un PIN
  Serial.begin(9600);                                                               // inicio de la comunicaciòn serial
}

void REncoder() {                                                                                         // función de interrupción del enconder roda direita
      Rtick++;                                                                                           // Nùmero de ticks roda direita
      CR++;                                                                                               // incremento del contador de ticks
      if (CR == contadorTicks){                                                                           // si el contador de ticks alcanza el valor de ticks determinado para el cálculo del tiempo
          float media = 0;                                                                                // variable creada para cálculo del promedio  
//-------------------------------------- -----------------------------    Filtro promedio    -----------------------------------------------------------------------------//
          for(int i=0;i < tam-1;i++){                                                                    // relleno del vector para cálculo posterior del promedio
              vectorR[i]=vectorR[i+1];                                                                   
          }
          vectorR[tam-1]=deltaMuestreoInterrupcionR ;                                                     // ùltimo dato del vector (medida actual) 

          for(int i=0;i<tam;i++){                                                                        // Suma de los valores del vector
              media = vectorR[i]+ media;
          }
          media = media/tam;                                                                             //división por el total de datos del vector
          deltaMuestreoInterrupcionR = media;                                                            // se reemplaza por el valor de su medío. 
//-------------------------------------- ----------------------------- ---------------------------------------------------------------------------------------------------//           
          frecuenciaR = (1000)/ deltaMuestreoInterrupcionR;                                              // frecuencia de interrupcção     
          muestreoAnteriorInterrupcionR = muestreoActualInterrupcionR;                                   // se actualiza el tiempo de interrupciòn anterior
          CR = 0;                                                                                        //Reinicio de contador de ticks
      } 
 } 

void LEncoder() {                                                                                       // funciòn de interrupciòn del enconder roda esquerda
      Ltick++;                                                                                           // Nùmero de ticks roda esquerda
      CL++;                                                                                             // incremento del contador de ticks
      if (CL == contadorTicks){                                                                         // si el contador de ticks alcanza el valor de ticks determinado para el cálculo del tiempo
          float media = 0;                                                                              // variable creada para cálculo del promedio
//-------------------------------------- -----------------------------    Filtro promedio    -----------------------------------------------------------------------------//
          for(int i=0;i < tam-1;i++){                                                                    // relleno del vector para calculo posterior del promedio
              vectorL[i]=vectorL[i+1];
          }
          vectorL[tam-1]=deltaMuestreoInterrupcionL;                                                     // último dato del vector (medida actual) 

          for(int i=0;i<tam;i++){                                                                        // Suma de los valores del vector
              media = vectorL[i]+ media;
          }
          media = media/tam;                                                                             //división por el total de datos del vector
          deltaMuestreoInterrupcionL = media;                                                            // se reemplaza por el valor de su medío. 
//-------------------------------------- ----------------------------- ---------------------------------------------------------------------------------------------------//      
          frecuenciaL = (1000)/ deltaMuestreoInterrupcionL;                                              // frecuencia de interrupciòn 
          muestreoAnteriorInterrupcionL = muestreoActualInterrupcionL;                                   // se actualiza el tiempo de interrupciòn anterior
          CL = 0;                                                                                        // Reinicio de contador de ticks
       } 
 } 


void loop() { 
    muestreoActual = millis();                                                                           //Tempo actual de amostra
    muestreoActualInterrupcionR = millis();                                                              // se asigna el tiempo de ejecuciòn a el amostra actual
    muestreoActualInterrupcionL = millis();                                                              // se asigna el tiempo de ejecuciòn a el amostra actual

    deltaMuestreo =(double) muestreoActual - muestreoAnterior;                                           // delta de muestreo 
    if ( deltaMuestreo >= k)                                                                             // se asegura el tiempo de amostra
    {   
        float Phid= atan2(Yd-y, Xd-x);                                                                   // Recalcular el ángulo deseado en cada iteración, dado que el cambia con respecto  a cada movimento
         
        deltaMuestreoInterrupcionR = muestreoActualInterrupcionR -  muestreoAnteriorInterrupcionR;       // diferencia tiempos de interruciones de ticks do motor     
        deltaMuestreoInterrupcionL = muestreoActualInterrupcionL -  muestreoAnteriorInterrupcionL;       // diferencia tiempos de interruciones de ticks do motor     

        if(deltaMuestreoInterrupcionR >= 200*contadorTicks){                                              // Esta es la forma de definir cuando el motor se encuentra quieto. Si deltaMuestreoInterrupcionR es mayor a 40 milisegundos por el preescalado de ticks
          frecuenciaR=0;                                                                                  // 40 mS es el tiempo que màximo se tarda un tick a la menor velocidad del motor
        }
        if(deltaMuestreoInterrupcionL >= 200*contadorTicks){                                              // Esta es la forma de definir cuando el motor se encuentra quieto. Si deltaMuestreoInterrupcionR es mayor a 40 milisegundos por el preescalado de ticks
          frecuenciaL=0;                                                                                  // 40 mS es el tiempo que màximo se tarda un tick a la menor velocidad del motor
        }

        Wr = contadorTicks*((2*PI)/N)*frecuenciaR;                                                        // frecuencia angular Rad/s
        Vr= Wr*(diametro/2);                                                                              // velocidad lineal cm/s
        Wl = contadorTicks*((2*PI)/N)*frecuenciaL;                                                        // frecuencia angular Rad/s
        Vl= Wl*(diametro/2);                                                                              // velocidad lineal cm/s    

//        V = (Vr+Vl)/2;                                                                                    // calculo da velocidade do robot
        V = 50;                                                                                           // velocidad constante para alcanzar el àngulo
        error = Phid - phi;                                                                               // error angular Angulo deseado menos el angulo del robot
        W = (Vr-Vl)/longitud + Kp * error;                                                                // Càlculo de la velocidad angular con las variables de control
        PWMr = V + (W*longitud)/2;                                                                        // Señal de control PWM llanta derecha
        PWMl = V - (W*longitud)/2;                                                                        // Señal de control PWM roda esquerda

//-------------------------------------- condicionales para limites de la señal de PWM ---------------------------------------------------------------------------------------------------//  

        if(PWMr > PWMmax){                                                                               
           PWMr = PWMmax;
        }
        if(PWMr < PWMmin){
          PWMr = PWMmin;
        }
        if(PWMl > PWMmax){
           PWMl = PWMmax;
        }
        if(PWMl < PWMmin){
           PWMl = PWMmin;
        } 

        if( abs(x-Xd) < 5 && abs(y-Yd) < 5){
          analogWrite(llantaR,0);
          analogWrite(llantaL,0); 
        }
        else {
          analogWrite(llantaR,PWMr);
          analogWrite(llantaL,PWMl); 
       }
        
//        analogWrite(llantaR,PWMr);                                                                           // PWM de la roda direita
//        analogWrite(llantaL,PWMl);                                                                           // PWM de la roda esquerda

//        analogWrite(llantaR,0);                                                                           // PWM de la roda direita
//        analogWrite(llantaL,0);                                                                           // PWM de la roda esquerda
        
        odometria();                                                                                      // cálculo de la odometría                 
        
        Serial.print(x);                                                                                  // se muestra el tiempo entre TIC y TIC
        Serial.print(",");                                                                                 // se muestra el tiempo entre TIC y TIC
        Serial.println(y);                                                                                // se muestra el tiempo entre TIC y TIC
       
        muestreoAnterior = muestreoActual;                                                                 // actualización del muestreo anterior
     }
}

void odometria(){ 

   deltaRtick = Rtick - RtickAnt;                                                                         // comparación de los ticks recorridos desde el último cálculo roda direita              
   Rdistancia = PI*diametro*(deltaRtick/(double) 20);                                                     // distancia recorrida por la roda direita desde el último cálculo

   deltaLtick = Ltick - LtickAnt;                                                                         // comparación de los ticks recorridos desde el último cálculo roda esquerda     
   Ldistancia = PI*diametro*(deltaLtick/(double) 20);                                                     // distancia recorrida por la roda esquerda desde el último cálculo   

   Cdistancia = (Rdistancia + Ldistancia)/2;                                                               // distancia recorrida por el punto central desde el último cálculo  

   x = x + Cdistancia*cos(phi);                                                                            // posición del punto X actual
   y = y + Cdistancia*sin(phi);                                                                            // posición del punto Y actual
   
   phi = phi + ((Rdistancia - Ldistancia)/longitud);                                                       // posición Angular actual
   phi = atan2(sin(phi),cos(phi));                                                                         //transformación de la posición angular entre -PI y PI

   RtickAnt = Rtick;                                                                                       // actualización de la variable RtickAnt con los valores de Rtick
   LtickAnt = Ltick;                                                                                       // actualización de la variable LtickAnt con los valores de Ltick
 } 
