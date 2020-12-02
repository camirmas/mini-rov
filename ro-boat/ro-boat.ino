//This code controls two motors
//motor A should be connected between A01 and A02
//motor B should be connected between B01 and B02
 

//The below code defines the output pins on the Arduino will hookup to specified pins on the HBridge
const int STBY = 10; //this will be the standby pin

//Motor A
const int PWMA = 3; //Speed control
const int AIN1 = 9; //Direction
const int AIN2 = 8; //Direction

//Motor B
const int PWMB = 5; //Speed control
const int BIN1 = 11; //Direction
const int BIN2 = 12; //Direction

const int MOTOR_L = 0;
const int MOTOR_R = 1; 

const int BUFFER_SIZE = 4;
char command[BUFFER_SIZE];

void setup() {
  Serial.begin(9600);
  
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

 
void loop() {
  if (Serial.available()) {
    if (Serial.readBytes(command, BUFFER_SIZE) > 1) {
      handleCommand();
      memset(command, 0, BUFFER_SIZE);
    }
  }
}


/*
 * Handles a Serial input to control ROV movement.
 */
void handleCommand() {
  int value;
  
  switch(command[0]) {
    case 'f':
      value = parseCommand(command);
      Serial.print("Forward: ");
      Serial.println(value);
      forward(value);
      break;
    case 'b':
      value = parseCommand(command);
      Serial.print("Back: ");
      Serial.println(value);
      back(value);
      break;
    case 'l':
      value = parseCommand(command);
      Serial.print("Left: ");
      Serial.println(value);
      left(value);
      break;
    case 'r':
      value = parseCommand(command);
      Serial.print("Right: ");
      Serial.println(value);
      right(value);
      break;
    case 's':
      Serial.println("Stop");
      stop();
      break;
    case 't':
      value = parseCommand(command);
      Serial.println("Test");
      test(value);
      break;
    default:
      Serial.print("Unrecognized Command: ");
      Serial.println(command);
      break;
  }
}


void forward(int value) {
  if (value > 255) {
    value = 255;
  }
  move(MOTOR_L, value, 0);
  move(MOTOR_R, value, 1);
}


void back(int value) {
  if (value > 255) {
    value = 255;
  }
  move(MOTOR_L, value, 1);
  move(MOTOR_R, value, 0);
}


void left(int value) {
  if (value > 255) {
    value = 255;
  }
  move(MOTOR_R, 0, 0);
  move(MOTOR_L, value, 0);
}


void right(int value) {
  if (value > 255) {
    value = 255;
  }
  move(MOTOR_L, 0, 0);
  move(MOTOR_R, value, 1);
}


/*
 * Move specific motor at speed and direction
 * motor: 0 for B 1 for A
 * speed: 0 is off, and 255 is full speed
 * direction: 1 clockwise, 0 counter-clockwise
 */
void move(int motor, int speed, int direction) {
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise
  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (direction == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  } else {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}


void stop(){
  //enable standby 
  digitalWrite(STBY, LOW);
}


void test(int value) {
  forward(value);
  delay(1000);
  back(value);
  delay(1000);
  left(value);
  delay(1000);
  right(value);
  delay(1000);
  stop();
}


int parseCommand(char c[]) {
  char value[BUFFER_SIZE-1];

  for (int i = 1; i < BUFFER_SIZE; i++) {
    value[i-1] = c[i];
  }
  return atoi(value);
}
