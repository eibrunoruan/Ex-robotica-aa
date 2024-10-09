// Definições de portas para motor, sensores e LED RGB
#define PININ1 2
#define PININ2 4
#define PININ3 5
#define PININ4 7
#define PINENA 3
#define PINENB 6

#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define S5 A4
#define S6 A5

// Parâmetros de configuração
#define TRESHOLD 700
#define SPEED0 255
#define SPEED1 220
#define SPEED2 150
#define SPEED3 100
#define SPEED4 80
#define SPEED5 50
#define SPEED6 0
#define SPEED7 200
#define RUNTIME 15500

void setup() {
  Serial.begin(9600);
}

void loop() {
  followLineMEF(); // Função principal de controle do seguidor de linha
}

void motorControl(int speedLeft, int speedRight) {
  pinMode(PININ1, OUTPUT);
  pinMode(PININ2, OUTPUT);
  pinMode(PININ3, OUTPUT);
  pinMode(PININ4, OUTPUT);
  pinMode(PINENA, OUTPUT);
  pinMode(PINENB, OUTPUT);

  // Ajuste do motor esquerdo
  if (speedLeft <= 0) {
    speedLeft = -speedLeft;
    digitalWrite(PININ3, HIGH);
    digitalWrite(PININ4, LOW);
  } else {
    digitalWrite(PININ3, LOW);
    digitalWrite(PININ4, HIGH);
  }

  // Ajuste do motor direito
  if (speedRight < 0) {
    speedRight = -speedRight;
    digitalWrite(PININ1, LOW);
    digitalWrite(PININ2, HIGH);
  } else {
    digitalWrite(PININ1, HIGH);
    digitalWrite(PININ2, LOW);
  }

  analogWrite(PINENA, speedLeft);
  analogWrite(PINENB, speedRight);
}

void motorOption(char option, int speedLeft, int speedRight) {
  switch (option) {
    case '8': // Frente
      motorControl(-speedLeft, speedRight);
      break;
    case '4': // Esquerda
      motorControl(-speedLeft, -speedRight);
      break;
    case '6': // Direita
      motorControl(speedLeft, speedRight);
      break;
    case '0': // Parar
      motorControl(0, 0);
      break;
  }
}

void readSensors(int *sensors) {
  sensors[0] = analogRead(S1);
  sensors[1] = analogRead(S2);
  sensors[2] = analogRead(S3);
  sensors[3] = analogRead(S4);
  sensors[4] = analogRead(S5);
  sensors[5] = analogRead(S6);
}

void followLineMEF(void) {
  int s[6];
  readSensors(s);

  if (s[0] <= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] <= TRESHOLD) {
    motorOption('8', SPEED0, SPEED0); // Seguir em frente
  } else if (s[0] >= TRESHOLD && s[5] >= TRESHOLD) {
    motorOption('8', SPEED0, SPEED0); // Seguir em frente
  } else if (s[2] <= TRESHOLD && s[3] <= TRESHOLD) {
    motorOption('8', SPEED0, SPEED0); // Seguir em frente
  } else if (s[1] <= TRESHOLD && s[4] <= TRESHOLD) {
    motorOption('8', SPEED0, SPEED1); // Pequeno ajuste para seguir em frente
  } else if (s[2] <= TRESHOLD && s[3] >= TRESHOLD) {
    motorOption('8', SPEED0, SPEED2); // Correção leve à direita
  } else if (s[3] <= TRESHOLD && s[2] >= TRESHOLD) {
    motorOption('8', SPEED2, SPEED0); // Correção leve à esquerda
  } else if (s[0] <= TRESHOLD) {
    motorOption('6', SPEED7, SPEED7); // Giro à direita
  } else if (s[5] <= TRESHOLD) {
    motorOption('4', SPEED7, SPEED7); // Giro à esquerda
  } else {
    motorOption('0', 0, 0); // Parar
  }
}
