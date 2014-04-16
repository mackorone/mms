#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>

#include <avr/pgmspace.h> // Needed in order to recall accel table out of flash memory to save space

#include "FloodFill.h"
/*
  Notes: Tab order counts. variables declared in tabs only are accesible in later tabs
         This does not pertain to functions.  Functions in any tab can be used anywhere
         Put _XX_ before file names to order them by number XX. the leading _ is important
*/

void setup(){
  Serial.begin(9600);
  setupPins();
  setupTimers();
  attachInterrupt(5, leftDig, RISING);
  attachInterrupt(4, rightDig, RISING);
  delay(3000);
  FloodFill f;
  
  f.solve();
}

void loop(){
  //centerFirstBox();
  //_turnLeft();
  //turnRightSteps(38);
  //solve();
  //_moveForward(1);
  //_turnLeft();

}

void _resetHonored(){
}

boolean _resetRequested(){
  return false;
}

void _turnAround(){
  _turnRight(2);
  //_turnRight();
}
