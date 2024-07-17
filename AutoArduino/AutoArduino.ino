#include <PS2X_lib.h>

PS2X ps2x;
int error = 0;
byte vibrate = 0;

int IN1 = 36;
int IN2 = 37;
int IN3 = 35;
int IN4 = 34;
int ENA = 13;
int ENB = 12;

void motores(int v, int m1, int m2);

void setup() {
  Serial.begin(9600);

  // Definimos los pines del driver como salidas
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Motores Inicialmente Apagados
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  // Asignamos pines al driver PS2
  delay(300);
  error = ps2x.config_gamepad(48, 50, 49, 51, true, true);  //(clock, command, attention, data)
}

void loop() {
  ps2x.read_gamepad(false, vibrate);

  int m1 = 0;
  int m2 = 0;

  if (ps2x.Button(PSB_L1))  // L1 - Adelante
  {
    Serial.println("Presionaste L1, Movinendo Adelante");
    m1 = 255;
    m2 = 255;
  }
  
  if (ps2x.Button(PSB_R1))  // R1 - Atras
  {
    Serial.println("Presionaste R1, Moviendo Atras");
    m1 = -255;
    m2 = -255;
  }

  if (ps2x.Button(PSB_L2))  // L2 - Izquierda
  {
    Serial.println("Presionaste L2, Moviendo a la Izquierda");
    m1 = (m1 != 0) ? m1 / 2 : 255; // reduce la velocidad del motor derecho si ya está en movimiento
    m2 = (m2 != 0) ? m2 : 0;
  }
  
  if (ps2x.Button(PSB_R2))  // R2 - Derecha
  {
    Serial.println("Presionaste R2, Moviendo a la Derecha");
    m1 = (m1 != 0) ? m1 : 0;
    m2 = (m2 != 0) ? m2 / 2 : 255; // reduce la velocidad del motor izquierdo si ya está en movimiento
  }
  
  // Si no hay botones presionados, detener motores
  if (!ps2x.Button(PSB_L1) && !ps2x.Button(PSB_R1) && !ps2x.Button(PSB_L2) && !ps2x.Button(PSB_R2)) {
    m1 = 0;
    m2 = 0;
  }

  motores((m1 != 0 || m2 != 0) ? (m1 > 0 ? 1 : -1) : 0, abs(m1), abs(m2));
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
