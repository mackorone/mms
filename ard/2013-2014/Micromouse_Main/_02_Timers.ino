void leftDig(){
  leftWallTime = millis();
  wallLeftSteps = left_stepper_position;
}

void rightDig(){
  rightWallTime = millis();
  wallRightSteps = right_stepper_position;
}

ISR(TIMER1_COMPA_vect){
 // Serial.println("Postition: "+right_stepper_position);
  long remaining;
  if ((right_stepper_target-right_stepper_position)<1)
    stepper_enabled=false;
  if (stepper_enabled)
  {
    //digitalWrite(RIGHT_PULSE_PIN, HIGH);
    PORTH = 1;
    delayMicroseconds(2);
    PORTH = 0;
    //digitalWrite(RIGHT_PULSE_PIN, LOW);
    right_stepper_position++;
    remaining = abs(right_stepper_target-right_stepper_position);
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

ISR(TIMER3_COMPA_vect){
  long remaining;
  if ((left_stepper_target-left_stepper_position)<1)
    stepper_enabled=false;
  if (stepper_enabled)
  {
    PORTJ = 1;
    delayMicroseconds(2);
    PORTJ = 0;
    //digitalWrite(LEFT_PULSE_PIN, LOW);
    left_stepper_position++;
    remaining = abs(left_stepper_target-left_stepper_position);
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

ISR(TIMER4_COMPA_vect){
	sensorAverage[0][sensorIndex] = analogRead(LEFT_FORWARD_ANALOG); // TODO change these to defined
	sensorAverage[1][sensorIndex] = analogRead(RIGHT_FORWARD_ANALOG);
	sensorAverage[2][sensorIndex] = analogRead(FRONT_ANALOG);
	sensorAverage[3][sensorIndex] = analogRead(LEFT_BACK_ANALOG);
	sensorAverage[4][sensorIndex] = analogRead(RIGHT_BACK_DIGITAL);

	if (sensorIndex == avgSamples)
		sensorIndex = 0;
	else
		sensorIndex += 1;

	sensorAverage[0][avgSamples] = 0;
	sensorAverage[1][avgSamples] = 0;
	sensorAverage[2][avgSamples] = 0;
	sensorAverage[3][avgSamples] = 0;
	sensorAverage[4][avgSamples] = 0;

	for (int i = 0; i<avgSamples; i++)
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

	timestamp = millis();
}

//ISR(TIMER4_COMPA_vect){
  /*
  Serial.print("1,");
  Serial.print(digitalRead(LEFT_BACK_DIGITAL)*300);
  Serial.print(",");
  Serial.print(digitalRead(RIGHT_BACK_DIGITAL)*280);
  Serial.print(",");
  Serial.print(analogRead(FRONT_ANALOG));
  Serial.print(",");
  Serial.print(analogRead(RIGHT_BACK_ANALOG));
  Serial.print(",");
  Serial.print(analogRead(LEFT_BACK_ANALOG));
  Serial.print(",");
  Serial.print(analogRead(RIGHT_FORWARD_ANALOG));
  Serial.print(",");
  Serial.println(analogRead(LEFT_FORWARD_ANALOG));*/
//}
// FOURTH MEASUREMENT TIMER IS IN CORRECTION FILE *********************************
