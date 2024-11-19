#include <Servo.h>

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

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  initializeServos(); // Set up initial servo positions
}

void loop() {
  // Initial position
  servo1.write(90);  
  servo2.write(10);   
  servo3.write(180); 
  servo4.write(175); 

  // Step 1: Move to pickup position
  performStep("Step 1.1: Elbow up", 90, 10, 90, 175, 500);
  performStep("Step 1.2: Move to Pickup Position", 180, 10, 90, 175, 500);
  performStep("Step 1.3: Claw open", 180, 10, 90, 140, 500);
  performStep("Step 1.4: Elbow down", 180, 10, 160, 140, 500);
  performStep("Step 1.5: Close Claw", 180, 10, 160, 165, 500);

  // Step 2: Lift object
  performStep("Step 2: Lift Object", 180, 10, 90, 165, 500);

  // Step 3: Move to place position
  performStep("Step 3: Move to Place Position", 45, 45, 105, 165, 500);

  // Step 4: Release object
  performStep("Step 4: Open Claw", 45, 45, 105, 140, 500);

  // Step 5: Return to initial position
  performStep("Step 5.1: Elbow up", 45, 10, 90, 140, 500);
  performStep("Step 5.2: Claw close", 45, 10, 90, 175, 500);
  performStep("Step 5.3: Axis rotation", 90, 10, 90, 175, 500);
  performStep("Step 5.4: Elbow down", 90, 10, 180, 175, 500);

  // Delay before restarting the loop
  delay(10000);
}
