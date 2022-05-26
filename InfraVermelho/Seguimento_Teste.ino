//Definição dos pinos de controlo do motor
#define ME 10                //Pino_Velocidade Motor Esquerda (0 a 255);
#define MD 9                 //Pino_Velocidade Motor Direita (0 a 255);
#define dirE 12              //Pino_Direção do Motor Esquerdo: Para frente HIGH;
#define ndirE 11             //Pino_Direção do Motor Esquerdo: Para frente LOW;
#define dirD 8               //Pino_Direção do Motor Direito: Para frente HIGH;
#define ndirD 13             //Pino_Direção do Motor Direito: Para frente LOW;

//Definição dos pinos dos sensores
#define pin_SE 7
#define pin_SD 6
bool SensorE = 0;
bool SensorD = 0;

//variáveis responsáveis por controlar a velocidade dos motores
int velocidade = 150;
int velocidade_min = 75;

//variável responsável por definir a diferença entre preto e branco
//menor que color_threshold -> branco; maior que color_threshold -> preto
int color_threshold = 350;

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
  
  //Aqui está toda a lógica de comportamento do robô:
  if((SensorE < color_threshold ) && (SensorD < color_threshhold)){ //Ambos os Sensores detetam cor branca, podem andar para a frente
    analogWrite(ME, velocidade); // Ambos motores ligam na mesma velocidade
    analogWrite(MD, velocidade);
    Serial.println("bb"); // informar que estão brancos os dois
  }
  
  else if((SensorE < color_threshold ) && (SensorD > color_threshold)){ // Esquerdo branco e Direito preto
    analogWrite(ME, velocidade_min); // O motor esquerdo desliga
    analogWrite(MD, velocidade); // O motor direito fica ligado, fazendo assim o carrinho virar
    Serial.println("bp");
  }
  
  else if((SensorE > color_threshold) && (SensorD < color_threshold)){ // Esquerdo preto e Direito branco
    analogWrite(ME, velocidade); //O motor esquerdo fica ligado
    analogWrite(MD, velocidade_min); // O motor direito desliga, fazendo assim o carrinho virar no outro sentido
    Serial.println("pb");
  }
  
  else if((SensorE > color_threshold) && (SensorD > color_threshold)){ // Ambos os sensores detetam preto
    analogWrite(ME, 0); // ambos os motores desligam
    analogWrite(MD, 0); 
    Serial.println("pp");
  }
}
