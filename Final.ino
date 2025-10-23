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

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);

  // Liga o LED verde por 5 segundos ao iniciar
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);
  delay(5000);

  // Desliga LED verde e inicia o carrinho
  digitalWrite(LED_VERDE, LOW);
  andarFrente();
}

void loop() {
  long distancia = medirDistancia();

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia <= 10 && distancia > 0) {
    pararMotores();
    digitalWrite(LED_VERMELHO, HIGH);  // Liga o LED vermelho
    digitalWrite(LED_VERDE, LOW);      // Garante LED verde apagado
  } else {
    andarFrente();
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_VERDE, LOW);      // Opcional: pode deixar apagado enquanto anda
  }

  delay(100); // Pequena pausa
}

// Função para medir distância
long medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  long distancia = duracao * 0.034 / 2; // Converte para cm
  return distancia;
}

// Função para andar para frente
void andarFrente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 70); // Ajuste a velocidade se necessário

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 70); // Ajuste a velocidade se necessário
}

// Função para parar motores
void pararMotores() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
