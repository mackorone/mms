bool _undoRequested(){
  ////Eriralprintln("*********UNDO REQUESTED************");
  return resetRunRequested;
}
// button is 5
// led is 2
void _undoHonored(){
  ////Eriralprintln("*********UNDO HONORED************");
  while(!digitalRead(5))
    delay(1);
  delay(500);
  digitalWrite(2, LOW);
  ////Eriralprintln("*********UNDO HONORED 2************");
  resetRunRequested = false;
}


void _resetPosition(){
  ////Eriral//Eriralprintln("*********RESET***********");
  delay(1500);
  while(digitalRead(5))
  {
    digitalWrite(2, HIGH);
    delay(50);
    digitalWrite(2, LOW);
    delay(50);
  }
  delay(3000);
}

void stopFrontWall(){
  correct = 0;
  while (stepper_enabled == true)
  {
      if(!(digitalRead(FRONT_DIGITAL)))
      {
         Estop();
         delay(100);
         stoppingFrontWall = 0;
      }
  }
  if (stoppingFrontWall == 1)
  {
    //bool initialFrontValue = digitalRead(FRONT_DIGITAL);
    //if (initialFrontValue)
    int temp_speed = speed_max;
    speed_max = 10;
    moveSteps(1000, 1);
    while(digitalRead(FRONT_DIGITAL))
      delay(1);
    Estop();
    speed_max = temp_speed;
  }
  stoppingFrontWall = 0;
  delay(80);
  syncMotors();
  right_stepper_target += 6; //
  left_stepper_target += 6; //
  
  digitalWrite(RIGHT_DIR_PIN, HIGH); 
  digitalWrite(LEFT_DIR_PIN, HIGH);
  
  stepper_enabled = true;
  
  delay(10);
}

void _moveForward(){
  int boxes = 1;
  if(boxes == 0)
    return;
  
  digitalWrite(RIGHT_DIR_PIN, HIGH); 
  digitalWrite(LEFT_DIR_PIN, HIGH);
  
  sensorAverage[LEFTFORWARDSENSOR][avgSamples +1] = 0; // set the sensor as reliable for the movement initially
  sensorAverage[RIGHTFORWARDSENSOR][avgSamples +1] = 0; //
  
  stoppingFrontWall = 0; // Method mouse is using to stop in the middle of a cell
  stoppingSideWall = 0;
    
  delay(80);
  
  if (stepper_enabled == false)
  {
    syncMotors();
  }
  right_stepper_target += FORWARD_STEPS * boxes; //
  left_stepper_target += FORWARD_STEPS * boxes; //
  
  stepper_enabled = true;
  
  int right1Sensor = sensorAverage[RIGHTFORWARDSENSOR][0];
  int left1Sensor = sensorAverage[LEFTFORWARDSENSOR][0];
    
  int right2Sensor = sensorAverage[RIGHTFORWARDSENSOR][1];
  int left2Sensor = sensorAverage[LEFTFORWARDSENSOR][1];
  
  sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] = 0;
  sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] = 0;
  
  if ( ((left1Sensor <= ANGLELEFTVALUE - ALLOWEDRANGE) || (left1Sensor >= ANGLELEFTVALUE + ALLOWEDRANGE) || (left2Sensor <= ANGLELEFTVALUE - ALLOWEDRANGE) || (left2Sensor >= ANGLELEFTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280) 
         && (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] == 0) ) // Check if sensor value is outside reliable range
  {
    sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] = RESETSTEPAMOUNT; // UNRELIABLE SENSOR FOR REST OF MOVE STEP COUNT AT WHICH IS WAS DECLARED UNRELIABLE
  }
  if ( ((right1Sensor <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (right1Sensor >= ANGLERIGHTVALUE + ALLOWEDRANGE) || (right2Sensor <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (right2Sensor >= ANGLERIGHTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280)
       && (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] == 0) )
  {
    sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] = RESETSTEPAMOUNT; // UNRELIABLE SENSOR FOR REST OF MOVE STEP COUNT AT WHICH IS WAS DECLARED UNRELIABLE 
  }
  correct = 1;
  if (moveBuffered == 1)
   moveBuffered = 2;
  if(boxes != 1)
    waitMovement();
}

int _turnLeft(){
  
  bufferedTurn = 2;
  waitMovement();
  correct = 0;
  
  moveBuffered = 0;
  
  digitalWrite(RIGHT_DIR_PIN, HIGH);
  digitalWrite(LEFT_DIR_PIN, LOW);
  
  // turn on 1/8 microstepping
 
  digitalWrite(48, HIGH); //MS 2
  digitalWrite(51, HIGH);  //MS 2
  
  int temp_speed = speed_max;
  int temp_accel = table_increment;
  table_increment = temp_accel + 8;
  speed_max = 500;
  
  delay(2); 
  syncMotors();
  right_stepper_target += TURN_STEPS;//79;
  left_stepper_target += TURN_STEPS;//79;
  //delay(1); // let microstepping go into effect
  stepper_enabled = true;
  
  delay(1); // Apears to fix the waitMovement somehow not waiting
  waitMovement();
  
  speed_max = temp_speed;
  table_increment = temp_accel;
  //
  digitalWrite(48, LOW); //MS 2
  digitalWrite(51, LOW);  //MS 2
  ////Eriral//Eriralprintln("---------------------");
  //delay(5000);
  delay(2);
}

int _turnRight(int times){
  
  bufferedTurn = 1;
  waitMovement();
  correct = 0;
  
  moveBuffered = 0;
  
  digitalWrite(RIGHT_DIR_PIN, LOW);
  digitalWrite(LEFT_DIR_PIN, HIGH);
  
  // turn on 1/8 microstepping
  
  digitalWrite(48, HIGH); //MS 2
  digitalWrite(51, HIGH);  //MS 2
  
  int temp_speed = speed_max;
  int temp_accel = table_increment;
  table_increment = temp_accel + 8;
  speed_max = 500;
  
  delay(2); 
  syncMotors();
  right_stepper_target += TURN_STEPS * times;//79;
  left_stepper_target += TURN_STEPS * times;//79;
  //delay(1); // let microstepping go into effect
  stepper_enabled = true;
  
  delay(1); // Apears to fix the waitMovement somehow not waiting
  waitMovement();
  
  speed_max = temp_speed;
  table_increment = temp_accel;
  //
  digitalWrite(48, LOW); //MS 2
  digitalWrite(51, LOW);  //MS 2
  //Eriralprintln("---------------------");
  //delay(5000);
  delay(2);
}


void syncMotors(){
  OCR3A = 100;
  OCR1A = 100;
  TCNT1 = 1;
  TCNT3 = 1;
  right_speed = 0;
  left_speed = 0;
  right_stepper_target = 0;
  left_stepper_target = 0;
  right_stepper_position = 0;
  left_stepper_position = 0;
  compensateRight(0);
  compensateLeft(0);
}

void waitMovement(){ // only useful for turns, all move forwards attempt error correction
  while(stepper_enabled == true)
  {
    if (stoppingFrontWall == 1)
      stopFrontWall();
    delay(1); // no sense wasting clock cycles during movement
  }
  delay(100); //Make sure the wheels come to a complete time
}

void compensateRight(int amount){
  if (amount <= -1)
    amount = -1 * pow(abs(amount), 0.5);
  left_correction = amount;
}

void compensateLeft(int amount){
  if (amount <= -1)
    amount = -1 * pow(abs(amount), 0.5);
  right_correction = amount;
}


void moveSteps(int steps, bool dir){
  digitalWrite(RIGHT_DIR_PIN, dir);
  digitalWrite(LEFT_DIR_PIN, dir);
  right_stepper_target += steps;
  left_stepper_target += steps;
  stepper_enabled = true;
  
  delay(1);
}

void turnLeftSteps(int steps){
  digitalWrite(RIGHT_DIR_PIN, HIGH);
  digitalWrite(LEFT_DIR_PIN, LOW);
  right_stepper_target += steps;
  left_stepper_target += steps;
  stepper_enabled = true;
  
  waitMovement();
}

void turnRightSteps(int steps){
  digitalWrite(RIGHT_DIR_PIN, LOW);
  digitalWrite(LEFT_DIR_PIN, HIGH);
  right_stepper_target += steps;
  left_stepper_target += steps;
  stepper_enabled = true;
  
  waitMovement();
}

bool _wallLeft()
{
  if (moveBuffered == 2)
  {
    while(abs(right_stepper_target - right_stepper_position) >= 340)
      delay(1);
  }
  if (stepper_enabled)
  {
    //Eriralprint("LEFT: ");
    if (sensorAverage[RIGHTFORWARDSENSOR][0] > right_wall_min || sensorAverage[RIGHTFORWARDSENSOR][1] > right_wall_min){
      moveBuffered = 1;
      //Eriralprintln(1);
      return 1;
    }
    else{
      moveBuffered = 1;
      //Eriralprintln(0);
      return 0;
    }
  }
  else
  {
    return !(digitalRead(LEFT_BACK_DIGITAL));
  }
}

bool _wallRight()
{
  if (moveBuffered == 2)
  {
    while(abs(right_stepper_target - right_stepper_position) >= 340)
      delay(1);
  }
  if (stepper_enabled)
  {
    //Eriralprint("RIGHT: ");
    if (sensorAverage[LEFTFORWARDSENSOR][0] > left_wall_min || sensorAverage[LEFTFORWARDSENSOR][1] > left_wall_min){
      moveBuffered = 1;
      //Eriralprintln(1);
      return 1;
    }
    else{
      moveBuffered = 1;
      //Eriralprintln(0);
      return 0;
    }
  }
  else
  {
  return !(digitalRead(RIGHT_BACK_DIGITAL));
  }
}

bool _wallFront()
{
  if (moveBuffered == 2)
  {
    while(abs(right_stepper_target - right_stepper_position) >= 340)
      delay(1);
  }
  if(stepper_enabled)
  {
    if (sensorAverage[2][0] > front_wall_min || sensorAverage[2][1] > front_wall_min)
    {
      moveBuffered = 1;
      return 1;
    }else{
      moveBuffered = 1;
      return 0;
    }
  }
  else
  {
    if (sensorAverage[2][avgSamples] > 350)
      return 1;
    else
      return 0;
  }
}

void Estop()
{
  stepper_enabled = true;
  
  right_stepper_position = right_stepper_target;
  left_stepper_position = left_stepper_target;
  left_speed = 0;
  right_speed = 0;
  compensateRight(0);
  compensateLeft(0);
}

/*void preMoveTwist(){

  // Pre_Move Twist correction
    
  int rightRawSensorCorrect = 0;
  int leftRawSensorCorrect = 0;
  int avgRawSensorCorrect = 0;
  
  int rightSensor = sensorAverage[RIGHTFORWARDSENSOR][avgSamples];
  int leftSensor = sensorAverage[LEFTFORWARDSENSOR][avgSamples];
    
   if (!( ( (leftSensor <= ANGLELEFTVALUE - ALLOWEDRANGE) || (leftSensor >= ANGLELEFTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280 )
           && (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] == 0) ) ) // Check if sensor value is within reliable range
    {
      leftRawSensorCorrect = ANGLERIGHTVALUE - leftSensor;
    } 
   if (!( ( (rightSensor <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (rightSensor >= ANGLERIGHTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280 )
           && (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] == 0) ) ) // Check if sensor value is within reliable range
    {
      rightRawSensorCorrect = ANGLERIGHTVALUE - rightSensor;
    }   
   if (leftRawSensorCorrect != 0 && rightRawSensorCorrect == 0)
      rightRawSensorCorrect = leftRawSensorCorrect;
      
   if (rightRawSensorCorrect != 0 && leftRawSensorCorrect == 0)
      leftRawSensorCorrect = rightRawSensorCorrect;
    
   avgRawSensorCorrect = (rightRawSensorCorrect + leftRawSensorCorrect) / 2;
    
   //if (avgRawSensorCorrect > 0)
       //turnRightSteps(avgRawSensorCorrect * TWISTMOVEMENTMULTIPLIER);
       //Eriralprintln(avgRawSensorCorrect = 1);
   //if (avgRawSensorCorrect < 0)
       //Eriralprintln(avgRawSensorCorrect = 1);
       //turnLeftSteps(-(avgRawSensorCorrect * TWISTMOVEMENTMULTIPLIER));
  // End Pr-Move Twist Correction
}*/

