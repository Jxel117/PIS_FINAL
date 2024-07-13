#include <PS2X_lib.h>

// Creamos una instancia de la clase PS2X
PS2X ps2x;

// Pines de conexión del control de PlayStation al Arduino
#define PS2_DAT  12  // Pin de datos
#define PS2_CMD  11  // Pin de comando
#define PS2_SEL  10  // Pin de selección
#define PS2_CLK  13  // Pin de reloj

// Para realizar la configuración
void setup() {
  Serial.begin(9600);

  // Inicializamos la conexión con el control de PlayStation
  int error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  
  if (error == 0) {
    Serial.println("Control de PlayStation conectado.");
  } else {
    Serial.println("Error al conectar el control de PlayStation.");
    return;
  }
}

// Para verificar el estado de los botones
void loop() {
  // Leemos el estado de los botones
  ps2x.read_gamepad(false, 0);

  // Verificamos cada botón y mostramos su estado en el monitor serial
  if (ps2x.Button(PSB_START)) {
    Serial.println("START presionado");
  }
  if (ps2x.Button(PSB_SELECT)) {
    Serial.println("SELECT presionado");
  }
  if (ps2x.Button(PSB_PAD_UP)) {
    Serial.println("PAD UP presionado");
  }
  if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.println("PAD DOWN presionado");
  }
  if (ps2x.Button(PSB_PAD_LEFT)) {
    Serial.println("PAD LEFT presionado");
  }
  if (ps2x.Button(PSB_PAD_RIGHT)) {
    Serial.println("PAD RIGHT presionado");
  }
  if (ps2x.Button(PSB_L1)) {
    Serial.println("L1 presionado");
  }
  if (ps2x.Button(PSB_L2)) {
    Serial.println("L2 presionado");
  }
  if (ps2x.Button(PSB_L3)) {
    Serial.println("L3 presionado");
  }
  if (ps2x.Button(PSB_R1)) {
    Serial.println("R1 presionado");
  }
  if (ps2x.Button(PSB_R2)) {
    Serial.println("R2 presionado");
  }
  if (ps2x.Button(PSB_R3)) {
    Serial.println("R3 presionado");
  }
  if (ps2x.Button(PSB_GREEN)) {
    Serial.println("TRIANGLE presionado");
  }
  if (ps2x.Button(PSB_RED)) {
    Serial.println("CIRCLE presionado");
  }
  if (ps2x.Button(PSB_BLUE)) {
    Serial.println("X presionado");
  }
  if (ps2x.Button(PSB_PINK)) {
    Serial.println("SQUARE presionado");
  }

  // Esperamos un poco antes de la siguiente lectura
  delay(100);
}
