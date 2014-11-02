void _moveForward(int boxes){
  if(boxes == 0)
    return;
  Serial.println("----------------------------------------");  
  //syncMotors();
  Serial.print(left_stepper_position);
  Serial.print("     ");
  Serial.println(right_stepper_position);
  long lastTimeStamp = timestamp; // timestamp determines 

  //while(lastTimeStamp == timestamp){ //more efficient way to make sure new sensor data is in
  //  delay(1); 
  //}
  lastTimeStamp = timestamp;
  
  //syncMotors();
  //preMoveTwist();
  //syncMotors();
  
  //lastTimeStamp = timestamp;
  
  int avgRawSensorCorrect = 0;
  int leftRawSensorCorrect = 0;
  int rightRawSensorCorrect = 0;
  
  int rightSensor = 0;
  int leftSensor = 0;
  
  int right1Sensor = 0;
  int left1Sensor = 0;
  
  int right2Sensor = 0;
  int left2Sensor = 0;
  
  digitalWrite(RIGHT_DIR_PIN, HIGH); 
  digitalWrite(LEFT_DIR_PIN, HIGH);
  
  sensorAverage[LEFTFORWARDSENSOR][avgSamples +1] = 0; // set the sensor as reliable for the movement initially
  sensorAverage[RIGHTFORWARDSENSOR][avgSamples +1] = 0; //
  
  stoppingFrontWall = 0; // Method mouse is using to stop in the middle of a cell
  stoppingSideWall = 0;
  
  wallLeftSteps = 0; // sensor read
  wallRightSteps = 0; // sensor read
  leftWallTime = 0;
  rightWallTime = 0;
  
  delay(80);
  
  syncMotors();
  
  right_stepper_target += FORWARD_STEPS * boxes; //
  left_stepper_target += FORWARD_STEPS * boxes; //
  
  stepper_enabled = true;
  
  right1Sensor = sensorAverage[RIGHTFORWARDSENSOR][0];
    left1Sensor = sensorAverage[LEFTFORWARDSENSOR][0];
    
    right2Sensor = sensorAverage[RIGHTFORWARDSENSOR][1];
    left2Sensor = sensorAverage[LEFTFORWARDSENSOR][1];
  
  if ( ((left1Sensor <= ANGLELEFTVALUE - ALLOWEDRANGE) || (left1Sensor >= ANGLELEFTVALUE + ALLOWEDRANGE) || (left2Sensor <= ANGLELEFTVALUE - ALLOWEDRANGE) || (left2Sensor >= ANGLELEFTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280) 
         && (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] == 0) ) // Check if sensor value is outside reliable range
    {
      sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] = RESETSTEPAMOUNT; // UNRELIABLE SENSOR FOR REST OF MOVE STEP COUNT AT WHICH IS WAS DECLARED UNRELIABLE
      compensateLeft(0);
    }
    if ( ((right1Sensor <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (right1Sensor >= ANGLERIGHTVALUE + ALLOWEDRANGE) || (right2Sensor <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (right2Sensor >= ANGLERIGHTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280)
         && (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] == 0) )
    {
      sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] = RESETSTEPAMOUNT; // UNRELIABLE SENSOR FOR REST OF MOVE STEP COUNT AT WHICH IS WAS DECLARED UNRELIABLE 
      compensateLeft(0);
    }
    
  delay(80);
  //Serial.println("----------------------------------------");
  while(stepper_enabled == true)
  {
    if(sensorAverage[2][avgSamples] >= FRONTSENSORWALLSTOPSTART && stoppingFrontWall == 0) // stopping if wall is present
    {
      right_stepper_target = right_stepper_position + FRONTSTEPSWALLSTOP; // Add number of steps remaining to wall
      left_stepper_target = left_stepper_position + FRONTSTEPSWALLSTOP;
      stoppingFrontWall = 1;
    }
    
    right1Sensor = sensorAverage[RIGHTFORWARDSENSOR][0];
    left1Sensor = sensorAverage[LEFTFORWARDSENSOR][0];
    
    right2Sensor = sensorAverage[RIGHTFORWARDSENSOR][1];
    left2Sensor = sensorAverage[LEFTFORWARDSENSOR][1];   
    
    if ((sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] <= left_stepper_position))
    {
      sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] = 0;
    }
    if ((sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] <= right_stepper_position))
    {
      sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] = 0;
    }
    
    if ( ((left1Sensor <= ANGLELEFTVALUE - ALLOWEDRANGE) || (left1Sensor >= ANGLELEFTVALUE + ALLOWEDRANGE) || (left2Sensor <= ANGLELEFTVALUE - ALLOWEDRANGE) || (left2Sensor >= ANGLELEFTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280) 
         && (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] == 0) ) // Check if sensor value is outside reliable range
    {
      sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] = left_stepper_position + RESETSTEPAMOUNT; // UNRELIABLE SENSOR FOR REST OF MOVE STEP COUNT AT WHICH IS WAS DECLARED UNRELIABLE
      compensateLeft(0);
    }
    if ( ((right1Sensor <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (right1Sensor >= ANGLERIGHTVALUE + ALLOWEDRANGE) || (right2Sensor <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (right2Sensor >= ANGLERIGHTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280)
         && (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] == 0) )
    {
      sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] = right_stepper_position + RESETSTEPAMOUNT; // UNRELIABLE SENSOR FOR REST OF MOVE STEP COUNT AT WHICH IS WAS DECLARED UNRELIABLE 
      compensateRight(0);
    }
    
    rightSensor = (right1Sensor + right2Sensor) / 2;
    leftSensor = (left1Sensor + left2Sensor) / 2;
    
    /*Serial.print(leftSensor);
    Serial.print("      ");
    Serial.println(sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1]);
    Serial.print("                ");
    Serial.print(rightSensor);
    Serial.print("      ");
    Serial.println(sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1]);*/
    int walls = 0;
    
    if (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] == 0)  // if Sensor is reliable preform data correction
    {
      rightRawSensorCorrect = ANGLERIGHTVALUE - rightSensor;
      walls++;
      if (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] != 0)
        leftRawSensorCorrect = rightRawSensorCorrect; // in case left is uneviable the average will still be right
    }
    if (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] == 0)
    {
      walls++;
      leftRawSensorCorrect = leftSensor - ANGLELEFTVALUE;
      if (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] != 0)
        rightRawSensorCorrect = leftRawSensorCorrect;
    }    
    
    //compensateRight(0); // Reset compensate values so the move function does not exit with them
    //compensateLeft(0);  
    
    if(walls != 0)
    {
       avgRawSensorCorrect = (rightRawSensorCorrect + leftRawSensorCorrect) / 2;
       Serial.print(avgRawSensorCorrect);
       Serial.print("       -         ");
       if (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] == 0)
         Serial.print(leftSensor);
       else
         Serial.print("   ");
       Serial.print(",");
       if (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] == 0)  // if Sensor is reliable preform data correction
         Serial.println(rightSensor);
       else
         Serial.println();
       if (avgRawSensorCorrect > 0)
       {
         if (walls = 1)
           compensateRight(-(avgRawSensorCorrect * TWISTMOVEMENTMULTIPLIER1WALL));
         else
           compensateRight(-(avgRawSensorCorrect * TWISTMOVEMENTMULTIPLIER2WALLS));
       }
       if (avgRawSensorCorrect < 0)
       {
         if (walls = 1)
           compensateLeft((avgRawSensorCorrect * TWISTMOVEMENTMULTIPLIER1WALL));
         else
           compensateLeft((avgRawSensorCorrect * TWISTMOVEMENTMULTIPLIER2WALLS));
       }
    }
    while(lastTimeStamp == timestamp)
    {
      if (stoppingFrontWall == 0 && wallLeftSteps != 0 && stoppingSideWall == 0 && abs(left_stepper_target - wallLeftSteps) <= FORWARD_STEPS)
      {
        right_stepper_target = wallLeftSteps + SIDESTEPWALLSTOP; // Add number of steps remaining to wall
        left_stepper_target = wallLeftSteps + SIDESTEPWALLSTOP;
        stoppingSideWall = 1;
      }
      if (stoppingFrontWall == 0 && wallRightSteps != 0 && stoppingSideWall == 0 && abs(right_stepper_target - wallRightSteps) <= FORWARD_STEPS)
      {
        right_stepper_target = wallRightSteps + SIDESTEPWALLSTOP; // Add number of steps remaining to wall
        left_stepper_target = wallRightSteps + SIDESTEPWALLSTOP;
        stoppingSideWall = 1;
      }
      //delay(1); // delay to not hog cpu power
    }
    lastTimeStamp = timestamp;
    if(!(digitalRead(FRONT_DIGITAL)))
    {
       Estop();
       delay(100);
       stoppingFrontWall = 0;
    }
  }    // END MOVEMENT FORWARD
  
  compensateLeft(0);
  compensateRight(0);
  
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
    delay(50);
  }
  delay(80);  
  Serial.println("----------------------------------------");  
}

int _turnLeft(){
  digitalWrite(RIGHT_DIR_PIN, HIGH);
  digitalWrite(LEFT_DIR_PIN, LOW);
  
  
  // turn on 1/8 microstepping
  
  digitalWrite(48, HIGH); //MS 2
  digitalWrite(51, HIGH);  //MS 2
  int temp_speed = speed_max;
  int temp_accel = table_increment;
  table_increment = temp_accel + 6;
  speed_max = 500;
  delay(2); 
  syncMotors();
  right_stepper_target += TURN_STEPS;
  left_stepper_target += TURN_STEPS;
  //delay(1); // let microstepping go into effect
  stepper_enabled = true;
  waitMovement();
  
  //
  table_increment = temp_accel;
  speed_max = temp_speed;
  digitalWrite(48, LOW); //MS 2
  digitalWrite(51, LOW);  //MS 2
  
  delay(2);
}

int _turnRight(){
  digitalWrite(RIGHT_DIR_PIN, LOW);
  digitalWrite(LEFT_DIR_PIN, HIGH);
  
  // turn on 1/8 microstepping
  
  digitalWrite(48, HIGH); //MS 2
  digitalWrite(51, HIGH);  //MS 2
  
  int temp_speed = speed_max;
  int temp_accel = table_increment;
  table_increment = temp_accel + 6;
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
  while(stepper_enabled)
  {
    delay(10); // no sense wasting clock cycles during movement
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
  return !(digitalRead(LEFT_BACK_DIGITAL));
}

bool _wallRight()
{
  return !(digitalRead(RIGHT_BACK_DIGITAL));
}

bool _wallFront()
{
  if (sensorAverage[2][avgSamples] > 350)
    return 1;
  else
    return 0;
}

void Estop()
{
  stepper_enabled = false;
  right_stepper_position = right_stepper_target;
  left_stepper_position = left_stepper_target;
  left_speed = 0;
  right_speed = 0;
  compensateRight(0);
  compensateLeft(0);
}

void preMoveTwist(){

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
    
   if (avgRawSensorCorrect > 0)
       //turnRightSteps(avgRawSensorCorrect * TWISTMOVEMENTMULTIPLIER);
       Serial.println(avgRawSensorCorrect = 1);
   if (avgRawSensorCorrect < 0)
       Serial.println(avgRawSensorCorrect = 1);
       //turnLeftSteps(-(avgRawSensorCorrect * TWISTMOVEMENTMULTIPLIER));
  // End Pr-Move Twist Correction
}

