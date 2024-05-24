#include <AccelStepper.h>

// Initialize the stepper library
AccelStepper myStepperA(AccelStepper::DRIVER, 3, 2); // step, direction
AccelStepper myStepperB(AccelStepper::DRIVER, 5, 4);
AccelStepper stepperRightLower(AccelStepper::DRIVER, 7, 6);
AccelStepper stepperClaw(AccelStepper::DRIVER, 9, 8);
AccelStepper stepperRightUpper(AccelStepper::DRIVER, 11, 10);

// Stepper properties
const float DRIVER_MICROSTEPPING = 0.5; // E.g., 1/2 = 0.5
const int STEPS_PER_REVOLUTION = 200 / DRIVER_MICROSTEPPING;

const int MAX_REVS_PER_SECOND = 3;
const int MAX_STEPS_PER_SECOND = STEPS_PER_REVOLUTION * MAX_REVS_PER_SECOND;

// Slider potentiometers
const int MIN_SLIDER = 710;
const int MAX_SLIDER = 1023;
const int MID_SLIDER = 830;
const int STOP_ZONE = 70;

void setup() {
  for (int i = 0; i < 5; i++) {
      initStepper(i);
  }
  Serial.begin(9600);
}

void loop() {
  // Read analog values and calculate motions

  for (int i = 0; i < 5; i++) {
    // Pins A1-A5 are used.
    int sliderValue = analogRead(A1 + i);
    float velocity = sliderToVelocity(sliderValue);
    setStepperVelocity(i, velocity);
    runStepper(i);
  }
}

// Function to convert analog read of slider voltage divider to velocity of motor
float sliderToVelocity(int sliderValue) {
  // Constrain sliderValue within the MIN_SLIDER and MAX_SLIDER bounds
  sliderValue = constrain(sliderValue, MIN_SLIDER, MAX_SLIDER);

  if (sliderValue > MID_SLIDER + STOP_ZONE) {
    // Mapping from just above the stop zone to the max slider value, from 0 to 3.0
    return map(sliderValue, MID_SLIDER + STOP_ZONE + 1, MAX_SLIDER, 0, MAX_REVS_PER_SECOND * 100) / 100.0;
  } else if (sliderValue < MID_SLIDER - STOP_ZONE) {
    // Mapping from the min slider value to just below the stop zone, from -3.0 to 0
    return map(sliderValue, MIN_SLIDER, MID_SLIDER - STOP_ZONE - 1, -MAX_REVS_PER_SECOND * 100, 0) / 100.0;
  } else {
    return 0; // Stop zone
  }
}

// Function to return a pointer to the corresponding stepper based on the index
AccelStepper* stepperFromIndex(int index) {
  switch (index) {
    case 0: return &myStepperA;
    case 1: return &stepperRightLower;
    case 2: return &myStepperB;
    case 3: return &stepperRightUpper;
    case 4: return &stepperClaw;
    default: return nullptr; // Return null if the index is out of bounds
  }
}

// Set up steppers
void initStepper(int index) {
  AccelStepper* stepper = stepperFromIndex(index);
  if (!stepper) return;
  // Set the maximum speed in steps per second:
  stepper->setMaxSpeed(MAX_STEPS_PER_SECOND);
}

// Move a stepper at some velocity
void setStepperVelocity(int stepperIndex, float motorRevsPerSec) {
  AccelStepper* stepper = stepperFromIndex(stepperIndex);
  if (!stepper) return;
  float stepsPerSecond = motorRevsPerSec * STEPS_PER_REVOLUTION;
  stepper->setSpeed(stepsPerSecond);
}

// Move a stepper toward
void runStepper(int index) {
  AccelStepper* stepper = stepperFromIndex(index);
  if (!stepper) return;
  stepper->runSpeed();
}
