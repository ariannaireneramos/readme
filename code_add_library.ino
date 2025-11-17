/* 

Arianna Ramos 

11/12/2025 

 

credit: Michael Klements, USF Department of Engineering 

*/ 

 

/* 

basic pin set up (this is for sobhi, remember this and KEEP TRACK) 

 

use H-bridge set up that we used for the origami bot, all the pins are the same. 

if someone has the set up still we can use that... ig 

 

SERVO (little blue motor) is going to be attached to PIN 10 on the uno, if this changes the code has to be changed accordingly 

 

another note i hope this works because i feel as if this is getting hella long.... 

*/ 

 

#include <LiquidCrystal_I2C.h> //liquid crystal library 

LiquidCrystal_I2C lcd(0x27, 16, 2); // specifices how to refrence the LCD and it's characteristics 

 

#include <Servo.h> //servo library 

Servo servo; 

 

int maxDist = 150; // max/stopping distance 

int stopDist = 50; 

float timeOut = 2*(maxDist+10)/100/340*1000000; //how long the ultra will check 

 

char array1[] = "Robot State: "; //edit this.. make it cute ask questions LOLL) 

char array2[] = "STOPPED "; 

char array3[] = "BACKWARD "; 

char array4[] = "FORWARD "; 

char array5[] = "OUT OF RANGE "; 

 

int echoPin = 8; // attach pin D8 to pin Echo of HC-SR04 

int trigPin = 7; // attach pin D7 to pin Trig of HC-SR04 

 

int in1 = 4; // variable names for the L298N 

int in2 = 5; // variable names for the L298N 

// in1 + 2 are the LEFT WHEELS 

 

int in3 = 9; // variable names for the L298N 

int in4 = 10; // variable names for the L298N 

// in3 + 4 are the RIGHT WHEELS 

 

/* 

note: i dont know how the pins are supposed to be attached to the wheels.. lol.. 

if the code runs backwards, switch top and bottom pins 

*/ 

int LCDprev = 99; // helps to update the LCD state tracks previous 

int LCDcurr = 100; // helps to update the LCD state tracks current 

 

int ledpin = 13; //specify the LED pin to pin 13 

 

long duration; // duration of sound wave travel 

int distance; // distance measurement 

 

void setup() { 

lcd.init(); // initializes LCD 

lcd.backlight(); // turn on backlight 

 

pinMode (trigPin, OUTPUT) ; // sonar digital pin mode for trig 

pinMode (echoPin, INPUT); // sonar digital pin mode for echo 

pinMode (in1, OUTPUT); // all L298N digital pins are outputs 

pinMode (in2, OUTPUT); // all L298N digital pins are outputs 

pinMode (in3, OUTPUT); // all L298N digital pins are outputs 

pinMode (in4, OUTPUT); // all L298N digital pins are outputs 

pinMode (ledpin, OUTPUT) ; // configure LCD pin as an output 

 

Serial.begin (9600) ; // use default of 9600 baud rate for serial communication 

 

lcd.setCursor(0, 0); // set cursor at top left 

 

lcd.print (array1); //prints the first line "Robot State: 

servo.attach(10); //attach servo to pin 10 

} 

 

void loop() 

{ 

/*  

NOTE: it is important only to update the LCD screen when a change of state has occured 

Otherwise, the program become inefficient. We only want to write to the LCD when  

there is a change in what the robot is doing. The LCDprey and LCDcurr variables  

help us to do this 

*/ 

LCDprev = LCDcurr; 

 

// Clears the trigPin condition 

digitalWrite (trigPin, LOW);//starts trig pin low 

delayMicroseconds (2); //waits for hardware to respond 

digitalWrite (trigPin, HIGH); //makes trig pin high 

delayMicroseconds (10) ; //continues high trig pin state for 10 microseconds 

digitalWrite (trigPin, LOW); //after 10 microseconds trig pin is brought low 

duration = pulseIn (echoPin, HIGH); //reads echo as it receives the pulse and stores duration 

distance = duration * 0.034 / 2; // Converts the time of flight to distance to object 

 

// Displays the distance on the serial Monitor 

Serial.print ("Distance: "); 

Serial.print (distance); 

Serial.println(" cm"); 

 

 

 

servo.write(90); //servo looks to 90 degree marker (directly in front) 

delay(750); 

int distance = getDistance(); //int to check if there are no object ahead 

if(distance >= stopDist) //if there are no objects within stopping distance, move forward 

{ 

LCDcurr = 3; 

 

moveForward(); 

digitalWrite(ledpin, HIGH); 

 

if (LCDprev != LCDcurr) { //checks validity of LCD display 

lcd.setCursor(0, 1); 

lcd.print(array4); 

delay(10); 

while(distance >= stopDist) //checks to make sure initial if value is still true 

{ 

distance = getDistance(); 

delay(250); 

} 

stopMove(); //stops all motors 

int turnDir = checkDirection(); //checks left and right object distances and receives turning instruction 

Serial.print(turnDir); 

switch (turnDir) 

{ 

case 0: //turn left 

turnLeft (400); 

break; 

case 1: //turn around 

turnLeft (700); 

break; 

case 2: //turn right 

turnRight (400); 

break; 

} 

} 

} 

} 

 

void moveForward() //all motors run forward 

{ 

digitalWrite(in1, HIGH); 

digitalWrite(in2, LOW); 

digitalWrite(in3, HIGH); 

digitalWrite(in4, LOW); 

} 

 

void stopMove() //all motors stop 

{ 

digitalWrite(in1, LOW); 

digitalWrite(in2, LOW); 

digitalWrite(in3, LOW); 

digitalWrite(in4, LOW); 

} 

 

void turnLeft(int duration) //motors turn left for a certain period 

{ 

digitalWrite(in1, HIGH); 

digitalWrite(in2, LOW); 

digitalWrite(in3, LOW); 

digitalWrite(in4, HIGH); 

delay(duration); 

digitalWrite(in1, LOW); 

digitalWrite(in2, LOW); 

digitalWrite(in3, LOW); 

digitalWrite(in4, LOW); 

} 

void turnRight(int duration) 

{ 

digitalWrite(in1, LOW); 

digitalWrite(in2, HIGH); 

digitalWrite(in3, HIGH); 

digitalWrite(in4, LOW); 

delay(duration); 

digitalWrite(in1, LOW); 

digitalWrite(in2, LOW); 

digitalWrite(in3, LOW); 

digitalWrite(in4, LOW); 

} 

int getDistance() //measure the distance to an object 

{ 

unsigned long pulseTime; //create a vairable to store the pulse travel time 

int distance; //create a variable to store the caclulated distance 

digitalWrite(trigPin, HIGH); //create 10 microsecond pulse 

delayMicroseconds(10);  

digitalWrite(trigPin, LOW); 

pulseTime = pulseIn(echoPin, HIGH, timeOut); //measure the time for the pulse to return 

distance = (float)pulseTime * 340 / 2 / 10000; //calculate the object distance based on the pulse time 

return distance; 

} 

 

int checkDirection() //checks left and right directions and decides which way to turn 

{ 

int distances [2] = {0,0}; //left and right distances 

int turnDir = 1; //Direction to turn, 0 left, 1 reverse, 2 right 

servo.write(180); //turn servo to look left 

delay(500); 

distances [0] = getDistance(); //get the left object distance 

servo.write(0); //turn the servo to look right 

delay(1000); 

distances [1] = getDistance(); //get the right object distance 

if (distances[0] >=200 && distances[1]>=200) //if both directions are clear, turn left 

turnDir = 0; 

else if (distances[0] <= stopDist && distances[1] <= stopDist) //if both directions are blocked, turn around 

turnDir = 1; 

else if (distances[0] >= distances[1]) //if left has more space, turn left 

turnDir = 0; 

else if (distances[0] , distances[1]) //if right as more sapce, turn right 

turnDir = 2; 

return turnDir; 

} 