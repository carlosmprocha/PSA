//Definição dos pinos de controlo do motor
#define ME 10 // Pino_Velocidade Motor Esquerda ( 0 a 255);
#define MD 9 //Pino_Velocidade Motor Direita ( 0 a 255);
#define dirE 12 //Pino_Direção do Motor Esquerdo: Para frente / Para trás (HIGH ou LOW);
#define ndirE 11
#define dirD 8 //Pino_Direção do Motor Direito: Para frente / Para trás (HIGH ou LOW);
#define ndirD 13

//Definição dos pinos dos sensores
#define pin_SE 7
#define pin_SD 6
bool SensorE = 0;
bool SensorD = 0;

//variável responsável por controlar a velocidade dos motores
int velocidade = 150;
int velocidade_min = 75;
int color_threshhold = 100;

void setup() {
  // put your setup code here, to run once:
  //Setamos os pinos de controle dos motores como saída
  pinMode(ME, OUTPUT);
  pinMode(MD, OUTPUT);
  pinMode(dirE, OUTPUT);
  pinMode(dirD, OUTPUT);
  pinMode(ndirE, OUTPUT);
  pinMode(ndirD, OUTPUT);
  
  //Setamos a direção inicial do motor como 0, isso fará com que ambos os motores girem para frente
  digitalWrite(dirE, LOW);
  digitalWrite(dirD, LOW);
  digitalWrite(ndirE, LOW);
  digitalWrite(ndirD, LOW);
  
  //Setamos os pinos dos sensores como entrada
  pinMode(pin_SE, INPUT);
  pinMode(pin_SD, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Neste processo armazenamos o valor lido pelo sensor na variável que armazena tais dados.
  SensorE = analogRead(pin_SE);
  SensorD = analogRead(pin_SD);
  
  //Aqui está toda a lógica de comportamento do robô: Para a cor branca atribuímos o valor 0 e, para a cor preta, o valor 1.
  if((SensorE < color_threshhold ) && (SensorD < color_threshhold)){ // Se detectar na extremidade das faixas duas cores brancas
  analogWrite(ME, velocidade); // Ambos motores ligam na mesma velocidade
  analogWrite(MD, velocidade);
  }
  
  if((SensorE < color_threshhold ) && (SensorD > color_threshhold)){ // Se detectar um lado preto e o outro branco
  analogWrite(ME, velocidade_min); // O motor esquerdo desliga
  analogWrite(MD, velocidade); // O motor direito fica ligado, fazendo assim o carrinho virar
  }
  
  if((SensorE > color_threshhold) && (SensorD < color_threshhold)){ // Se detectar um lado branco e o outro preto
  analogWrite(ME, velocidade); //O motor esquerdo fica ligado
  analogWrite(MD, velocidade_min); // O motor direita desliga, fazendo assim o carrinho virar no outro sentido
  }

}
