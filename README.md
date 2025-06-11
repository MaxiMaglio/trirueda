# Robot Seguidor de Línea

Este proyecto implementa un robot seguidor de línea utilizando un ESP32 y sensores infrarrojos. El robot sigue una línea negra sobre una superficie clara, ajustando su movimiento según la posición de la línea.

## Componentes Principales

### Hardware
- ESP32 como microcontrolador
- 2 motores DC (izquierdo y derecho)
- 3 sensores infrarrojos para detección de línea
- Driver de motores (L298N o similar)

### Conexiones
- **Motor Derecho**:
  - ENABLE1: Pin 5
  - IN11: Pin 25
  - IN12: Pin 26

- **Motor Izquierdo**:
  - ENABLE2: Pin 18
  - IN21: Pin 9
  - IN22: Pin 10

- **Sensores de Línea**:
  - Sensor Izquierdo: Pin 19
  - Sensor Central: Pin 18
  - Sensor Derecho: Pin 23
  - Sensor de Parada: Pin 5

## Funcionamiento

### Configuración Inicial
1. El código configura los pines de los motores y sensores
2. Inicializa la comunicación serial para depuración
3. Configura los canales PWM para el control de velocidad de los motores

### Lógica de Seguimiento
El robot utiliza tres sensores para seguir la línea:
- Si el sensor central detecta la línea (ON), el robot avanza recto
- Si el sensor izquierdo detecta la línea, el robot gira a la izquierda
- Si el sensor derecho detecta la línea, el robot gira a la derecha
- Si todos los sensores están activos, el robot se detiene por 5 segundos y luego avanza un poco

### Control de Velocidad
- Velocidad base: 70 (en escala PWM de 0-255)
- Los motores se controlan mediante PWM para ajuste de velocidad
- Al girar, un motor reduce su velocidad a 1/5 de la velocidad base

## Depuración
El código incluye mensajes de depuración que se muestran cada 500ms en el monitor serial, mostrando:
- Estado de cada sensor (ENCENDIDO/APAGADO)
- Acción que está realizando el robot

## Requisitos
- Arduino IDE
- Biblioteca Arduino.h
- ESP32 board support package

## Uso
1. Cargar el código en el ESP32
2. Conectar los componentes según las conexiones especificadas
3. Colocar el robot sobre una línea negra
4. El robot comenzará a seguir la línea automáticamente

## Notas
- El robot se detiene por 5 segundos cuando detecta una intersección (los 3 sensores activos -> sensor derecho, izquierdo y el stop)
- La velocidad base puede ajustarse modificando la variable `BASE_SPEED`
- El tiempo de actualización del loop principal es de 50ms para evitar congelamientos 