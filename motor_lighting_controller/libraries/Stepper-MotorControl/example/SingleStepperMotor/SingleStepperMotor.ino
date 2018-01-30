#include <Stepper-MotorControl.h>

MotorControl stepper(7, 6, 5); // step, direction, enable pin

void setup() {
  stepper.enable();
}


void loop() {

  for (int i = 0; i < 200; i++) { // 200 steps
    stepper.doStep();
  }

  delay(1000);
}
