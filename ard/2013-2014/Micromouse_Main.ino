#include <avr/pgmspace.h> // Needed in order to recall accel table out of flash memory to save space

/*
  Notes: Tab order counts. variables declared in tabs only are accesible in ltaer tabs
         This does not pertain to functions.  Functions in any tab can be used anywhere
         Put _XX_ before file names to order them by number XX. the leading _ is important
*/

void setup(){
  Serial.begin(9600);
  setupPins();
  setupTimers();
  delay(7500);
  attachInterrupt(5, leftDig, RISING);
  attachInterrupt(4, rightDig, RISING);
}

void loop(){
  //centerFirstBox();
  solve();
  //_moveForward(3);
  //_turnLeft();
  delay(1000);
}
