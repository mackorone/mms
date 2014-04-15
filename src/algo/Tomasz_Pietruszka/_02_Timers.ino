void leftDig() {
  //leftWallTime = millis();
  //wallLeftSteps = left_stepper_position;
  
  if (stoppingFrontWall == 0 && stoppingSideWall == 0 && correct == 1 && abs(left_stepper_target - left_stepper_position) <= FORWARD_STEPS)
  {
    right_stepper_target = left_stepper_position + SIDESTEPWALLSTOP; // Add number of steps remaining to wall
    left_stepper_target = left_stepper_position + SIDESTEPWALLSTOP;
    stoppingSideWall = 1;
  }
}

void rightDig() {
  //rightWallTime = millis();
  //wallRightSteps = right_stepper_position;
  if (stoppingFrontWall == 0 && stoppingSideWall == 0 && correct == 1 && abs(right_stepper_target - right_stepper_position) <= FORWARD_STEPS)
  {
    right_stepper_target = right_stepper_position + SIDESTEPWALLSTOP; // Add number of steps remaining to wall
    left_stepper_target = right_stepper_position + SIDESTEPWALLSTOP;
    stoppingSideWall = 1;
  }
}

ISR(TIMER1_COMPA_vect) {
  // //Eriralprintln("Postition: "+right_stepper_position);
  long remaining;
  if ((right_stepper_target - right_stepper_position) < 1)
    stepper_enabled = false;
  if (stepper_enabled)
  {
    //digitalWrite(RIGHT_PULSE_PIN, HIGH);
    PORTH = 1;
    delayMicroseconds(2);
    PORTH = 0;
    //digitalWrite(RIGHT_PULSE_PIN, LOW);
    right_stepper_position++;
    remaining = abs(right_stepper_target - right_stepper_position);
    if (remaining > right_speed / table_increment) // be sure there is time to stop
      right_speed += table_increment;
    else
      right_speed -= table_increment;
    if (right_speed > speed_max) // not too fast
      right_speed = speed_max;
    int temp = (right_correction * (left_speed * MOVEMENT_MULTIPLY_CORRECTION));
    if (right_speed + temp < 0) // definitely not too slow
      right_speed = -(temp); // right correction should always be negative
    OCR1A = pgm_read_word_near(accel_table + right_speed + temp);   // pgm_read_word_near(charSet + k)
  }
}

ISR(TIMER3_COMPA_vect) {
  long remaining;
  if ((left_stepper_target - left_stepper_position) < 1)
    stepper_enabled = false;
  if (stepper_enabled)
  {
    PORTJ = 1;
    delayMicroseconds(2);
    PORTJ = 0;
    //digitalWrite(LEFT_PULSE_PIN, LOW);
    left_stepper_position++;
    remaining = abs(left_stepper_target - left_stepper_position);
    if (remaining > left_speed / table_increment) // be sure there is time to stop
      left_speed += table_increment;
    else
      left_speed -= table_increment;
    if (left_speed > speed_max) // not too fast
      left_speed = speed_max;
    int temp = (left_correction * (left_speed * MOVEMENT_MULTIPLY_CORRECTION));
    if (left_speed + temp < 0) // definitely not too slow
      left_speed = -(temp); // left correction should always be negative
    OCR3A = pgm_read_word_near(accel_table + left_speed + temp);
  }
}

ISR(TIMER4_COMPA_vect) {
  sensorAverage[0][sensorIndex] = analogRead(LEFT_FORWARD_ANALOG); // TODO change these to defined
  sensorAverage[1][sensorIndex] = analogRead(RIGHT_FORWARD_ANALOG);
  sensorAverage[2][sensorIndex] = analogRead(FRONT_ANALOG);
  sensorAverage[3][sensorIndex] = analogRead(LEFT_BACK_ANALOG);
  sensorAverage[4][sensorIndex] = analogRead(RIGHT_BACK_DIGITAL);

  if(!digitalRead(5))
  {
    digitalWrite(2, HIGH);
    resetRunRequested = true;
  }
  
  if (sensorIndex == avgSamples)
    sensorIndex = 0;
  else
    sensorIndex += 1;

  sensorAverage[0][avgSamples] = 0;
  sensorAverage[1][avgSamples] = 0;
  sensorAverage[2][avgSamples] = 0;
  sensorAverage[3][avgSamples] = 0;
  sensorAverage[4][avgSamples] = 0;

  for (int i = 0; i < avgSamples; i++)
  {
    sensorAverage[0][avgSamples] += sensorAverage[0][i];
    sensorAverage[1][avgSamples] += sensorAverage[1][i];
    sensorAverage[2][avgSamples] += sensorAverage[2][i];
    sensorAverage[3][avgSamples] += sensorAverage[3][i];
    sensorAverage[4][avgSamples] += sensorAverage[4][i];
  }
  sensorAverage[0][avgSamples] /= avgSamples;
  sensorAverage[1][avgSamples] /= avgSamples;
  sensorAverage[2][avgSamples] /= avgSamples;
  sensorAverage[3][avgSamples] /= avgSamples;
  sensorAverage[4][avgSamples] /= avgSamples;

  if (correct == 1)
  {
    int avgRawSensorCorrect = 0;
    int leftRawSensorCorrect = 0;
    int rightRawSensorCorrect = 0;

    if (sensorAverage[2][avgSamples] >= FRONTSENSORWALLSTOPSTART && stoppingFrontWall == 0) // stopping if wall is present
    {
      right_stepper_target = right_stepper_position + FRONTSTEPSWALLSTOP; // Add number of steps remaining to wall
      left_stepper_target = left_stepper_position + FRONTSTEPSWALLSTOP;
      stoppingFrontWall = 1;
    }

    if ((sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] <= left_stepper_position))
    {
      sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] = 0;
    }
    if ((sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] <= right_stepper_position))
    {
      sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] = 0;
    }


    if ( ((sensorAverage[LEFTFORWARDSENSOR][0] <= ANGLELEFTVALUE - ALLOWEDRANGE) || (sensorAverage[LEFTFORWARDSENSOR][0] >= ANGLELEFTVALUE + ALLOWEDRANGE) || (sensorAverage[LEFTFORWARDSENSOR][1] <= ANGLELEFTVALUE - ALLOWEDRANGE) || (sensorAverage[LEFTFORWARDSENSOR][1] >= ANGLELEFTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280)
         && (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] == 0) ) // Check if sensor value is outside reliable range
    {
      sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] = left_stepper_position + RESETSTEPAMOUNT; // UNRELIABLE SENSOR FOR REST OF MOVE STEP COUNT AT WHICH IS WAS DECLARED UNRELIABLE
      compensateLeft(0);
    }
    if ( ((sensorAverage[RIGHTFORWARDSENSOR][0] <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (sensorAverage[RIGHTFORWARDSENSOR][0] >= ANGLERIGHTVALUE + ALLOWEDRANGE) || (sensorAverage[RIGHTFORWARDSENSOR][1] <= ANGLERIGHTVALUE - ALLOWEDRANGE) || (sensorAverage[RIGHTFORWARDSENSOR][1] >= ANGLERIGHTVALUE + ALLOWEDRANGE) || sensorAverage[2][avgSamples] >= 280)
         && (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] == 0) )
    {
      sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] = right_stepper_position + RESETSTEPAMOUNT; // UNRELIABLE SENSOR FOR REST OF MOVE STEP COUNT AT WHICH IS WAS DECLARED UNRELIABLE
      compensateRight(0);
    }

    int rightSensor = (sensorAverage[RIGHTFORWARDSENSOR][0] + sensorAverage[RIGHTFORWARDSENSOR][1]) / 2;
    int leftSensor = (sensorAverage[LEFTFORWARDSENSOR][0] + sensorAverage[LEFTFORWARDSENSOR][1]) / 2;

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

    if (walls != 0)
    {
      avgRawSensorCorrect = (rightRawSensorCorrect + leftRawSensorCorrect) / 2;
      /*//Eriralprint(avgRawSensorCorrect);
      //Eriralprint("       -         ");
      if (sensorAverage[LEFTFORWARDSENSOR][avgSamples + 1] == 0)
        //Eriralprint(leftSensor);
      else
        //Eriralprint("   ");
      //Eriralprint(",");
      if (sensorAverage[RIGHTFORWARDSENSOR][avgSamples + 1] == 0)  // if Sensor is reliable preform data correction
        //Eriralprintln(rightSensor);
      else
        //Eriralprintln();*/
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
  }
}

//ISR(TIMER4_COMPA_vect){
/*
//Eriralprint("1,");
//Eriralprint(digitalRead(LEFT_BACK_DIGITAL)*300);
//Eriralprint(",");
//Eriralprint(digitalRead(RIGHT_BACK_DIGITAL)*280);
//Eriralprint(",");
//Eriralprint(analogRead(FRONT_ANALOG));
//Eriralprint(",");
//Eriralprint(analogRead(RIGHT_BACK_ANALOG));
//Eriralprint(",");
//Eriralprint(analogRead(LEFT_BACK_ANALOG));
//Eriralprint(",");
//Eriralprint(analogRead(RIGHT_FORWARD_ANALOG));
//Eriralprint(",");
//Eriralprintln(analogRead(LEFT_FORWARD_ANALOG));*/
//}
// FOURTH MEASUREMENT TIMER IS IN CORRECTION FILE *********************************
