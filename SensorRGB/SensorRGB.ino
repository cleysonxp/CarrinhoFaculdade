// Autor: Cleyson Soares
// Projeto: Teste Sensor RGB TCS230 com LEDs
// Versão: 2.1 (Correção da confusão verde x amarelo)
// Data: 23/10/2025

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define OUT 8

#define LED_VERMELHO 10
#define LED_VERDE 11

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int readChannel(bool s2, bool s3) {
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  delayMicroseconds(200);
  unsigned long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += pulseIn(OUT, LOW, 100000);
  }
  return sum / 5;
}

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);
  Serial.println("=== Teste Sensor RGB TCS230 (versão 2.1) ===");
}

void loop() {
  // Leituras médias
  redFrequency   = readChannel(LOW,  LOW);
  greenFrequency = readChannel(HIGH, HIGH);
  blueFrequency  = readChannel(LOW,  HIGH);

  int R = redFrequency;
  int G = greenFrequency;
  int B = blueFrequency;

  Serial.print("R="); Serial.print(R);
  Serial.print(" G="); Serial.print(G);
  Serial.print(" B="); Serial.println(B);

  int diffRG = abs(R - G);
  int mediaRGB = (R + G + B) / 3;

  // Detecta cores
  bool isYellow = (diffRG <= 10 && B >= max(R, G) + 12 && mediaRGB < 30);
  bool isGreen  = (G <= R - 3 && G <= B - 3) || (mediaRGB >= 30 && diffRG <= 6 && B >= G + 10);
  bool isRed    = (R <= G - 4 && R <= B - 4);

  if (isYellow) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, HIGH);
    Serial.println("AMARELO");
  } 
  else if (isRed) {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_VERDE, LOW);
    Serial.println("VERMELHO");
  } 
  else if (isGreen) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    Serial.println("VERDE");
  } 
  else {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    Serial.println("NENHUMA COR");
  }

  delay(300);
}
