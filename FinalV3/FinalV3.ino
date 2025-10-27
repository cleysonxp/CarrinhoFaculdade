// Autor: Cleyson Soares
// Projeto: Carrinho seguidor de linha com RGB, IR e Ultrassônico
// Versão: 4.0 (integração final completa)
// Data: 23/10/2025

// ---------- Motores ----------
#define ENA 9
#define IN1 8
#define IN2 7
#define ENB 10
#define IN3 6
#define IN4 5

// ---------- Sensor Ultrassônico ----------
#define TRIG 3
#define ECHO 2

// ---------- LEDs ----------
#define LED_VERDE 11
#define LED_VERMELHO 12

// ---------- Sensor RGB TCS230 ----------
#define S0 A0
#define S1 A1
#define S2 A2
#define S3 A3
#define OUT 4

// ---------- Sensores Infravermelhos ----------
#define IR_ESQ A4
#define IR_DIR A5

// ---------- Variáveis ----------
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// ---------- Valores de calibração ----------
int RED_REF = 100;
int GREEN_REF = 100;
int BLUE_REF = 100;

bool fimPercurso = false;

// ---------- Funções ----------
void andarFrente();
void pararMotores();
void virarEsquerda();
void virarDireita();
long medirDistancia();
int readChannel(bool s2, bool s3);
String detectarCor();
void seguirLinha();
void calibrarSensor();

void setup() {
  // Sensores ultrassônico
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // LEDs
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // Motores
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensor RGB
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Sensores IR
  pinMode(IR_ESQ, INPUT);
  pinMode(IR_DIR, INPUT);

  Serial.begin(9600);
  Serial.println("=== Start Veículo Autônomo ===");

  //Calibração automatica do sensor TCS230
  //calibrarSensor();

  // Espera cor verde para começar
  String corDetectada = "";
  do {
    corDetectada = detectarCor();
    Serial.print("Detectando, aguardando cor verde para o inicio: ");
    Serial.println(corDetectada);
    Serial.print(" R:");
    Serial.print(redFrequency);
    Serial.print(" G:");
    Serial.print(greenFrequency);
    Serial.print(" B:");
    Serial.println(blueFrequency);
    delay(500);
  } while (corDetectada != "VERDE");

  digitalWrite(LED_VERDE, HIGH);
  delay(5000);
  digitalWrite(LED_VERDE, LOW);
  andarFrente();
}

void loop() {
  String cor = detectarCor();
  long distancia = medirDistancia();

  Serial.print("Cor: ");
  Serial.print(cor);
  Serial.print(" | Distância: ");
  Serial.print(distancia);
  Serial.print(" cm | R:");
  Serial.print(redFrequency);
  Serial.print(" G:");
  Serial.print(greenFrequency);
  Serial.print(" B:");
  Serial.println(blueFrequency);

  if (fimPercurso) {
    pararMotores();
    digitalWrite(LED_VERMELHO, HIGH);

    if (cor == "VERDE") {
      fimPercurso = false;
      digitalWrite(LED_VERDE, HIGH);
      delay(5000);
      digitalWrite(LED_VERDE, LOW);
    }

    delay(100);
    return;
  }

  // Evita colisão
  if (distancia <= 10 && distancia > 0) {
    pararMotores();
    digitalWrite(LED_VERMELHO, HIGH);
    delay(500);
  }
  else if (cor == "VERMELHO") {
    pararMotores();
    digitalWrite(LED_VERMELHO, HIGH);
    delay(5000); // fica parado 5 segundos
    digitalWrite(LED_VERMELHO, LOW);
    fimPercurso = true;
  }
  else if (cor == "AMARELO") {
    pararMotores();

    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_VERMELHO, HIGH);
      delay(500);
      digitalWrite(LED_VERMELHO, LOW);
      delay(500);
    }    
    seguirLinha();
  }
  else if (cor == "VERDE") {
    digitalWrite(LED_VERMELHO, LOW);
    seguirLinha();
  }
  else {
    digitalWrite(LED_VERMELHO, LOW);
    seguirLinha();
  }
  delay(100);
}

// ---------- Funções auxiliares ----------
long medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duracao = pulseIn(ECHO, HIGH);
  return duracao * 0.034 / 2;
}

void andarFrente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 80);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 80);
}

void pararMotores() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void virarEsquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 70);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 70);
}

void virarDireita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 70);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 70);
}

void seguirLinha() {
  int esquerda = digitalRead(IR_ESQ);
  int direita = digitalRead(IR_DIR);

  if (esquerda == LOW && direita == LOW) {
    andarFrente();
  } else if (esquerda == LOW && direita == HIGH) {
    virarEsquerda();
  } else if (esquerda == HIGH && direita == LOW) {
    virarDireita();
  } else {
    pararMotores();
  }
}

int readChannel(bool s2, bool s3) {
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  delayMicroseconds(200);
  unsigned long soma = 0;
  for (int i = 0; i < 5; i++) soma += pulseIn(OUT, LOW, 100000);
  return soma / 5;
}

void calibrarSensor() {
  Serial.println("=== CALIBRAÇÃO DE BRANCO ===");
  Serial.println("Posicione o sensor sobre branco puro...");
  delay(3000);
  
  RED_REF = readChannel(LOW, LOW);
  GREEN_REF = readChannel(HIGH, HIGH);
  BLUE_REF = readChannel(LOW, HIGH);
  
  Serial.print("Calibração concluída - R:");
  Serial.print(RED_REF);
  Serial.print(" G:");
  Serial.print(GREEN_REF);
  Serial.print(" B:");
  Serial.println(BLUE_REF);
  delay(2000);
}

String detectarCor() {
  redFrequency   = readChannel(LOW,  LOW);
  greenFrequency = readChannel(HIGH, HIGH);
  blueFrequency  = readChannel(LOW,  HIGH);

  int R = redFrequency;
  int G = greenFrequency;
  int B = blueFrequency;

  bool isYellow = (R < 20 && G > 20 && G < 30 && B > 35);
  bool isRed = (G > 50 && R < 25 && B < 45);
  bool isGreen = (R > 35 && R < 45 && G > 33 && G < 42 && B > 45);

  if (isGreen) {
    return "VERDE";
  } 
  else if (isRed) {
    return "VERMELHO";
  } 
  else if (isYellow) {
    return "AMARELO";
  } 
  else {
    return "NENHUMA";
  }
}