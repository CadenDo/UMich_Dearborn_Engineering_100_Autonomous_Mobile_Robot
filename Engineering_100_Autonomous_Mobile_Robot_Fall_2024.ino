/*  
To anyone in Engineering 100 at the University of Michigan-Dearborn that finds this and this project is still relevant, don't copy this code, use it as a reference!!!
Copying this code will not make you a better programmer.
You won't gain the skills of debugging or just creating the program.
You'll also probably say that this is a skill (programming) you have now, when it reality you don't.
You can probably get away with this at low level classes, but in the higher level you're setting yourself up for failure because they expect you know this stuff.
Take this one piece of advice, when find things online, learn how they work, and why did this user do it this way. 
*/

// IR Sensors Pins
const int RightIRSensor = 9;
const int LeftIRSensor = 10;

//Right Motor Pins
const int ENA=6;
const int rightMotorPin1=11;
const int rightMotorPin2=8;

//Left Motor Pins
const int ENB=5;
const int leftMotorPin1=7;
const int leftMotorPin2=4;

// Ultra Sonic Sensors Pins
const int TRIG_PIN = 3;
const int ECHO_PIN = 2;

// Misc Global Settings
const int MovementSpeed = 175;
const int OBSTACLE_DISTANCE = 27;

void setup() // setting up pin modes & Inital states
{
  TCCR0B = TCCR0B & B11111000 | B00000010; //Sets PWM pin 5 & 6 to 7812.5 Hz
  
  pinMode(ENA, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  pinMode(ENB, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(RightIRSensor, INPUT);
  pinMode(LeftIRSensor, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  TurnLogic(0,0);   
}

// ALL Motor Modes
void moveForward() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
}

void moveBackward() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
}

void stopMotor() { 
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turnLeft() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
}

void turnRight() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
}

// Main Loop
void loop()
{
  if (isObstacleDetected())
  {
    avoidObstacle(MovementSpeed);
    delay(1000);
    moveForward(); 
  }

  int rightIRSensorValue = digitalRead(RightIRSensor);
  int leftIRSensorValue = digitalRead(LeftIRSensor);

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
  {
    TurnLogic(MovementSpeed, MovementSpeed);
  }
  //If right sensor detects black line, then turn right
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  {
      TurnLogic(-MovementSpeed, MovementSpeed); 
  }
  //If left sensor detects black line, then turn left  
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  {
      TurnLogic(MovementSpeed, -MovementSpeed); 
  } 
  //If both the sensors detect black line, then stop 
  else 
  {
      TurnLogic(-MovementSpeed, MovementSpeed); 
  }
}

// Motor Adjustment Function (Automatic Line Follower)
void TurnLogic(int rightMotorSpeed, int leftMotorSpeed)
{
  
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else 
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }
  analogWrite(ENA, abs(rightMotorSpeed));
  analogWrite(ENB, abs(leftMotorSpeed));    
}

// Object Detection Function Using UltraSonic Sensor
bool isObstacleDetected()
{
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  // Centimeter Calculations
  distance = (duration / 2.0) * 0.0343;

  return (distance > 0 && distance < OBSTACLE_DISTANCE);
}

// Function to avoid an obstacle
void avoidObstacle(int MovementSpeed)
{
    stopMotor();
    delay(1000); 

    turnRight();
    analogWrite(ENA, MovementSpeed+30);
    analogWrite(ENB, MovementSpeed+30);
    delay(3000); // Adjust as needed
    stopMotor();
    delay(3000);

    moveForward();
    analogWrite(ENA, 225);
    analogWrite(ENB, 225);
    delay(4820);    
    analogWrite(ENA, MovementSpeed);
    analogWrite(ENB, MovementSpeed);
    delay(1000); // Adjust as needed
    stopMotor();
    delay(1000); // Make sure everything is right 

    turnLeft();
    analogWrite(ENA, MovementSpeed+20);
    analogWrite(ENB, MovementSpeed+20);
    delay(335   0); // Adjust as needed
    stopMotor();
    delay(1000); // Make sure everything is right 

    moveForward();
    analogWrite(ENA, MovementSpeed);
    analogWrite(ENB, MovementSpeed);
    delay(4000); // adjust as needed
    stopMotor();
    delay(1000); // Make sure everything is right 

    moveForward();
      int leftSensor = digitalRead(LeftIRSensor);
      int rightSensor = digitalRead(RightIRSensor);

    while((leftSensor != 1) && (rightSensor != 1 )){ // moving slowly towards the line to prevent overshooting, small delay added to due to sensor reading really fast
      analogWrite(ENA, MovementSpeed);
      analogWrite(ENB, MovementSpeed);
      delay(100);
      leftSensor = digitalRead(LeftIRSensor);
      rightSensor = digitalRead(RightIRSensor);
    }
    moveForward();
    delay(1000);

    stopMotor();
    delay(1000); // Make sure everything is right

    // turnRight();
    // analogWrite(ENA, MovementSpeed);
    // analogWrite(ENB, MovementSpeed-30);
    // delay(1000);
    // stopMotor();
    // delay(1000);  // Make sure everything is right
}