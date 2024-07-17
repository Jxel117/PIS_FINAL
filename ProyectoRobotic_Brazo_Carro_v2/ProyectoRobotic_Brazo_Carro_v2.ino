#include <HX711.h>

#include <PS2X_lib.h>

#include <Key.h>
#include <Keypad.h>

#include <Servo.h>
// Test de Servos
Servo servoBase, servoCodo, servoAntebrazo, servoPinza;

// Mando
PS2X ps2x;
int error = 0;
byte vibrate = 0;

// Pines Servos PWM
int servoPinB = 7, ang_ServoB = 90, valorAngB = 0;  //Servo Base 0 - 180 //
int servoPinA = 3, ang_ServoA = 0, valorAngA;       //Servo Antebrazo
int servoPinC = 4, ang_ServoC = 180, valorAngC;      //Servo Codo
int servoPinP = 6, ang_ServoP = 0, valorAngP;       //Servo Pinza //


// Controlador de encenderMotor
// IN1 & IN2 son pines de entrada de modulo L28N para motor 1
int IN1 = 36;
int IN2 = 37;
// IN3 & IN4 son pines de entrada de modulo L28N para motor 2
int IN3 = 35;
int IN4 = 34;

// Signal PWM for Motor 1
int ENA = 13;
// Signal PWM for Motor 2
int ENB = 12;

int analog_right_eje_y;

// Variable para girara a la izquierda o derecha de los motor
int aux;
// Variable de velocidad
int velocidad;
// Analogo izquierdo para giro con el analogo o eje x
int analog_left_eje_x;

// Variables balanza - digitales
int pinDT = 38;
int pinSCK = 8; //pwm
HX711 celda;


String lastMessage = "";  // Variable para almacenar el último mensaje enviado
String message = "";      // Variable para almacenar el mensaje enviado



void encenderMotor(int v, int m1, int m2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoAntebrazo.attach(servoPinA, 500, 2400);  //0 - 80
  servoCodo.attach(servoPinC, 500, 2400);//180-90
  servoPinza.attach(servoPinP, 500, 2400);  // 0 - 75
  servoBase.attach(servoPinB, 500, 2400); // 0 -180 

  error = ps2x.config_gamepad(48, 50, 49, 51, true, true);  //(clock, command, attention, data)

  // Driver L298N
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // inicializar motores
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  // verificacionConexionMando();

  // Celda de carga
  celda.begin(pinDT,pinSCK);
  celda.set_scale(2213.f);
  celda.tare();
}

void loop() {
  servoAntebrazo.write(ang_ServoA);
  servoCodo.write(ang_ServoC);
  servoPinza.write(ang_ServoP);
  servoBase.write(ang_ServoB);
  // actualiza el valor de todos los botones y joysticks analogicos
  ps2x.read_gamepad(false, vibrate);


  // testJoystick();
  // testServoMotores();
  
  // Controla Carro
  // controlarCarro();
  controlarCarro_2();
  
  // Controla Brazo Robotico
  movimientoServoAntebrazo();
  movimientoServoBase();
  movimientoServoCodo();
  movimientoServoPinza();
  // pesarObjetos();
}

void pesarObjetos(){
  Serial.print("Valor gramos: ");
  Serial.println(celda.get_units(10),1);
  // Apaga celda
  celda.power_down();
  delay(3000);
  // Enciende celda
  celda.power_up();
}

// Verificacion conexion mando
void verificacionConexionMando() {
  if (error == 0) {
    message = "Mando de PS2 conectado y configurado correctamente.";
  } else if (error == 1) {
    message = "Error al inicializar el mando de PS2. Asegúrate de que está correctamente conectado y trata de nuevo.";
  } else if (error == 2) {
    message = "El mando de PS2 no se encontró. Asegúrate de que está conectado y que el cableado es correcto.";
  } else {
    message = "Error desconocido al intentar conectar el mando de PS2.";
  }
  impresionMensaje();
}

// Impresion de Mensaje
void impresionMensaje() {
  if (message != lastMessage) {
    Serial.println(message);
    lastMessage = message;
  }
}

// Test de botones
void testJoystick() {
  // Copiar todo el codigo para testear funcionamiento del joystick
  if (ps2x.Button(PSB_PAD_LEFT)) {
    Serial.println("Presionaste el boton a la izquierda...");
  } else if (ps2x.Button(PSB_PAD_RIGHT)) {
    Serial.println("Presionaste el boton a la derecha...");
  }

  if (ps2x.Button(PSB_PAD_UP)) {
    Serial.println("Presionaste el boton arriba...");
    Serial.println(ps2x.Button(PSB_PAD_UP));
  } else if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.println("Presionaste el boton abajo..");
    Serial.println(ps2x.Button(PSB_PAD_DOWN), DEC);
  }

  if (ps2x.Button(PSB_L1)) {
    Serial.println("Presionaste el boton L1...");
    Serial.println(ps2x.Button(PSB_L1), DEC);
  } else if (ps2x.Button(PSB_R1)) {
    Serial.println("Presionaste el boton R1.");
    Serial.println(ps2x.Button(PSB_R1), DEC);
  }

  if (ps2x.Button(PSB_L2)) {
    Serial.println("Presionaste el boton L2...");
    Serial.println(ps2x.Button(PSB_L2), DEC);
  } else if (ps2x.Button(PSB_R2)) {
    Serial.println("Presionaste el boton R2.");
    Serial.println(ps2x.Button(PSB_R2), DEC);
  }

  if (ps2x.Button(PSB_BLUE)) {
    Serial.println("Presionaste el boton x o azul...");
  } else if (ps2x.Button(PSB_RED)) {
    Serial.println("Presionaste el boton criculo o rojo...");

  } else if (ps2x.Button(PSB_TRIANGLE)) {
    Serial.println("Presionaste el boton verde o triangulo...");
  } else if (ps2x.Button(PSB_PINK)) {
    Serial.println("Presionaste el boton cuadrado o rosado...");
  }

  // if (ps2x.Analog(PSS_LX), DEC) {
  //   Serial.println(ps2x.Analog(PSS_LX), HEX);
  //   Serial.println("Moviendo el analogo izq en el eje x");
  // }else if(ps2x.Analog(PSS_LY), DEC){
  //   Serial.println(ps2x.Analog(PSS_LY), HEX);
  //   Serial.println("Moviendo el analogo izq en el eje y");
  // }
  // if (ps2x.Analog(PSS_RX), DEC) {
  //   Serial.println("Moviendo el analogo derech en el eje x");
  // }else if(ps2x.Analog(PSS_RY), DEC){
  //   Serial.println("Moviendo el analogo derech en el eje y");
  // }
}

// Test de Servos
void testServoMotores(){
  for (int i = 0; i < 181; i++) {
    servoBase.write(i);
    servoAntebrazo.write(i);
    servoCodo.write(i);
    servoPinza.write(i);
    delay(25);
  }
  delay(1200);
  for (int i = 180; i > 1; i--) {
    delay(25);
    servoBase.write(i);
    servoAntebrazo.write(i);
    servoCodo.write(i);
    servoPinza.write(i);
  }
}

// Movimiento ServoAnteBrazo
void movimientoServoAntebrazo() {

  /*
    Valor maximo_angulo = 90 -> encogido
    Valor minimo_angulo = 0  -> estirado
    Tecla DOWN = estirar antebrazo -> Alargara
    Tecla UP = levantar antebrazo -> formara un angulo de 90
    Min = 0
    Max = 80 o 90
  */
  if(ps2x.Button(PSB_PAD_DOWN)){
    Serial.println("Bajando brazo");
    if(ang_ServoA >= 0 && ang_ServoA <= 88){
      ang_ServoA += 2;
    }
    Serial.println(ang_ServoA);
  }else if(ps2x.Button(PSB_PAD_UP)){
    Serial.println("Alzando brazo");
    if(ang_ServoA <= 90 && ang_ServoA >= 2){
      ang_ServoA -= 2;
    }
    Serial.println(ang_ServoA);
  }
}

// Movimiento ServoBase
void movimientoServoBase() {

  /*
    Valor maximo_angulo = 180
    Valor minimo_angulo = 0  
    Valor central = 90
    Tecla Left = gira a la izquierda
    Tecla Right = gira a la derecha
    Min = 0
    Max = 8180
  */
  if(ps2x.Button(PSB_PAD_LEFT)){
    Serial.println("Girando Base Izq");
    if(ang_ServoB < 180){
      ang_ServoB += 2;
    }
    Serial.println(ang_ServoB);
  }else if(ps2x.Button(PSB_PAD_RIGHT)){
    Serial.println("Girando Base Der.");
    if(ang_ServoB > 0){
      ang_ServoB -= 2;
    }
    Serial.println(ang_ServoB);
  }
  
}

// Movimiento ServoCodo
void movimientoServoCodo() {

  /*
    Valor maximo_angulo = 90
    Valor minimo_angulo = 180  
    Tecla L1 = dobla codo
    Tecla R1 = estira codo
    Min = 180
    Max = 90
  */
  if(ps2x.Button(PSB_BLUE)){
    Serial.println("Estirando Codo..");
    if(ang_ServoC < 180){
      ang_ServoC += 2;
    }
    Serial.println(ang_ServoC);
  }else if(ps2x.Button(PSB_GREEN)){
    Serial.println("Doblando codo...");
    if(ang_ServoC > 90){
      ang_ServoC -= 2;
    }
    Serial.println(ang_ServoC);
  }
  
}

// Movimiento ServoPinza
void movimientoServoPinza() {

  /*
    Valor maximo_angulo = 75
    Valor minimo_angulo = 0  
    Tecla L2 = abre pinza
    Tecla R2 = cierra pinza
    Min = 0
    Max = 75
  */
  if(ps2x.Button(PSB_PINK)){
    Serial.println("Abriendo Pinza...");
    if(ang_ServoP >= 0 && ang_ServoP < 76){
      ang_ServoP += 2;
    }
    Serial.println(ang_ServoP);
  }else if(ps2x.Button(PSB_RED)){
    Serial.println("Cerrando Pinza..");
    if(ang_ServoP <= 76 && ang_ServoP >= 2){
      ang_ServoP -= 2;
    }
    Serial.println(ang_ServoP);
  }
  
}

// Controlar Carro
void controlarCarro(){
  analog_left_eje_x = ps2x.Analog(PSS_LX), DEC;
  // x almacena valores del analogo en un rango de 0 a 255
  analog_left_eje_x -= 127;
  // x comprendera rangos de -127 a 127
  if (analog_left_eje_x == 128) {
    analog_left_eje_x = 127;
  }
  // Analogo derecho para velocidad y sentido de las ruedas
  // superior 0 - inferior 255, intermedio 128
  analog_right_eje_y = ps2x.Analog(PSS_RY);
  analog_right_eje_y = analog_right_eje_y - 127;

  if (analog_right_eje_y == 128) {
    analog_right_eje_y = 127;
  }

  // Serial.print("....RX_y: ");
  // Serial.print(analog_right_eje_y);
  // Serial.print("....\n");

  // Si mueve el analogo hacia adelante (-127) o hacia atras (127) hara mover los motore
  // 1 es la posicion central del analogo

  if (analog_right_eje_y != 1) {
    //Movera hacia delante o hacia atras cuando es menor o mayor de 1, es decir los numeros negativos o positivos
    //
    if (analog_right_eje_y < 10) {
      // Serial.print("Moviendo hacia delante..");
      // Serial.print("Analogo > 1, valor: ");
      // Serial.println(analog_right_eje_y);
      velocidad = analog_right_eje_y;
      // Serial.println(velocidad);
      // Giro hacia derecha
      if (analog_left_eje_x > 27) {
        //
        analog_left_eje_x = (-1) * analog_left_eje_x + 127;
        aux = (velocidad * analog_left_eje_x) / 100;
        encenderMotor(1, velocidad, aux);
      } else if (analog_left_eje_x < -27) {
        // Giro hacia izquierda
        analog_left_eje_x += 127;
        // aux -> velocidad variable de acuerdo al giro y eje de aceleracion
        aux = (velocidad * analog_left_eje_x) / 100;
        encenderMotor(1, aux, velocidad);
      } else {
        // No habra pulsado ninguno, solo mueve hacia la posicion que esta moviendo el analogo derecho
        encenderMotor(1, velocidad, velocidad);
      }

    } else if (analog_right_eje_y > 10) {
      // Moviendo hacia atras
      velocidad = analog_right_eje_y;
      if(analog_left_eje_x > 27){
        // retrocede hacia izquierda
        analog_left_eje_x = (-1) * analog_left_eje_x + 127;
        aux = (velocidad * analog_left_eje_x) / 100;
        encenderMotor(-1, velocidad, aux);
      }else if(analog_left_eje_x < -27){
        // retrocede hacia derecha
        // reduce motor izquierdo
        analog_left_eje_x += 127;
        // aux -> velocidad variable de acuerdo al giro y eje de aceleracion
        aux = (velocidad * analog_left_eje_x) / 100;
        encenderMotor(-1, aux, velocidad);
      }else{
        // solo retrocede hacia atras
        encenderMotor(-1, velocidad, velocidad);
      }
    }
    // impresionMensaje();

  } else {
    // Dejo de mover el analogo
    velocidad = 0;
    // Serial.println(analog_right_eje_y);
    // encenderMotor(2, velocidad, velocidad);

    // Solo movera el analognizquierd
    if (analog_left_eje_x < -27) {
        //
        velocidad = 127;
        analog_left_eje_x = (-1) * analog_left_eje_x + 127;
        aux = (velocidad * analog_left_eje_x) / 100;
        encenderMotor(1, velocidad, aux);
      } else if (analog_left_eje_x > 27) {
        // Giro hacia izquierda
        velocidad = 127;
        analog_left_eje_x += 127;
        // aux -> velocidad variable de acuerdo al giro y eje de aceleracion
        aux = (velocidad * analog_left_eje_x) / 100;
        encenderMotor(1, aux, velocidad);
      } else {
        // No habra pulsado ninguno, solo mueve hacia la posicion que esta moviendo el analogo derecho
        encenderMotor(2, velocidad, velocidad);
      }
  }

  // delay(50);
}

// Controlar Carro 2 metodo
void controlarCarro_2(){
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

  encenderMotor((m1 != 0 || m2 != 0) ? (m1 > 0 ? 1 : -1) : 0, abs(m1), abs(m2));
}

// Encender Motores ruedas
void encenderMotor(int v, int m1, int m2) {
  // v = 1 -> adelante
  // v = -1 -> atras
  if (v == 1) {
    // Envia senial de salida
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
  // rueda izquierda
  analogWrite(ENA, abs(m1));
  // rueda derecha
  analogWrite(ENB, abs(m2));
}