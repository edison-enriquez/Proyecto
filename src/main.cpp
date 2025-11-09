#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <Wire.h>

// Configuración de pines
#define PIN_SDA 21
#define PIN_SCL 22
#define SERVO_X 27
#define SERVO_Y 26
#define SERVO_Z 25
#define JOY_X 34
#define JOY_Y 35
#define JOY_SW 32

// Configuración pantalla
#define ANCHO 128
#define ALTO 64
#define OLED_RESET -1
Adafruit_SSD1306 display(ANCHO, ALTO, &Wire, OLED_RESET);

// Configuración servos
Servo servoX;
Servo servoY;
Servo servoZ;

// Variables globales
int valX = 90;
int valY = 90;
int valZ = 90;
String movimiento = "";

void setup() {
  Serial.begin(115200);
  
  // Inicializar servos
  servoX.attach(SERVO_X);
  servoY.attach(SERVO_Y);
  servoZ.attach(SERVO_Z);
  
  // Inicializar pantalla
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error en pantalla OLED");
    while(1);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Brazo robot listo!");
  display.display();
  
  pinMode(JOY_SW, INPUT_PULLUP);
  delay(1500);
}

void loop() {
  int xValue = analogRead(JOY_X);
  int yValue = analogRead(JOY_Y);
  int swValue = digitalRead(JOY_SW);
  
  // Mapear valores
  valX = map(xValue, 0, 4095, 0, 180);
  valY = map(yValue, 0, 4095, 0, 180);
  
  // Mover servos
  servoX.write(valX);
  servoY.write(valY);
  
  // Control servo Z
  if(swValue == LOW) {
    servoZ.write(45);
    movimiento = "Dibujando!";
  } else {
    servoZ.write(90);
  }
  
  // Mostrar en pantalla
  display.clearDisplay();
  display.setCursor(0,10);
  display.print("Mov: ");
  display.println(movimiento);
  display.setCursor(0,30);
  display.print("X: ");
  display.print(valX);
  display.print(" Y: ");
  display.println(valY);
  display.display();
  
  delay(100);
}
