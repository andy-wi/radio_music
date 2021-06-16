// Test sketch for switching between 2 wavetables 

#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <stdint.h>
#include <sample_data.h> 

#include <AudioSynthWavetable.h>
#include <AudioSynthWavetable.h>
#include <AudioSynthWavetable.h>
#include "EPRoll_samples.h"
#include "Keyngdom_samples.h"
#include "HollowDeep_samples.h"

AudioSynthWavetable wavetable;
AudioSynthWavetable wavetable1;
AudioSynthWavetable wavetable2;
AudioOutputAnalog dac1;
AudioMixer4 mixer;
AudioMixer4 mixer1;
AudioConnection patchCord1(wavetable, 0, mixer, 1);
AudioConnection patchCord2(wavetable1, 0, mixer, 2);
AudioConnection patchCord3(wavetable2, 0, mixer1, 2);
AudioConnection patchCord4(mixer, 0, mixer1, 1);
AudioConnection patchCord5(mixer1, dac1);

// Bounce objects to read pushbuttons 
Bounce button0 = Bounce(8, 15);  // button is digital pin 8

void setup() { 
  Serial.begin(115200);
  pinMode(8, INPUT_PULLUP);  // reset input is digital pin 9
  AudioMemory(20);
  mixer.gain(1, 0.9);
  mixer.gain(2, 0.9);
  mixer1.gain(1, 0.9);
  mixer1.gain(2, 0.9);
  wavetable.setInstrument(EPRoll);
  wavetable.playFrequency(700);
  wavetable.amplitude(0);
  wavetable1.setInstrument(Keyngdom);
  wavetable1.playFrequency(700);
  wavetable1.amplitude(0);
  wavetable2.setInstrument(HollowDeep);
  wavetable2.playFrequency(700);
  wavetable2.amplitude(0); 
  
}

// int mixer_setting = 0; 
// int mixer1_setting = 0;

void loop() {
  // Update all the button objects
  button0.update();

  //Read CV Input as 1v per Octave
 float CV1 = analogRead(A6);  // A8 is the other input (Start)
//  float in = analogRead(A9); 
//  float note = (in/16);
  float note = (CV1 /26) + 24;  // range 3.3 octaves, convert to midi note numbers 0 to 63

//Pitch drift/detune
float CV2 = analogRead(A8); 
float drift = (CV2 / 2046);   // range .5 of midi note number 

//Update frequency
  float note_mod = (note - drift);
  float freq = (float) 440.0 *(float) (pow(2, (note_mod - 57) /12.0)); 
 
//Mix the output waveforms using the pot 
  int pot1 = analogRead(A9);  // pot1 = 0 to 1023, A7 is the other pot (Start)
  float gain1 = (float)pot1 / 1023.0;
  float gain2 = 1.0 - gain1;
// float gain1 = 0.3;
// float gain2 = 0.7;
  mixer.gain(1, gain1);
  mixer.gain(2, gain2);

  int pot2 = analogRead(A7); 
  float gain3 = (float)pot2 / 1023.0;
  float gain4 = 1.0 - gain3; 
  mixer1.gain(1, gain3);
  mixer1.gain(2, gain4); 

  //Get frequency and gain from pots
 // float freq = (float)pot1*3.0;
 float gain = 0.9;
 
  // Update frequency & amplitude
  wavetable.amplitude(gain);
  wavetable.setFrequency(freq);
  wavetable1.amplitude(gain);
  wavetable1.setFrequency(freq);
  wavetable2.amplitude(gain);
  wavetable2.setFrequency(freq);

//   Serial.print ("note = ");Serial.print (note);
//         Serial.println();
//      delay(500);
//       Serial.print ("drift = ");Serial.print (drift);
//         Serial.println();
//      delay(500);
//  Serial.print ("note_mod = ");Serial.print (note_mod);
//         Serial.println();
//      delay(500);Serial.print ("freq = ");Serial.print (freq);
//         Serial.println();
//      delay(500);Serial.println();
//       delay(500);

//Bank LEDs are digital pins 3,4,5,6, Reset LED is pin 11
  
}
