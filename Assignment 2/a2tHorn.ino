//CPSC 599 Assignment 2: The T-Horn
//Author: Greyson Mannella & Will Ehman
//-------------------------------------------------------------------------------
//This program allows the user to use an arduino with four pushbuttons and an
//accelerometer as a "Digital Trumpet". It uses X-axis position to determine the
//range of notes being played, analogous to the speed of buzzing lips when
//playing a standard trumpet. It uses the standard Bb trumpet fingerings for 
//notes C4 to E5. "Open" fingerings (i.e. fingerings where none of the three
//pads on a standard trumpet are pressed down) are played with a separate button
//located on the back for ease of playing.
//-------------------------------------------------------------------------------
//This program uses a "smooth" function to smooth out/stabilize accelerometer
//input. "smooth" is (c) 2020 Chris Ruppel. Function is available at 
//https://chrisruppel.com/blog/arduino-analog-signal-input-smoothing/.
//-------------------------------------------------------------------------------

#include "pitchesTransposed.h"

int cuePin = 2;         //initialize pin for back "cue" button
int buttonPinOne = 3;   //initialize pin for fingerpad 1
int buttonPinTwo = 4;   //initialize pin for fingerpad 2
int buttonPinThree = 5; //initialize pin for fingerpad 3
int speakerPin = 12;    //initialize speaker pin

int valZero = 0;        //initialize variable for cue button digital read
int valOne = 0;         //initialize variable for button 1 digital read
int valTwo = 0;         //initialize variable for button 2 digital read
int valThree = 0;       //initialize variable for button 3 digital read

int pitch = 0;          //intialize variable that changes based on accelerometer thresholds
int smoothValue = 0;    //initalize variable for analog accelerometer input


//Note that our input pins utilize the Arduino Uno's built-in resistors, rather than
//external pull-down resistors.
void setup() {
  pinMode(cuePin, INPUT_PULLUP);          //declare cue button pin as input
  pinMode(buttonPinOne, INPUT_PULLUP);    //delcate button 1 pin as input
  pinMode(buttonPinTwo, INPUT_PULLUP);    //delcate button 2 pin as input
  pinMode(buttonPinThree, INPUT_PULLUP);  //delcate button 3 pin as input
  pinMode(speakerPin, OUTPUT);            //declare piezo pin as output
  Serial.begin(9600);                     //allow for readout of accelerometer's analog output
}

void loop() {
  Serial.println(smooth());               //print accelerometer readout to serial monitor
  int smoothValue = smooth();             //cast smooth() function output to a variable

  //The following switch statements bin the accelerometer's X-axis position into one of
  //four predetermined threshold values. These values were based on the approximate
  //analog readout of the accelerometer when held flat, at 45 degree angles, and at 90
  //degree angles in both directions. This restricts the pitch range to some extent,
  //but it allows for precision when tilting to choose a certain pitch.
  
  if (smoothValue < 290) {
    pitch = 1;                            //lowest angle - trumpet bell towards floor
  } 
  
  if (smoothValue <= 337 && smoothValue >= 290) {
    pitch = 2;
  }
  
  if (smoothValue <= 377 && smoothValue > 337) {
    pitch = 3;
  }
  
  if (smoothValue > 377) {
    pitch = 4;                            //highest angle - trumpet bell towards ceiling
  } 

  //digitalRead values are negated to account for the internal pullup resistors, rather
  //than external pull-down resistors used in the first prototype.
  
  valZero = not digitalRead(cuePin);          //read cue pin & cast to variable
  valOne = not digitalRead(buttonPinOne);     //read button 1 pin & cast to variable
  valTwo = not digitalRead(buttonPinTwo);     //read button 2 pin & cast to variable
  valThree = not digitalRead(buttonPinThree); //read button 3 pin & cast to variable

  //"Open" position - only the button on the back is pressed.
  //This fingering maps to four different pitches within the selected range,
  //and thus plays four different notes depending on the accelerometer input.
  if (valZero == HIGH && valOne == LOW && valTwo == LOW && valThree == LOW) {
    if (pitch == 1){
      tone(speakerPin, NOTE_C4);
    }
    if (pitch == 2){
      tone(speakerPin, NOTE_G4);
    }
    if (pitch == 3){
      tone(speakerPin, NOTE_C5);
    }
    if (pitch == 4){
      tone(speakerPin, NOTE_E5);
    }
  }

  //"123" position - all three fingerpads pressed.
  //This fingering only maps to one note in range, so it plays the same note
  //regardless of X-axis position.
  if (valOne == HIGH && valTwo == HIGH && valThree == HIGH) {
    tone(speakerPin, NOTE_CS4);
  }

  //"12" position - first two fingerpads pressed.
  //This fingering maps to three notes on the lower end of the staff,
  //so it only uses the first three pitch thresholds.
  if (valOne == HIGH && valTwo == HIGH && valThree == LOW) {
    if (pitch == 1){
      tone(speakerPin, NOTE_E4);
    }
    if (pitch == 2){
      tone(speakerPin, NOTE_A4);
    }
    if (pitch == 3){
      tone(speakerPin, NOTE_CS5);
    }
  }

  //"13" position - first and third fingerpads pressed.
  //This fingering maps to one note in range.
  if (valOne == HIGH && valTwo == LOW && valThree == HIGH) {
    tone(speakerPin, NOTE_D4);
  }

  //"23" position - second and third fingerpads pressed.
  //This fingering maps to two notes on the lower end of the staff,
  //and thus anticipates accelerometer position within the two lower ranges.
  if (valOne == LOW && valTwo == HIGH && valThree == HIGH) {
    if (pitch == 1){
      tone(speakerPin, NOTE_DS4);
    }
    if (pitch == 2){
      tone(speakerPin, NOTE_GS4);
    }
  }

  //"2" position - middle fingerpad pressed.
  //This maps to four different notes within range.
  if (valOne == LOW && valTwo == HIGH && valThree == LOW) {
    if (pitch == 1){
      tone(speakerPin, NOTE_FS4);
    }
    if (pitch == 2){
      tone(speakerPin, NOTE_B4);
    }
    if (pitch == 3){
      tone(speakerPin, NOTE_DS5);
    }
    if (pitch == 4){
      tone(speakerPin, NOTE_FS5);
    }
  }

  //"1" position - first fingerpad pressed.
  //This maps to four different notes within range.
  if (valOne == HIGH && valTwo == LOW && valThree == LOW) {
    if (pitch == 1){
      tone(speakerPin, NOTE_F4);
    }
    if (pitch == 2){
      tone(speakerPin, NOTE_AS4);
    }
    if (pitch == 3){
      tone(speakerPin, NOTE_D5);
    }
    if (pitch == 4){
      tone(speakerPin, NOTE_F5);
    }
  }

  //Note that we do not account for the "3" fingering, since that fingering
  //maps to notes that are outside our range. To avoid overcomplicating the 
  //necessary tilt position, this fingering does not output tones to the speaker.

  //No buttons pressed - cues the speaker to stop producing sound.
  if (valZero == LOW && valOne == LOW && valTwo == LOW && valThree == LOW) {
    noTone(speakerPin);
  }
}

//smooth function 
int smooth() {
  int i;
  int value = 0;
  int numReadings = 40;                   //number of readings changed to increase stability.

  for (i = 0; i < numReadings; i++){
    value = value + analogRead(A0);

    // 1ms pause adds more stability between reads.
    delay(1);
  }
  value = value / numReadings;

  return value;
}
