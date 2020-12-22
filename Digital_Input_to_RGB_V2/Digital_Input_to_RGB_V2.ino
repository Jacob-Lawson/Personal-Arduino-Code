/*
    Digital Input to RGB V2
    Created by: Jacob Lawson
    Date: 02/06/2020
*/

// Include libraries
#include <pitches.h>
#include <Servo.h>

// RGB LED output ports
#define BLUE 3
#define GREEN 5
#define RED 6

// button input ports
int buttonApin = 10;
int buttonBpin = 9;
int buttonCpin = 8;

// internal LED port
int BRDLED = 13;

//the pin of the active buzzer
int buzzer = 12;

// define variables
int redValue;
int greenValue;
int blueValue;
int ColourCycle = 255;
int LEDSetupLoopNo;
int SERVOSetupLoopNo;
String cycleTxt="  cycle #: ";
int melody1[] = {      // passive buzzer melody setup
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int melody2[] = {      // passive buzzer SOTW melody setup 13 notes
  NOTE_D4, NOTE_F4, NOTE_G4, NOTE_D4, NOTE_F4, NOTE_GS4, NOTE_G4, NOTE_D4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_D4};
int MelodyT1 = 200;  // 500 miliseconds
int MelodyT2 = 800; //  800 miliseconds
int pos = 0;    // variable to store the servo position

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

void setup() 
{
  // set pins to output
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // write to pins
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  // default to high and set low when pressed 
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);  
  pinMode(buttonCpin, INPUT_PULLUP);  

  // initialise the buzzer pin as an output
  pinMode(buzzer,OUTPUT);

  // initialise mode of pin ball switch
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  // Initialise servo
  myservo.attach(11);  // attaches the servo on pin 9 to the servo object
  myservo.write(pos); 
  delay(15);
  
  // sets internal LED pin to output
  pinMode(BRDLED, OUTPUT);

  // sets up serial port to communicate data back to the serial monitor
  Serial.begin(9600); // 9600 is the baud rate which is the speed of commuication to the serial port

  // output cycle to internal LED shows successful setup
  Serial.println("Please enter Number of LED setup Cycles");
  while(Serial.available()==0){
        // makes the code wait until a number is entered 
  }
  LEDSetupLoopNo = Serial.parseInt();
  int i = 0;
  
  Serial.println("beginning board LED setup cycle");
  while (i < LEDSetupLoopNo) // cycles BRDLED setup 10 times
    {
      for (int thisNote = 0; thisNote < 13; thisNote++) {
        // pin12 output to buzzer, every scale is 0.5 sencond
        tone(12, melody2[thisNote], MelodyT2);
    
        digitalWrite(BRDLED, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(100);               // wait for a set period
        digitalWrite(BRDLED, LOW);    // turn the LED off by making the voltage LOW
        delay(10);               // wait for a set period
        digitalWrite(BRDLED, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(20);               // wait for a set period
        digitalWrite(BRDLED, LOW);    // turn the LED off by making the voltage LOW
        delay(800);               // wait for a set period
      }
    Serial.print(cycleTxt);
    Serial.println(i);
    i++;
    }
    Serial.println("ending board LED setup cycle");
    Serial.println();

    // output cycle to Servo shows successful setup
    Serial.println("Please enter Number of Servo setup Cycles");
    while(Serial.available()==0){
        // makes the code wait until a number is entered 
    }
    SERVOSetupLoopNo = Serial.parseInt();
    i = 0;
    
    Serial.println("starting motor servo setup");
    while (i < SERVOSetupLoopNo) // cycles BRDLED setup 10 times
      {
          for (pos = 0; pos <= 180; pos += 1) {          // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
          }
          for (pos = 180; pos >= 0; pos -= 1) {         // goes from 180 degrees to 0 degrees
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
          }
          for (int thisNote = 0; thisNote < 8; thisNote++) {
            // pin12 output to buzzer, every scale is 0.5 sencond
            tone(12, melody1[thisNote], MelodyT1);
            delay(100);
          }
          Serial.print(cycleTxt);
          Serial.println(i);
          i++;
      }
      Serial.println("ending Servo setup cycle");
      Serial.println();
}

void loop() 
{
  #define delayTime 10 // fading time between colors

  redValue = 0; // choose a value between 1 and 255 to change the color.
  greenValue = 0;
  blueValue = 0;

  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);

  int digitalVal = digitalRead(2); //read pin 2
  if(HIGH == digitalVal)
  {
    digitalWrite(BRDLED,LOW);//turn the led off
    // tone(buzzer,349); // note F            
    delay(10);
    noTone(buzzer); 
  }
  else
  {
    digitalWrite(BRDLED,HIGH);//turn the led on
    // tone(buzzer,392); // note G        
    delay(10);
    noTone(buzzer); 
  }
  
  if (digitalRead(buttonApin) == LOW) // pin A pressed will hold red for 1 sec and then cycle through to green
  {
    redValue = 255; // choose a value between 1 and 255 to change the color.
    greenValue = 0;
    blueValue = 0;

    Serial.println("beginning RGB LED cycle from red to green");
    
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    delay(1000); 
    
      for (int i = 0; i < ColourCycle; i += 1) // fades out red bring green full when i=255
    {
      redValue -= 1;
      greenValue += 1;
      // The following was reversed, counting in the wrong directions
      // analogWrite(RED, 255 - redValue);
      // analogWrite(GREEN, 255 - greenValue);
      analogWrite(RED, redValue);
      analogWrite(GREEN, greenValue);
      delay(delayTime);

      tone(12, NOTE_C5, 100); // note C 5th octive
      delay(1);//wait for 1ms
      digitalWrite(buzzer,LOW);
      delay(1);//wait for 1ms
      Serial.print(cycleTxt);
      Serial.println(i);
    }
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
    noTone(buzzer); 
    Serial.println("completed RGB LED cycle from red to green");
    Serial.println();
  }

  
  if (digitalRead(buttonBpin) == LOW) // pin B pressed will hold green for 1 sec and then cycle through to blue
  {
    redValue = 0; // choose a value between 1 and 255 to change the color.
    greenValue = 255;
    blueValue = 0;

    Serial.println("beginning RGB LED cycle from green to blue");
   
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    delay(1000); 

      for (int i = 0; i < ColourCycle; i += 1) // fades out green bring blue full when i=255
      {
        greenValue -= 1;
        blueValue += 1;
        // The following was reversed, counting in the wrong directions
        // analogWrite(GREEN, 255 - greenValue);
        // analogWrite(BLUE, 255 - blueValue);
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
        delay(delayTime);
        
        tone(12, NOTE_E5, 100); // note E 5th octive
        delay(1);//wait for 1ms
        digitalWrite(buzzer,LOW);
        delay(1);//wait for 1ms
        Serial.print(cycleTxt);
        Serial.println(i);
      }
      analogWrite(RED, 0);
      analogWrite(GREEN, 0);
      analogWrite(BLUE, 0);
      noTone(buzzer); 
      Serial.println("completed RGB LED cycle from green to blue");
      Serial.println();
  }


  if (digitalRead(buttonCpin) == LOW) // pin C pressed will hold blue for 1 sec and then cycle through to red
  {
    redValue = 0; // choose a value between 1 and 255 to change the color.
    greenValue = 0;
    blueValue = 255;

    Serial.println("beginning RGB LED cycle from blue to red");
    
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    delay(1000); 

        for (int i = 0; i < ColourCycle; i += 1) // fades out blue bring red full when i=255
      {
        blueValue -= 1;
        redValue += 1;
        // The following was reversed, counting in the wrong directions
        // analogWrite(BLUE, 255 - redValue);
        // analogWrite(RED, 255 - greenValue);
        analogWrite(RED, redValue);
        analogWrite(BLUE, blueValue);
        delay(delayTime); 
  
        
        tone(12, NOTE_G5, 100); // note G 5th octive
        delay(1);//wait for 1ms
        digitalWrite(buzzer,LOW);
        delay(1);//wait for 1ms
        Serial.print(cycleTxt);
        Serial.println(i);
      }
      analogWrite(RED, 0);
      analogWrite(GREEN, 0);
      analogWrite(BLUE, 0);
      noTone(buzzer); 
      Serial.println("completed RGB LED cycle from blue to red");
      Serial.println();
  }

}
