/*
 *  oneString midi controller firmware 
 *  Created by Wil Lindsay   wil@straytechnologies.com
 *  Please see bottom of file for credits, copying and copyright information
 *  
 *  This code is designed to work with the oneString controller circuit. 
 *  More information available at www.straytechnologies.com
 *  
 *  First Public Release : V1.0 May 15, 2012 
 *      V1.1       5-Jun-2012 
 *                 Added additional LED function
 *                 Corrected tuning jitter
 *                 Correction to octave ranging in One-Hand Mode
 *                 Improvements to Bend Modes
 *
 *      V1.2      17-Jul-2012 
 *                 Resolves bass jitter in one-hand mode seen with some setups
 *                 Improves "sticky fingers" bass retriggering on release in one-hand mode
 *                 Removed redundant note-off calls in One-Hand Mode
 *
 *  This work is reliant on the ardumidi library (a part of ttymidi).
 *  Ardumidi library (file 6.0) is provided and used unchanged. It's use is governed by the same 
 *  licensing as the oneString midi control firmware. More information provided in those source files. 
 *
 */


#include <ardumidi.h>

/* Pin constants */
const int pressureSensorPin = 4;
const int positionSensorPin = 5;
const int tuningPotPin = 3;
const int ledPin = 13;

const int dip1Pin = 12;
const int dip2Pin = 11;
const int dip3Pin = 10;
const int dip4Pin = 9;

/* vars */
int modeUpper;  //controller mode selected via dip switches 1-4
int modeLower;  // lower half 1-4
int midiChannel = 0;  //midi send channel

int sensorPosition; //0-1023 position from positionSensorPin
int sensorVelocity;  // from pressureSensorPin reading mapped to 0-127
int tuningPotVal; //1023-0 from tuning potentiometer

int note;  //0-127 midi note
int noteHolder;  //stores note to creat noteOff after bend 
int bendCenter;  //stores center position for bend calcs 
int bendAmount;
int bendHolder;
int modHolder; //stores modulation (cc01) one hand and CC modes
int cc7Holder;  //stores midi cc in CC modes
int cc20Holder; 
int cc21Holder;
int cc22Holder;

int noteStateFlag = 0;  
/* 0= noNote 1=midi_on 2=pitch_bend 3=noteOff */

void setup(){
  Serial.begin(38400);  //3250 for true midi: this is as close as we can get on FTDI without data loss
  pinMode(ledPin, OUTPUT);
  pinMode(dip1Pin,INPUT);
  pinMode(dip2Pin,INPUT);
  pinMode(dip3Pin,INPUT);
  pinMode(dip4Pin,INPUT);

  modHolder = 0;  //init holder

  for (int i=0;i<3;i++){      //blink x3 to show succesful load and initialization
    digitalWrite(ledPin,HIGH);
    delay(150);
    digitalWrite(ledPin,LOW);
    delay(150);
  }
}

void loop(){
  //**read the 3 sensors**
  sensorPosition = analogRead(positionSensorPin);
  sensorVelocity = map(analogRead(pressureSensorPin),0,1023,0,127);
  tuningPotVal = 1023-analogRead(tuningPotPin);

  // mode is a binary from dipswitch: bits 1 and 2 are modeUpper, bits 3 and 4 are modeLower
  modeUpper = ((digitalRead(dip1Pin)<<1)+(digitalRead(dip2Pin)));
  modeLower = ((digitalRead(dip3Pin)<<1)+(digitalRead(dip4Pin)));

  switch (modeUpper){  //determine mode and jump to that function to create midi output
  case 0:
    modeZero(sensorPosition, sensorVelocity, tuningPotVal);
    break;
  case 1:
    modeOne(sensorPosition, sensorVelocity, tuningPotVal);
    break;
  case 2:
    modeTwo(sensorPosition, sensorVelocity, tuningPotVal);
    break;
  case 3:
    modeThree(sensorPosition, sensorVelocity, tuningPotVal);
    break;
  } 
}

/* Mode0 : retrigger mode w/ open bass note *************************************************/
void modeZero(int mPosition, int mVelocity, int mTune){
  int bassNote;

  switch (modeLower){  //determine octave span from dipswitches
  case 0:  //(1 octive)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible notes to FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+13); //map to 1 octaves from bass
    if (note == bassNote+13){note = bassNote+12;} //Allows for full range scaling
    break;
  case 1:  //(2  octaves)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible notes to FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+25);
        if (note == bassNote+25){note = bassNote+24;} //Allows for full range scaling
    break;
  case 2:  //(4 octaves)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible notes to FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+49);
        if (note == bassNote+49){note = bassNote+48;} //Allows for full range scaling
    break;
  case 3:  //(full range)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible notes to FULL octaves
    note = map(mPosition,0,1023,bassNote,128);
        if (note == bassNote+128){note = bassNote+127;} //Allows for full range scaling
    break;
  }

  if (note > 127){ //error trap high end for notes higher than midi spec
    note = 127;
  }

  switch (noteStateFlag){
    //** each note has 4 stages: 0=wait for input, 1=turn note on, 2= hold note, 3= release note **

  case 0:  //do nothing - wait for press
    if (mVelocity > 0){
      noteStateFlag = 1;
      digitalWrite(ledPin,HIGH);
      //delay(100);
    }
    break;
  case 1:  //start note
    midi_note_on(midiChannel,note,mVelocity);//no velocity for pitchbend mode
    noteHolder = note;
    noteStateFlag = 2;
    break;
  case 2:  //hold note
    if (mVelocity > 0){
      if (note == noteHolder){  //change velovity
        midi_key_pressure(midiChannel,noteHolder,mVelocity); //check for velocity change
      } 
      else{   //or retrigger
        midi_note_off(midiChannel,noteHolder,0);
        midi_note_on(midiChannel,note,mVelocity);
        noteHolder=note;
      }
    }
    else{
      noteStateFlag = 3;
    }
    break;
  case 3: //end note
    midi_note_off(midiChannel,noteHolder,0);
    midi_note_off(midiChannel,note,0);
    noteStateFlag = 0;
    digitalWrite(ledPin,LOW);
    break;
  }
}

/* Pitch Bend Mode **************************************************************************/
void modeOne(int mPosition, int mVelocity, int mTune){
  int bassNote;

  switch (modeLower){  //determine octave span from dipswitches
  case 0:  //(1 octive)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible notes to FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+13); //map to 1 octaves from bass
    if (note == bassNote+13){note = bassNote+12;} //Allows for full range scaling
    break;
  case 1:  //(2  octaves)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible notes to FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+25);
        if (note == bassNote+25){note = bassNote+24;} //Allows for full range scaling
    break;
  case 2:  //(4 octaves)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible notes to FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+49);
        if (note == bassNote+49){note = bassNote+48;} //Allows for full range scaling
    break;
  case 3:  //(full range)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible notes to FULL octaves
    note = map(mPosition,0,1023,bassNote,128);
        if (note == bassNote+128){note = bassNote+127;} //Allows for full range scaling
    break;
  }

  if (note > 127){ //error trap high end for notes higher than midi spec
    note = 127;
  }

  switch (noteStateFlag){
    //** each note has 4 stages: 0=wait for input, 1=turn note on, 2= hold note, 3= release note **
  case 0: //wait for input
    //do nothing
    if (mVelocity > 0){
      noteStateFlag = 1;
      digitalWrite(ledPin,HIGH);
      //delay(100);
    }
    break;
  case 1:  //turn note on
    midi_note_on(midiChannel,note,mVelocity);//no velocity for pitchbend mode
    bendCenter = mPosition;
    noteHolder = note;
    noteStateFlag = 2;
    break;
  case 2:  //hold/change note
    if (mVelocity > 0){
      midi_key_pressure(midiChannel,noteHolder,mVelocity); //check for velocity change
      //find bendAmount
      if (mPosition > bendCenter){
        bendAmount=map(mPosition,bendCenter,1023,8192,16383);
      }
      if (mPosition < bendCenter){
        bendAmount=map(mPosition,0,bendCenter,0,8192);
      }
      if (mPosition <2){
        bendAmount=8192;
      }
        midi_pitch_bend(midiChannel,bendAmount);
    }
    else{
      noteStateFlag = 3;
    }
    break;
  case 3:   //turn note off
    midi_note_off(midiChannel,noteHolder,0);
    noteStateFlag = 0;
    bendAmount = 8192;
    digitalWrite(ledPin,LOW);
    break;
  }
}
/* one handed mode w/ modulation *******************************************************/
void modeTwo(int mPosition, int mVelocity, int mTune){
  int bassNote;
  boolean bendMode;
  if (mPosition < 4){ mPosition = 0;}//Resolves some analog jitter in ribbon
  switch (modeLower){
  case 0:  //(2 octive retrigger mode)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+25); //map to 1 octaves from bass
    if (note == bassNote+25){note = bassNote+24;} //Allows for full range scaling
    bendMode = false;
    break;
  case 1:  //(4 octaves retrigger mode)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+49);
    if (note == bassNote+49){note = bassNote+48;} //Allows for full range scaling
    bendMode = false;
    break;
  case 2:  //(2 octaves bend mode)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+25);
    if (note == bassNote+25){note = bassNote+24;} //Allows for full range scaling
    bendMode = true;
    break;
  case 3:  //(4 octaves bend mode)
    bassNote = map(mTune,0,1023,0,114);  //tune to all possible FULL octaves
    note = map(mPosition,0,1023,bassNote,bassNote+49);
    if (note == bassNote+49){note = bassNote+48;} //Allows for full range scaling
    bendMode = true;
    break;
  }

  if (note > 127){ //error trap high end for notes higher than midi spec
    note = 127;
  }

  if (mVelocity != modHolder){       //modulation (MIDI_CC 1) from pressure sensor
    midi_controller_change(midiChannel,1,mVelocity);
    modHolder = mVelocity;
  }
  
  if ((mVelocity > 0)||(noteStateFlag > 0)){   //LED on Pressure sensor or noteOn
    digitalWrite(ledPin,HIGH);
  }else{
    digitalWrite(ledPin,LOW);
  }


  if (bendMode == false){  //retrigger mode
    switch (noteStateFlag){
      //** each note has 4 stages: 0=wait for input, 1=turn note on, 2= hold note, 3= release note **
    case 0:
      //do nothing - wait for press
      if (mPosition > 1){
        delay(2);
        noteStateFlag = 1;
      }
      break;
    case 1:
      midi_note_on(midiChannel,note,127);//no velocity for pitchbend mode
      noteHolder = note;
      noteStateFlag = 2;
      delay(10);
      break;
    case 2:
      if (mPosition > 1){
        if (note != noteHolder){  // retrigger
          midi_note_off(midiChannel,noteHolder,0);
          midi_note_on(midiChannel,note,127);
          noteHolder=note;
        }
      }
      else{
        noteStateFlag = 3;
      }
      delay(30); //solves "sticky finger" lifts retriggering notes on release
      break;
    case 3:
      midi_note_off(midiChannel,noteHolder,0);
      //midi_note_off(midiChannel,note,0);
      noteStateFlag = 0;
      break;
    }
  } 
  else{   //** one-hand bend mode
    switch (noteStateFlag){
      //** each note has 4 stages: 0=wait for input, 1=turn note on, 2= hold note, 3= release note **
    case 0:
      //do nothing
      if (mPosition > 1){
        noteStateFlag = 1;
        delay(1);
        digitalWrite(ledPin,HIGH);
        //delay(100);
      }
      break;
    case 1:
      midi_note_on(midiChannel,note,127);//no velocity for pitchbend mode
      bendCenter = mPosition;
      noteHolder = note;
      noteStateFlag = 2;
      break;
    case 2:
      if (mPosition > 1){
        //find bendAmount
        if (mPosition > bendCenter){
          bendAmount=map(mPosition,bendCenter,1023,8192,16383);
        }
        if (mPosition < bendCenter){
          bendAmount=map(mPosition,0,bendCenter,0,8192);
        }
        midi_pitch_bend(midiChannel,bendAmount);
        delay(10); //prevent voltage bounce on release
      }
      else{
        noteStateFlag = 3;
        bendAmount = 8192;
        //midi_pitch_bend(midiChannel,bendAmount);
      }
      break;
    case 3:
      midi_note_off(midiChannel,noteHolder,0);
      noteStateFlag = 0;
      digitalWrite(ledPin,LOW);
      break;
    }

  }
}

/* CC mode (no Note_on or Off) **************************************************************/
void modeThree(int mPosition, int mVelocity, int mTune){
  int knobVal = map(mTune,0,1023,0,127);
  int posCCVal =map(mPosition,0,1023,0,127);

  if ((posCCVal > 0) || (mVelocity > 0)){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  switch (modeLower){     //CC20, modulation, & CC21
  case 0:
    if ((posCCVal != cc20Holder)&&(posCCVal >0)){  //(MIDI_CC 20) from position sensor
      midi_controller_change(midiChannel,20,map(mPosition,0,1023,0,127));
      cc20Holder = posCCVal;
    }else {
      if ((posCCVal != cc20Holder)&&(posCCVal == 0)){  //pull to zero if position sensor is not held
        midi_controller_change(midiChannel,20,0);
        cc20Holder = posCCVal;
      }
    }
    if (mVelocity != modHolder){       //modulation (MIDI_CC 1) from pressure sensor
      midi_controller_change(midiChannel,1,mVelocity);
      modHolder = mVelocity;
    }
    if (knobVal != cc21Holder){       //(MIDI_CC 21) from tuning pot
      midi_controller_change(midiChannel,21,knobVal);
      cc21Holder = knobVal;
    }
    break;
  case 1:   //cc20, cc21 & cc22
    if ((posCCVal != cc20Holder)&&(posCCVal >0)){       //(MIDI_CC 20) from position sensor
      midi_controller_change(midiChannel,20,map(mPosition,0,1023,0,127));
      cc20Holder = posCCVal;
    }else{
    if ((posCCVal != cc20Holder)&&(posCCVal == 0)){  //pull to zero if position sensor is not held
        midi_controller_change(midiChannel,20,0);
        cc20Holder = posCCVal;
      }
    }
    if (mVelocity != cc21Holder){       //(MIDI_CC 21) from pressure sensor
      midi_controller_change(midiChannel,21,mVelocity);
      cc21Holder = mVelocity;
    }
    if (knobVal != cc22Holder){       //(MIDI_CC 22) from tuning pot
      midi_controller_change(midiChannel,22,knobVal);
      cc22Holder = knobVal;
    }
    break;
  case 2:   //***   RESERVED   ***
    break;  
  case 3:   //classic ribbon mode with cc1 & cc7
    bendCenter = 512;
    if (mPosition > bendCenter){
      bendAmount=map(mPosition,bendCenter,1023,8192,16383);
    }
    if (mPosition < bendCenter){
      bendAmount=map(mPosition,0,bendCenter,0,8192);
    }
    if (mPosition <2){
      bendAmount=8192;
    }
    if (bendAmount != 8192){
      midi_pitch_bend(midiChannel,bendAmount);  //send bend
    }else{
      if(bendHolder != 8192){
        midi_pitch_bend(midiChannel,bendAmount); //return to zero on last touch 
      }
    }

    if (mVelocity != modHolder){       //modulation (MIDI_CC 1) from pressure sensor
      midi_controller_change(midiChannel,1,mVelocity);
      modHolder = mVelocity;
    }
    if (knobVal != cc7Holder){       //volume (MIDI_CC 7) from tuning pot
      midi_controller_change(midiChannel,7,knobVal);
      cc7Holder = knobVal;
    }
    bendHolder = bendAmount;
    break;
  }
}

/*
*    Copyright 2012 Wil Lindsay
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *    This file is dependent on the ardumidi library which is a part of ttymidi.
 *    As such, ardumidi is also distributed under the GNU General Public License 
 *    but is not distributed as part of this package. 
 *
 ****************************************************************************************
 *
 *  This project could never have been released as an open source project without the help 
 *  of the Kickstarter community (www.kickstarter.com), and folks who have supported my
 *  work over the years.  The project successfully went through a fund-raising process in order 
 *  to support the HUGE amount of time dedicated to circuit design, coding, testing,
 *  designing a PCB, setting up a project Wiki, and creating extensive documentation. 
 *  Over 60 backers showed financial support with many of them waiting patiently for a lengthy 
 *  development process and the reward of a DIY kit or instrument. A few folks simply donated 
 *  a few bucks with little expectation of reward, and it is that type of support that can 
 *  really make open-source work rewarding and viable.  As a small token, I'd like to list a 
 *  few of those folks here, where their thought can live with the project source-code and 
 *  everywhere it propagates. 
 *  
 *  Josh Sosnin 
 *  @RockNRollGeek : www.twitter.com/RockNRollGeek 
 *  Gold Beach  : www.goldbeachband.com 
 *  Jonathan (Jub) Hart 
 *  Jeffy Thomas  : www.cuttink.com 
 *  Steven Montez 
 *  Dave Moore : www.davemoorestudios.com 
 *  vodstrup 
 *  and MANY anonymous donors …  to all: Thanks!  
 */








