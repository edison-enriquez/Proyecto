#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// CONFIGURACIÓN DE LA PANTALLA
#define ANCHO 128
#define ALTO 64
#define OLED_RESET -1
Adafruit_SSD1306 display(ANCHO, ALTO, &Wire, OLED_RESET);

// CONFIGURACIÓN DE LOS SERVOS
Servo servoX;
Servo servoY;
Servo servoZ;

#define SERVO_X 27
#define SERVO_Y 26
#define SERVO_Z 25

// CONFIGURACIÓN DEL JOYSTICK

#define JOY_X 34
#define JOY_Y 35
#define JOY_SW 32

// VARIABLES GLOBALES

int valX = 90;
int valY = 90;
int valZ = 90;
String movimiento = "";


// CONFIGURACIÓN INICIAL

void setup() {
  Serial.begin(115200);

  // Inicio de los  servos
  servoX.attach(SERVO_X);
  servoY.attach(SERVO_Y);
  servoZ.attach(SERVO_Z);

  // Inicializar pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No se detecta pantalla OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Brazo robot listo!");
  display.display();

  pinMode(JOY_SW, INPUT_PULLUP);
  delay(1500);
}

// BUCLE PRINCIPAL

void loop() {
  int xValue = analogRead(JOY_X);
  int yValue = analogRead(JOY_Y);
  int swValue = digitalRead(JOY_SW);

  // Busqueda valores de joystick a ángulos de servos
  valX = map(xValue, 0, 4095, 0, 180);
  valY = map(yValue, 0, 4095, 0, 180);

  // Mover servos segun el joystick
  servoX.write(valX);
  servoY.write(valY);

  // Servo Z (presionar el botón para "dibujar")
  if (swValue == LOW) {
    servoZ.write(45);  // Baja el lapiz
    movimiento = "Dibujando!";
  } else {
    servoZ.write(90);  // Sube el lapiz
  }

  // Detectar dirección
  if (xValue > 3000) movimiento = "Derecha";
  else if (xValue < 1000) movimiento = "Izquierda";
  else if (yValue > 3000) movimiento = "Arriba";
  else if (yValue < 1000) movimiento = "Abajo";
  else if (swValue == LOW) movimiento = "Centro (boton)";
  else movimiento = "Quieto";

  // Mostrar en pantalla
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Mov: ");
  display.println(movimiento);
  display.setCursor(0, 30);
  display.print("X: "); display.print(valX);
  display.print(" | Y: "); display.println(valY);
  display.display();

  delay(100);
}
