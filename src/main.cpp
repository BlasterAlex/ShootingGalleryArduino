#include <Arduino.h>
#include <CustomStepper.h>

#define DEBOUNCE 200

const int potPin = A0; // потенциометр
const int button = 7;  // кнопка

// Степпер
byte tSteps[] = {8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001};
CustomStepper stepper(8, 9, 10, 11, (byte *)&tSteps, 4075.7728395, 16, STOP);

// Состояние игры
bool isPlaying = false;

void setup() {
  pinMode(button, INPUT);
  Serial.begin(9600);
}

void loop() {

  // Считывание показателей с потенциометра
  int sensorReading = analogRead(potPin);
  int stepperSpeed = map(sensorReading, 0, 1023, -25, 25);

  static bool preBtnState;
  static int lastTime;
  int now = millis();

  // Текущее состояние кнопки
  const int stateButton = digitalRead(button);

  // Нажатие кнопки
  if (stateButton == HIGH && preBtnState == LOW && now - lastTime >= DEBOUNCE) {
    isPlaying = !isPlaying;
    lastTime = now;
  }
  preBtnState = stateButton;

  // Движение мотора
  stepper.setRPM((stepperSpeed > 0) ? stepperSpeed : -stepperSpeed);

  if (!isPlaying || stepperSpeed == 0)
    stepper.setDirection(STOP);
  else {
    if (stepperSpeed > 0)
      stepper.setDirection(CW);
    else
      stepper.setDirection(CCW);
  }

  if (stepper.isDone())
    stepper.rotate();
  else
    stepper.run();
}
