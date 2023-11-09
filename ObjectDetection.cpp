#include <AFMotor.h>
#include <Servo.h>
#define MotorSpeed 180
#define TriggerPin A0
#define EchoPin A1
#define IRPin A2  // Pin connected to the IR sensor
#define ServoCenter 90
int distance;
int LeftDistance;
int RightDistance;
int Left = 0;
int Right = 0;
Servo servo;
AF_DCMotor Motor1(1);
AF_DCMotor Motor2(2);
AF_DCMotor Motor3(3);
AF_DCMotor Motor4(4);

void setup() {
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(IRPin, INPUT);  // IR sensor as input
  servo.attach(10);
  initialize();
  Motor1.setSpeed(MotorSpeed);
  Motor2.setSpeed(MotorSpeed);
  Motor3.setSpeed(MotorSpeed);
  Motor4.setSpeed(MotorSpeed);
}

void loop() {
  distance = measureDistance();
  bool irDetected = checkIRSensor();  // Check IR sensor for objects emitting heat
  if (distance <= 12 || irDetected) {
    stopMotors();
    moveBackward();
    delay(100);
    stopMotors();
    Left = checkLeftDistance();
    servo.write(ServoCenter);
    delay(800);
    Right = checkRightDistance();
    servo.write(ServoCenter);
    if (Left < Right) {
      turnLeft();
      delay(500);
      stopMotors();
      delay(200);
    } else if (Left > Right) {
      turnRight();
      delay(500);
      stopMotors();
      delay(200);
    }
  } else {
    moveForward();
  }
}

void moveForward() {
  Motor1.run(FORWARD);
  Motor2.run(FORWARD);
  Motor3.run(FORWARD);
  Motor4.run(FORWARD);
}

void moveBackward() {
  Motor1.run(BACKWARD);
  Motor2.run(BACKWARD);
  Motor3.run(BACKWARD);
  Motor4.run(BACKWARD);
}

void turnLeft() {
  Motor1.run(BACKWARD);
  Motor2.run(BACKWARD);
  Motor3.run(FORWARD);
  Motor4.run(FORWARD);
}

void turnRight() {
  Motor1.run(FORWARD);
  Motor2.run(FORWARD);
  Motor3.run(BACKWARD);
  Motor4.run(BACKWARD);
}

void stopMotors() {
  Motor1.run(RELEASE);
  Motor2.run(RELEASE);
  Motor3.run(RELEASE);
  Motor4.run(RELEASE);
}

int checkLeftDistance() {
  servo.write(20);
  delay(800);
  LeftDistance = measureDistance();
  return LeftDistance;
}

int checkRightDistance() {
  servo.write(150);
  delay(800);
  RightDistance = measureDistance();
  return RightDistance;
}

int measureDistance() {
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(4);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  long pulseDuration = pulseIn(EchoPin, HIGH);
  long cm = pulseDuration / 29 / 2; // Time to distance conversion
  return cm;
}

bool checkIRSensor() {
  // Read the IR sensor value
  int irValue = analogRead(IRPin);
  
  // Convert the sensor value to temperature (you may need to calibrate this)
  float temperature = (irValue * 0.48875) - 50.0;  // Calibration values may vary
  
  // Check if temperature is above 20 degrees Celsius
  if (temperature > 20.0) {
    return true;  // IR sensor detected an object emitting heat
  } else {
    return false;
  }
}

void initialize() {
  delay(3000);
  for (int a = 0; a < 4; a++) {
    servo.write(ServoCenter);
    delay(50);
    servo.write(40);
    delay(50);
    servo.write(90);
    delay(50);
    servo.write(ServoCenter);
  }
}
