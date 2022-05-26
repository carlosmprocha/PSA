//Definição dos pinos de controlo do motor
#define ME 9 // Pino_Velocidade Motor Esquerda ( 0 a 255);
#define MD 11 //Pino_Velocidade Motor Direita ( 0 a 255);
#define dirE 8 //Pino_Direção do Motor Esquerdo: Para frente (HIGH);
#define dirD 10 //Pino_Direção do Motor Direito: Para frente / Para trás (HIGH ou LOW);
#define ndirE 11 //Pino_Direção do Motor Esquerdo: Para trás (LOW);
#define ndirD 13//Pino_Direção do Motor Direito: Para trás (LOW)

//Definição do pino do sensor da parede
#define pin_SP1 7 // Sensor de Parede da Frente
#define pin_SP2 6 // Sensor de Parede de Trás
bool SensorP1 = 0;
bool SensorP2 = 0;

//variável responsável por controlar a velocidade dos motores
int velocidade = 150;
int velocidade_min = 75;

//variável responsável por definir a diferença entre preto e branco
//menor que dist_threshold -> perto; maior que dist_threshold -> longe
int dist_threshold = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //pinos de controlo dos motores como saída
  pinMode(ME, OUTPUT);
  pinMode(MD, OUTPUT);
  pinMode(dirE, OUTPUT);
  pinMode(dirD, OUTPUT);
  pinMode(ndirE, OUTPUT);
  pinMode(ndirD, OUTPUT);

  //direção inicial do motor
  digitalWrite(dirE, HIGH); //carro anda para frente se ME tiver mais que 0
  digitalWrite(dirD, HIGH);
  digitalWrite(ndirE, LOW);
  digitalWrite(ndirD, LOW);
  analogWrite(ME, 0); //O motor esquerdo fica ligado
  analogWrite(MD, 0);
    
  //pinos dos sensores como entrada
  pinMode(pin_SP1, INPUT);
  pinMode(pin_SP2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Neste processo armazenamos o valor lido pelo sensor na variável que armazena tais dados.
  SensorP1 = analogRead(pin_SP1);
  SensorP2 = analogRead(pin_SP2);
  
  //Aqui está toda a lógica de comportamento do robô:
  if((SensorP1 < dist_threshold) && (SensorP2 < dist_threshhold)){ // Ambos os sensores detetam perto, pode seguir em frente
  analogWrite(ME, velocidade); // Ambos motores ligam na mesma velocidade
  analogWrite(MD, velocidade);
  Serial.println("pp"); // perto perto
  }
  
  else if((SensorP1 < dist_threshold) && (SensorP2 > dist_threshhold)){ // SensorP1 está perto e Sensor P2 está longe
    analogWrite(ME, 0); // O motor esquerdo desliga
    analogWrite(MD, velocidade); // O motor direito fica ligado, fazendo assim o carrinho virar
    Serial.println("pl");
  }
  
  else if((SensorP1 > dist_threshold) && (SensorP2 < dist_threshold)){ // SensorP1 está longe e SensorP2 está perto
    analogWrite(ME, velocidade); // O motor esquerdo fica ligado, fazendo assim o carrinho virar
    analogWrite(MD, 0); // O motor direito desliga
    Serial.println("lp");
  }
  
  else if((SensorP1 > dist_threshold) && (SensorP2 > dist_threshold)){ // Ambos os sensores detetam longe
    analogWrite(ME, 0); // ambos os motores desligam
    analogWrite(MD, 0); 
    Serial.println("ll"); // longe longe
  }
}
