#include <Arduino.h>
#include <Servo.h>

// Pin definitions for TCS3200
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define OUT_PIN 6

// Declare Servo objects for each joint
Servo servo1, servo2, servo3, servo4;

// Function to initialize servo positions
void initializeServos() {
  servo1.attach(9);  // Base rotation servo on pin 9
  servo2.attach(10); // Shoulder servo on pin 10
  servo3.attach(11); // Elbow servo on pin 11
  servo4.attach(12); // Claw servo on pin 12
}

// Function to smoothly move a servo to a target angle
void smoothMove(Servo &servo, int startAngle, int targetAngle, int stepDelay) {
  if (startAngle < targetAngle) {
    for (int angle = startAngle; angle <= targetAngle; angle++) {
      servo.write(angle);
      delay(stepDelay); // Delay between each step
    }
  } else {
    for (int angle = startAngle; angle >= targetAngle; angle--) {
      servo.write(angle);
      delay(stepDelay); // Delay between each step
    }
  }
}

// Function to perform a robotic arm movement step with smooth transitions
void performStep(String stepName, int baseAngle, int shoulderAngle, int elbowAngle, int clawAngle, int delayTime) {
  Serial.println(stepName);

  // Smoothly move servos to the specified angles
  smoothMove(servo1, servo1.read(), baseAngle, 10);
  smoothMove(servo2, servo2.read(), shoulderAngle, 10);
  smoothMove(servo3, servo3.read(), elbowAngle, 10);
  smoothMove(servo4, servo4.read(), clawAngle, 5);

  // Wait for movement to complete
  delay(delayTime);
}

// Function to read color frequency
unsigned long readColor(bool s2State, bool s3State) {
  digitalWrite(S2, s2State); // Set S2 state
  digitalWrite(S3, s3State); // Set S3 state
  return pulseIn(OUT_PIN, LOW); // Measure frequency from OUT pin
}

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging

  // Set S0 and S1 for frequency scaling (20%)
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  digitalWrite(S0, HIGH);  // Frequency scaling
  digitalWrite(S1, LOW);

  // Set S2 and S3 for photodiode selection
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Set OUT pin as input
  pinMode(OUT_PIN, INPUT);

  initializeServos(); // Set up initial servo positions
  Serial.println("TCS3200 Color Detection and Robotic Arm Initialized");
}

void loop() {
  // Read raw frequency values for red, green, and blue
  unsigned long red = readColor(LOW, LOW);    // Red filter
  unsigned long green = readColor(HIGH, HIGH); // Green filter
  unsigned long blue = readColor(LOW, HIGH);  // Blue filter

  // Print the raw RGB values
  Serial.print("Red: ");
  Serial.print(red);
  Serial.print(", Green: ");
  Serial.print(green);
  Serial.print(", Blue: ");
  Serial.println(blue);

  // Determine the dominant color
  if (green < red && green < blue) {
    Serial.println("Detected Color: Green");
    // Perform actions for green detection
    performStep("Step 1.1: Elbow up", 90, 10, 90, 175, 500);
    performStep("Step 1.2: Move to Pickup Position", 180, 10, 90, 175, 500);
    performStep("Step 1.3: Claw open", 180, 10, 90, 140, 500);
    performStep("Step 1.4: Elbow down", 180, 10, 160, 140, 500);
    performStep("Step 1.5: Close Claw", 180, 10, 160, 170, 500);
    performStep("Step 2: Lift Object", 180, 10, 90, 170, 500);
    performStep("Step 3: Move to Place Position", 30, 45, 105, 170, 500);
    performStep("Step 4: Open Claw", 30, 45, 105, 140, 500);
    performStep("Step 5.1: Elbow up", 30, 10, 90, 140, 500);
    performStep("Step 5.2: Claw close", 30, 10, 90, 175, 500);
    performStep("Step 5.3: Axis rotation", 90, 10, 90, 175, 500);
    performStep("Step 5.4: Elbow down", 90, 10, 180, 175, 500);
    delay(5000); // Wait before the next reading
  } else if (blue < red && blue < green) {
    Serial.println("Detected Color: Blue");
    // Perform actions for blue detection
    performStep("Step 1.1: Elbow up", 90, 10, 90, 175, 500);
    performStep("Step 1.2: Move to Pickup Position", 180, 10, 90, 175, 500);
    performStep("Step 1.3: Claw open", 180, 10, 90, 140, 500);
    performStep("Step 1.4: Elbow down", 180, 10, 160, 140, 500);
    performStep("Step 1.5: Close Claw", 180, 10, 160, 170, 500);
    performStep("Step 2: Lift Object", 180, 10, 90, 170, 500);
    performStep("Step 3: Move to Place Position", 75, 45, 105, 170, 500);
    performStep("Step 4: Release object", 75, 45, 105, 140, 500);
    performStep("Step 5.1: Elbow up", 75, 10, 90, 140, 500);
    performStep("Step 5.2: Claw close", 75, 10, 90, 175, 500);
    performStep("Step 5.3: Axis rotation", 90, 10, 90, 175, 500);
    performStep("Step 5.4: Elbow down", 90, 10, 180, 175, 500);
    delay(5000); // Wait before the next reading
  } else {
    Serial.println("Detected Color: Unknown (Mixed/Neutral)");
  }

  delay(500); // Wait before the next reading
}