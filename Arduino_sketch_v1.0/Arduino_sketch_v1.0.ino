
#include <NewPing.h> 


#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

#define ENA 5
#define ENB 6


// Front Sensor
#define TRIG_FRONT 2
#define ECHO_FRONT 3
// Back Sensor
#define TRIG_BACK 4
#define ECHO_BACK 5
// Left Sensor
#define TRIG_LEFT 6
#define ECHO_LEFT 7
// Right Sensor
#define TRIG_RIGHT 12
#define ECHO_RIGHT 13


#define TOUCH_FRONT A0
#define TOUCH_BACK A1
#define TOUCH_LEFT A2
#define TOUCH_RIGHT A3


#define MAX_DISTANCE 200 
#define OBSTACLE_THRESHOLD 50 
#define TURN_DURATION 1000 


NewPing sonarFront(TRIG_FRONT, ECHO_FRONT, MAX_DISTANCE);
NewPing sonarBack(TRIG_BACK, ECHO_BACK, MAX_DISTANCE);
NewPing sonarLeft(TRIG_LEFT, ECHO_LEFT, MAX_DISTANCE);
NewPing sonarRight(TRIG_RIGHT, ECHO_RIGHT, MAX_DISTANCE);


byte incomingCommand = 0;
bool movingForward = false;
bool movingBackward = false;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  
  pinMode(TOUCH_FRONT, INPUT);
  pinMode(TOUCH_BACK, INPUT);
  pinMode(TOUCH_LEFT, INPUT);
  pinMode(TOUCH_RIGHT, INPUT);
  
  Serial.begin(115200); 
  

  stopMotors();
}

void loop() {

  handleTouchControl();
  

  if (Serial.available() >= 1) {
    incomingCommand = Serial.read();
    executeCommand(incomingCommand);
  }
  
  if (movingForward) {
    handleForwardObstacle();
  } else if (movingBackward) {
    handleBackwardObstacle();
  }
  
  delay(50);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  movingForward = true;
  movingBackward = false;
  Serial.println("Moving Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  movingBackward = true;
  movingForward = false;
  Serial.println("Moving Backward");
}

void turnLeft() {
  stopMotors();
  delay(50);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(TURN_DURATION);
  stopMotors();
  Serial.println("Turning Left");
}

void turnRight() {
  stopMotors();
  delay(50);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(TURN_DURATION);
  stopMotors();
  Serial.println("Turning Right");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  movingForward = false;
  movingBackward = false;
  Serial.println("Motors Stopped");
}


void executeCommand(byte cmd) {
  switch(cmd) {
    case 0x01: // Move Forward
      moveForward();
      break;
    case 0x02: // Move Backward
      moveBackward();
      break;
    case 0x03: // Turn Left
      turnLeft();
      break;
    case 0x04: // Turn Right
      turnRight();
      break;
    case 0x05: // Stop
      stopMotors();
      break;
    default:
      // Ignore other commands (meant for ESP32)
      break;
  }
}

// --- Obstacle Handling ---
void handleForwardObstacle() {
  int distance = sonarFront.ping_cm();
  if (distance > 0 && distance < OBSTACLE_THRESHOLD) {
    stopMotors();
    Serial.println("Obstacle detected in front!");
    // Check left and right to find a free path
    int leftDist = sonarLeft.ping_cm();
    int rightDist = sonarRight.ping_cm();
    
    if (leftDist > rightDist && leftDist > OBSTACLE_THRESHOLD) {
      turnLeft();
      moveForward(); 
    } 
    else if (rightDist > leftDist && rightDist > OBSTACLE_THRESHOLD) {
      turnRight();
      moveForward();
    }
    else {
      stopMotors();
    }
  }
}

void handleBackwardObstacle() {
  int distance = sonarBack.ping_cm();
  if (distance > 0 && distance < OBSTACLE_THRESHOLD) {
    stopMotors();
    Serial.println("Obstacle detected behind!");
  }
}

// --- Backup Touch Control ---
void handleTouchControl() {
  if (digitalRead(TOUCH_FRONT) == HIGH) {
    moveForward();
    delay(500); 
  }
  else if (digitalRead(TOUCH_BACK) == HIGH) {
    moveBackward();
    delay(500);
  }
  else if (digitalRead(TOUCH_LEFT) == HIGH) {
    turnLeft();
    delay(500);
  }
  else if (digitalRead(TOUCH_RIGHT) == HIGH) {
    turnRight();
    delay(500);
  }
}