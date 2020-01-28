//CPSC 599 Assignment 1: The Self-Preserving Light Switch
//Author: Greyson Mannella (30009538)
//-------------------------------------------------------------------------------
//This program allows the user to activate an LED light with a pushbutton.
//If the LED is on for ten seconds, a three-second alarm will sound.
//The alarm will continue to sound every ten seconds until LED is turned off.
//Once LED is turned off, the alarm will no longer sound.
//-------------------------------------------------------------------------------
//Includes the use of the millisDelay library,
//in order to delay the alarm tone without halting the program.
//millisDelay is (c) 2018 Forward Computing and Control Pty. Ltd.,
//http://forward.com.au.
//-------------------------------------------------------------------------------

#include <millisDelay.h>               //include millisDelay library

int ledpin = 3;                        //init pin for LED 
int buttonpin = 0;                     //init pin for pushbutton
int speakerpin = 7;                    //init pin for piezo
int val = 0;                           //init button input signal variable
bool pressed = false;                  //init button press flag
millisDelay delayTimer;                //init delay timer


void setup() {
  pinMode(ledpin, OUTPUT);              //declare LED pin as output
  pinMode(buttonpin, INPUT);            //declare pushbutton pin as input
  pinMode(speakerpin, OUTPUT);          //declare piezo pin as output
}


void loop() {
  val = digitalRead(buttonpin);         //read the current pushbutton signal

//if pushbutton has been pressed...
  if (val == LOW){                      
    delay(100);                         //...delay 100ms for button press time...
    pressed = !pressed;                 //...negate button press flag...
    digitalWrite(ledpin, pressed);      //...update current state of the LED...
    delayTimer.start(10000);            //...and start a delay timer for 10000ms.
  }
  
//if delay timer is up & button has been pressed...
  if (delayTimer.justFinished() && pressed) { 
    tone(speakerpin, 800, 3000);        //...sound an alarm for 3 seconds...
    delayTimer.start(10000);            //...and restart the delay timer.
  }
}
