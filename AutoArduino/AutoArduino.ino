#include <PS2X_lib.h>

PS2X ps2x;
int error = 0;
byte vibrate = 0;

int IN1 = 13;
int IN2 = 12;
int IN3 = 9;
int IN4 = 8;
int ENA = 11;
int ENB = 10;

void motores(int v, int m1, int m2);

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Asegúrate de que los motores están apagados al iniciar
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  delay(300);
  error = ps2x.config_gamepad(4, 6, 5, 7, true, true);  //(clock, command, attention, data)
}

void loop() {
  ps2x.read_gamepad(false, vibrate);

  if (ps2x.Button(PSB_GREEN))  // Triangulo - Adelante
  {
    Serial.println("Presionaste triangulo, Movinendo Adelante");
    motores(1, 255, 255);
  } else if (ps2x.Button(PSB_BLUE))  // X - Atras
  {
    Serial.println("Presionaste X, Moviendo Atras");
    motores(-1, 255, 255);
  } else if (ps2x.Button(PSB_PINK))  // Cuadrado -Izquierda
  {
    Serial.println("Presionaste cuadrado, Moviendo a la Izquierda");
    motores(1, 255, 0);              
  } else if (ps2x.Button(PSB_RED))  // Circulo - Derecha
  {
    Serial.println("Presionaste circulo, moviendo a la Derecha");
    motores(1, 0, 255);  
  } else {
    motores(0, 0, 0);  
  }

  delay(100);
}

void motores(int v, int m1, int m2) {
  if (v == 1) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (v == -1) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  analogWrite(ENA, m1);
  analogWrite(ENB, m2);
}