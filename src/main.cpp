#include <Arduino.h>

// Pines del Motor 1
#define ENABLE1 5
#define IN11 25
#define IN12 26

// Pines del Motor 2 (Motor izquierdo mirando sensor de frente)
#define ENABLE2 18
#define IN21 9
#define IN22 10

// Pines sensores de línea
#define LINE_SENSOR_LEFT 19
#define LINE_SENSOR_CENTER 18
#define LINE_SENSOR_RIGHT 23
#define LINE_SENSOR_STOP 5

#define ON 0      //luz encendida, sensor activo porque captura el rebote
#define OFF 1

// PWM
long BASE_SPEED = 70;
long RIGHT_SPEED = 70;
long LEFT_SPEED = 70;
long SPEED_MODIFIER = 1;
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

// Canales PWM
#define PWM_CH_IN11 0
#define PWM_CH_IN12 1
#define PWM_CH_IN21 2
#define PWM_CH_IN22 3

// Debug timing
unsigned long lastDebugTime = 0;
const unsigned long DEBUG_INTERVAL = 500;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando robot seguidor de línea...");

  // Configurar sensores
  pinMode(LINE_SENSOR_LEFT, INPUT);
  pinMode(LINE_SENSOR_CENTER, INPUT);
  pinMode(LINE_SENSOR_RIGHT, INPUT);

  // Habilitar motores
  pinMode(ENABLE1, OUTPUT);
  pinMode(ENABLE2, OUTPUT);
  digitalWrite(ENABLE1, HIGH);
  digitalWrite(ENABLE2, HIGH);

  // Configurar PWM para cada pin de control del motor
  ledcSetup(PWM_CH_IN11, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN11, PWM_CH_IN11);

  ledcSetup(PWM_CH_IN12, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN12, PWM_CH_IN12);

  ledcSetup(PWM_CH_IN21, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN21, PWM_CH_IN21);

  ledcSetup(PWM_CH_IN22, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(IN22, PWM_CH_IN22);

  Serial.println("Configuración completada");
}

void lineFollowPWM() {
  int sensorIzq = digitalRead(LINE_SENSOR_LEFT);
  int sensorCentro = digitalRead(LINE_SENSOR_CENTER);
  int sensorDer = digitalRead(LINE_SENSOR_RIGHT);
  int sensorStop = digitalRead(LINE_SENSOR_STOP);

  // Debug sensores
  if (millis() - lastDebugTime >= DEBUG_INTERVAL) {
    Serial.print("Sensores -> Izq: ");
    Serial.print(sensorIzq == ON ? "ENCENCIDO" : "APAGADO");
    Serial.print(" | Centro: ");
    Serial.print(sensorCentro == ON ? "ENCENCIDO" : "APAGADO");
    Serial.print(" | Der: ");
    Serial.print(sensorDer == ON ? "ENCENCIDO" : "APAGADO");
    Serial.print("  --> Acción: ");

    if (sensorIzq == ON && sensorCentro == ON && sensorDer == ON) {
      Serial.println("DETENERSE - TODOS LOS SENSORES ACTIVOS");
    } else if (sensorDer == OFF && sensorIzq == OFF) {
      Serial.println("SEGUIR RECTO");
    } else if (sensorIzq == OFF && sensorDer == OFF) {
      Serial.println("Girar DERECHA");
    } else if (sensorIzq == ON && sensorDer == OFF) {
      Serial.println("GIRAR IZQUIERDA");
    } else {
      Serial.println("DETENERSE");
    }

    lastDebugTime = millis();
  }

  // Verificar si todos los sensores están activos
  if (sensorIzq == ON && sensorDer == ON && sensorStop == ON) {
    // Detener el robot
    ledcWrite(PWM_CH_IN11, 0);
    ledcWrite(PWM_CH_IN12, 0);
    ledcWrite(PWM_CH_IN21, 0);
    ledcWrite(PWM_CH_IN22, 0);
    delay(5000); // Esperar 5 segundos
    
    // Avanzar un poco y luego continuar con el seguimiento normal
    ledcWrite(PWM_CH_IN11, BASE_SPEED/2);
    ledcWrite(PWM_CH_IN12, 0);
    ledcWrite(PWM_CH_IN21, BASE_SPEED/2);
    ledcWrite(PWM_CH_IN22, 0);
    delay(1000); // Avanzar por 1 segundo
    return; // Salir de la función para que en la siguiente iteración vuelva al seguimiento normal
  }

  // Movimiento según sensores
  if (sensorIzq == OFF && sensorDer == OFF) {
    // Seguir recto
     ledcWrite(PWM_CH_IN11, BASE_SPEED);
     ledcWrite(PWM_CH_IN12, 0);
     ledcWrite(PWM_CH_IN21, BASE_SPEED);
     ledcWrite(PWM_CH_IN22, 0);
   }
   else if (sensorDer == OFF && sensorIzq == ON) {
    // Girar a la izquierda
    ledcWrite(PWM_CH_IN11, BASE_SPEED);     //Pin de giro a la derecha
    ledcWrite(PWM_CH_IN12, 0);
    ledcWrite(PWM_CH_IN21, BASE_SPEED/5);         //Pin de giro a la izquierda
    ledcWrite(PWM_CH_IN22, 0);
   } else if (sensorIzq == OFF && sensorDer == ON) {
    // Girar a la derecha
    ledcWrite(PWM_CH_IN11, BASE_SPEED/5);         //Pin de giro a la derecha
    ledcWrite(PWM_CH_IN12, 0);
    ledcWrite(PWM_CH_IN21, BASE_SPEED);     //Pin de giro a la izquierda
    ledcWrite(PWM_CH_IN22, 0);
  }
}

void loop() {
  delay(50); // Cambiado a 50ms para evitar congelamiento
  lineFollowPWM();
}