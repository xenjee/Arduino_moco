/*
 Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */





 /* Notes sg:
 - I don't know if we need the setAcceleration when using a "on the fly" speed value (joystick)
 - The setSpeed method isn't what we use in animation curves. 
 - If a position channel stays constant, the object shouldn't move.
 - With animation channels, we need to drive a position value = void moveTo(long absolute); 
 
 "Record head": Drive with joystick and record to a channel:
 - Set the speed "on the fly" (Joystick) makes the motor move a certain amount of steps,
   given a MicroBetweenSteps value (=speed). I suspect that the speed is really just the MicroBetweenSteps value (?)
 - Count the number of steps achieved.
 - Calculate the position from the number of steps

 About the "setAcceleration": after reading this: https://groups.google.com/forum/#!searchin/accelstepper/motion$20control%7Csort:relevance/accelstepper/PXOuFJQ9WqU/qLirTeqcE_0J
 I wonder if it's the other way around: setAcceleration and dont use "setSpeed" (per mike M., the creator of the lib)
>>> TRY THIS (20170208) >>> 
- in the loop: //stepper.setAcceleration(((PS3.getAnalogHat(LeftHatY))-128)*3)
 */


#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
                                                       // sg - I'd like to hard code - ish the BT address (00:11:22:98:76:54) 
bool printTemperature, printAngle;

/**********************
ADDING BOUNCE INIT
***********************/

// Bounce.pde
// -*- mode: C++ -*-
//
// Make a single stepper bounce from one limit to another
//
// Copyright (C) 2012 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper (1,3,2); // Setting to DRIVER mode (1) and step/dir pins (order?)

/**********************
END OF BOUNCE INIT
***********************/

void setup() {
  Serial.begin(9600);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));


/**********************
ADDING BOUNCE SETUP
***********************/

 // Change these to suit your stepper if you want (original comment from creator - accelStepper Bounce example)

 
  stepper.setMaxSpeed(1000);
  //stepper.setAcceleration(200); // sg - let see if this is really needed, since it's expensive (square root calculation)
  stepper.setSpeed(0); // initialises to no move (for joystick)

  
/**********************
END OF BOUNCE SETUP
***********************/

  
}
void loop() {



  
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
   // then do something with: PS3.getAnalogHat(LeftHatX) or PS3.getAnalogHat(LeftHaty) or (PS3.getAnalogHat(RightHatX) or (PS3.getAnalogHat(RightHaty)
         stepper.setSpeed(((PS3.getAnalogHat(LeftHatY))-128)*5);
         //Serial.print(F("\r\nLeftHatY: "));
         //Serial.print(PS3.getAnalogHat(LeftHatY));

      if (PS3.PS3Connected) { // The Navigation controller only have one joystick
        // then do something with: (PS3.getAnalogHat(RightHatX) or (PS3.getAnalogHat(RightHaty)
      }
    }

    // Analog button values can be read from almost all buttons
    if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
        // then do something
      if (PS3.PS3Connected) {
        // then do something
      }
    }

    if (PS3.getButtonClick(PS)) {
      // then do something, (well, disconnect apparently)
      PS3.disconnect();
    }
    else {
      if (PS3.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTraingle"));
        PS3.setRumbleOn(RumbleLow);
        
      }
      if (PS3.getButtonClick(CIRCLE)) {
        // then do something
        PS3.setRumbleOn(RumbleHigh);
      }
      if (PS3.getButtonClick(CROSS))
        // then do something
      if (PS3.getButtonClick(SQUARE))
        // then do something

      if (PS3.getButtonClick(UP)) {
        // then do something
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED4);
        }
      }
      if (PS3.getButtonClick(RIGHT)) {
        // then do something
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED1);
        }
      }
      if (PS3.getButtonClick(DOWN)) {
        // then do something
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED2);
        }
      }
      if (PS3.getButtonClick(LEFT)) {
        // then do something
        if (PS3.PS3Connected) {
          PS3.setLedOff();
          PS3.setLedOn(LED3);
        }
      }

      if (PS3.getButtonClick(L1))
        // then do something
      if (PS3.getButtonClick(L3))
        // then do something
      if (PS3.getButtonClick(R1))
        // then do something
      if (PS3.getButtonClick(R3))
        // then do something

      if (PS3.getButtonClick(SELECT)) {
        // then do something
        PS3.printStatusString();
      }
      if (PS3.getButtonClick(START)) {
        // then do something
        printAngle = !printAngle;
      }
    }
#if 0 // Set this to 1 in order to see the angle of the controller
    if (printAngle) {
      Serial.print(F("\r\nPitch: "));
      Serial.print(PS3.getAngle(Pitch));
      Serial.print(F("\tRoll: "));
      Serial.print(PS3.getAngle(Roll));
    }
#endif
  }



/**********************
ADDING BOUNCE LOOP
***********************/

    stepper.run(); // sg - Is this needed ??? hmmm, it may.

/**********************
END OF BOUNCE LOOP
***********************/


}
