/* 
For LCD, using SerLCD, from Steven Cogswell.
// Sparkfun SerLCD.  https://www.sparkfun.com/products/9568
// https://github.com/scogswell/ArduinoSerLCD
*/

#include <AccelStepper.h> // stepper library. Using a minidriverfrom sparkfun.
#include <SoftwareSerial.h>
#include <SerLCD.h>
#define x_pin A0      // Pin A0 connected to JOYSTICK x axis pin
#define buttonJoy_pin 7 // Pin 7 connected to joystick's digital push action
#define step_pin 2    // Pin 2 connected to Steps pin on EasyDriver
#define dir_pin 3     // Pin 3 connected to Direction pin

// For Stepper Motor: Define a stepper and the pins it will use
AccelStepper stepper(1,2,3); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

// for LCD 
SoftwareSerial NSS(13,8);
SerLCD theLCD(NSS,20,4);

// VARIABLES 

// For Stepper Motor
long steps = 0;
long pos = 0;
int _direction;    // Variable to set Rotation (CW-CCW) of the motor

// Serial Monitor stuff = add a variable to calculate amount of steps to move
long askedpos;

// Motor control variables
int maximumspeed, accel, timer, stopWatch;

// for timer operations (reset)
unsigned long previousMillis=0;
unsigned long currentMillis = millis();  // for timer operations (reset)

// LCD strings variables
char stepsstring[10], maximumspeedstring[10], accelstring[10], timerstring[10], posAstring[10], posBstring[10]; // create string arrays

// for Joystick
// const int buttonJoy_pin = 7; // digital

// for Buttons
const int button1Pin = 9;  // pushbutton 1 pin
const int button2Pin = 10;  // pushbutton 2 pin
const int button3Pin = 11;  // pushbutton 3 pin
const int button4Pin = 12;  // pushbutton 3 pin
int positionA, positionB;

// Potentiometer for max speed and accelleration
int potPin = 1;    // select the input pin for the potentiometer
int potPin2 = 2;    // select the input pin for the potentiometer
int potspeedval = 0;       // variable to store the value coming from the sensor
int potaccelval = 0;       // variable to store the value coming from the sensor

void setup()
{
  // Setup the Joystick
  pinMode(dir_pin, OUTPUT);
  pinMode(x_pin, INPUT);
  pinMode(buttonJoy_pin, INPUT_PULLUP); //activate pull-up resistor on the push-button pin
  
  // Set up the pushbutton pins to be an input:
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(button4Pin, INPUT);
  
  // setup lcd  
  NSS.begin(9600);
  theLCD.begin(); // This will attempt to initialize the display to blank with the backlight on 

 // initialize serial:
  Serial.begin(57600);
  delay(1000);  // give enough time to open the serial monitor after uploading
  Serial.println("-------------------- ");
  Serial.println("Starting... enter desired position: "); 

  // LCD hidden cursor position
  theLCD.setPosition(1,1);   
  theLCD.print("Steps:");
  theLCD.setPosition(2,1);
  theLCD.print("Speed:");
  theLCD.setPosition(3,1);
  theLCD.print("Accel:");
  theLCD.setPosition(4,1);
  theLCD.print("PosA:");  
  theLCD.setPosition(4,11);
  theLCD.print("PosB:");   
  delay(500);  
}


// LOOP //
void loop() 
{
  // Potentiometer values to set max speed and acceleration
  potspeedval = analogRead(potPin);    // read the value from maxspeed pot
  potaccelval = analogRead(potPin2);    // read the value from accel pot
  
  // Motor stuff
  maximumspeed = (potspeedval - 24.2) * 3.8;
  accel = (potaccelval - 25) * 32;
  stepper.setMaxSpeed(maximumspeed);
  stepper.setAcceleration(accel);
  
  // Timer stuff
  timer = (currentMillis - previousMillis) /1000 ;

  // Buttons Stuff
  int button1State, button2State, button3State, button4State; // variables to hold the pushbutton states

  // Read the current pushbutton states
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  button4State = digitalRead(button4Pin);

  // Joystick stuff
  int buttonJoyState;
  buttonJoyState = digitalRead(buttonJoy_pin);
   
  // SERIAL MONITOR: set (enter) and go to position.
  if (Serial.available() > 0) {
    Serial.println("-----------");
    Serial.print("steps before: ");
    Serial.println(steps);
    askedpos = Serial.parseInt();
    delay(2);
    Serial.print("requested position: ");
    Serial.println(askedpos);
    Serial.print("move amount: ");
    Serial.println(askedpos - steps);
    
    stepper.move(askedpos - steps);

    // Re-calculate resulting steps value
    if (askedpos > steps)
      {steps = steps + askedpos;}
    if (askedpos < steps)
      {steps = steps - (steps - askedpos);}
      
    while (stepper.distanceToGo() != 0) 
    {stepper.run();}
    if (stepper.distanceToGo() == 0)
    {Serial.print("steps after move: ");
      Serial.println(steps);}
  }

  // JOYSTICK actions
  while (analogRead(x_pin) >= 570 && analogRead(x_pin) <= 1024) {
    if (steps < 11500) {      // Maximum steps the stepper can move away from the Home Position
      digitalWrite(dir_pin, HIGH);
      digitalWrite(step_pin, HIGH);
      delay(1);
      digitalWrite(step_pin, LOW);
      delay(1);
      steps++;  // Increase the number of steps taken

    }
  }
  
  while (analogRead(x_pin) >= 0 && analogRead(x_pin) <= 450) {
    //if (steps > 0) {  //  To make sure the Stepper doesn't go beyond the Home Position
      digitalWrite(dir_pin, LOW);  // (HIGH = anti-clockwise / LOW = clockwise)
      digitalWrite(step_pin, HIGH);
      delay(1);
      digitalWrite(step_pin, LOW);
      delay(1);
      steps--;   // Decrease the number of steps taken
    //}      
  }
  
  if (buttonJoyState == LOW)
  {
    Serial.println("buttonJoy_pin is pressed");
    stepper.move(0 - steps);
    while (stepper.distanceToGo() != 0)  
    {stepper.run();}
    steps = 0;
  }
  
  if (button1State == LOW)
  {
    positionA = steps;
    Serial.println("----------");
    Serial.print("PositionA: ");
    Serial.println(positionA);
  }
  
  if (button2State == LOW)
  {
    Serial.println("----------");
    Serial.print("Go to PositionA: ");
    Serial.println(positionA);  
    stepper.move(positionA - steps);
    steps = positionA;
    while (stepper.distanceToGo() != 0) 
    {stepper.run();}
    
  }
if (button3State == LOW)
  {
    positionB = steps;
    Serial.println("----------");
    Serial.print("PositionB: ");
    Serial.println(positionB);
  }
  
  if (button4State == LOW)
  {
    Serial.println("----------");
    Serial.print("Go to PositionB: ");
    Serial.println(positionB);  
    stepper.move(positionB - steps);
    steps = positionB;
    while (stepper.distanceToGo() != 0) 
    {stepper.run();}  
  }

 // LCD DISPLAY for target position, maximumspeed and acceleration. 
  sprintf(stepsstring,"%05d",steps); // create strings from the numbers
  sprintf(maximumspeedstring,"%05d",maximumspeed); // right-justify to 4 spaces
  sprintf(accelstring,"%05d",accel);
  sprintf(posAstring,"%04d", positionA);
  sprintf(posBstring,"%04d", positionB);
  delay(200); // if not long enough, the position entered in serial monitor might be chopped after 3 digits.
  
// Display motor values
  theLCD.setPosition(1,12);   
  theLCD.print(stepsstring);
  theLCD.setPosition(2,12);
  theLCD.print(maximumspeedstring);
  theLCD.setPosition(3,12);
  theLCD.print(accelstring);
  theLCD.setPosition(4,6);
  theLCD.print(posAstring);  
  theLCD.setPosition(4,16);
  theLCD.print(posBstring);  
  delay(2);

}

 
