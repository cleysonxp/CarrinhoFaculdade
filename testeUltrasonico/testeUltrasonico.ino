#define TRIG 3
#define ECHO 2
#define LED_VERDE 11
#define LED_VERMELHO 12

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  Serial.begin(9600);

  // Liga o LED verde ao iniciar
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);
}

void loop() {
  long distancia = medirDistancia();

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia <= 10 && distancia > 0) {
    digitalWrite(LED_VERMELHO, HIGH);  // Liga o LED vermelho
    digitalWrite(LED_VERDE, LOW);      // Apaga o LED verde
  } else {
    digitalWrite(LED_VERMELHO, LOW);   // Desliga o LED vermelho
    digitalWrite(LED_VERDE, HIGH);     // Liga o LED verde
  }

  delay(200); // pequena pausa para estabilidade
}

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
