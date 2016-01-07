#include "Continuous.h"
#include "CurveProfile.h"
#include <iostream>
#ifdef _WIN32
#   include <Windows.h>
#   define _USE_MATH_DEFINES
#   undef max
#   undef min
#	include <Windows.h>
#endif
#include <iostream>

using namespace std;
#include <cmath>
#include <ctime>
#undef M_PI
#define M_PI 3.14159265358979323846


namespace continuous {

std::string Continuous::mouseFile() const {
    return "megaMouse.xml";
}

std::string Continuous::interfaceType() const {
    return "CONTINUOUS";
}

void Continuous::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {

	m_mouse = mouse;

	while (true) {
		/*if (wallRight()) {
			moveForward();
		}
		else {
			curveTurnRight();
		}*/
		
		if (!wallRight()) {
			//m_mouse->delay(350);
			curveTurnRight();
		}
		while (wallFront()) {
			curveTurnLeft();
		}
		moveForward();
		//correctErrors();
		
	}
	

	//read initial sensor values to determine first cell move
	walls_global[0] = wallLeft();
	walls_global[1] = wallFront();
	walls_global[2] = wallRight();

	haveSensorReading = false;
	movesDoneAndWallsSet = true;

	/***********************************GYRO TEST**************************************************/
	long long start = millis();
	double angle = 0;
	int timeConst = 1; //ms
	setSpeed(25, 25);
	//clock_t start;
	//clock_t elapsed;
	//start = clock();
	while (true) {//void loop
		// ....
		long long elapsed = millis() - start;
		//elapsed = clock() - start;
		if (elapsed >= timeConst) {
			//start = millis();
			//start = clock();
			angle += m_mouse->readGyro() * timeConst / 1000;
			//cout << angle << "\n";
			start = millis();
			//correction();
			if (angle <= -90) {
				setSpeed(0, 0);
				while (true);
			}
		}
	}

}
/***************************************END GYRO TEST**********************************************/


//1ms timer
void Continuous::correction() {
	static int totalForwardCount = 0;
	static int forwardCount = 0;
	static bool in_acceleration = false;
	static char indexInBuffer = 0;
	static bool movedForward = false;
	if (!movesReady) {
		// Hoping we never get here, but maybe the algorithm is slow.
		haveSensorReading = false;
		return;
	}
	readSensors();
	angle += m_mouse->readGyro() * 0.001;
	if (currentMoveDone) {
		movedForward = false;
		if (firstMove) {
			firstMove = false;
		}
		if (forwardCount != 0) {
			forwardCount--;
		}
		else {
			totalForwardCount = 0;
			in_acceleration = false;
		}
		indexInBuffer += 1;
		currentMoveDone = false;
		if (movesBuffer[indexInBuffer] == 0) {
			// Make sure walls_global is set by the time we get here (STILL NEED TO DO IN TURN AROUND).
			movesReady = false;

			//mySerial.print(walls_global[0]);
			//mySerial.print(walls_global[1]);
			//mySerial.println(walls_global[2]);
			movesDoneAndWallsSet = true;
			indexInBuffer = 0;
			haveSensorReading = false;
			return;
		}
	}

	switch (movesBuffer[indexInBuffer]) {
	case 'f':
		if (!in_acceleration) {
			forwardCount = 1;
			int index = indexInBuffer + 1;
			while (movesBuffer[index++] == 'f') {
				forwardCount++;
			}
			totalForwardCount = forwardCount;
			if (solving) {
				goalSpeed = (int)solveSpeed*(float)forwardCount;
				if (goalSpeed > maxSpeed)
					goalSpeed = maxSpeed;
				if (goalSpeed < solveSpeed)
					goalSpeed = solveSpeed;
			}
			else {
				goalSpeed = (int)exploreSpeed * (float)forwardCount;
				if (goalSpeed < exploreSpeed)
					goalSpeed = exploreSpeed;
				if (goalSpeed > maxSpeed)
					goalSpeed = maxSpeed;
			}
			in_acceleration = true;
		}
		else {
			// if((float)totalForwardCount / (float)(forwardCount) == totalForwardCount) {
			if (forwardCount == 2) {
				goalSpeed = 240;
			}
		}
		moveType = FORWARD;

		if (!movedForward) {
			movedForward = true;
			moveForward();
		}
		forwardCorrection();
		break;
	case 'r':
		moveType = TURN_RIGHT;
		if (firstMove) {
			leftTicks = 0;
			rightTicks = 0;
			//rightTurnFirstCell();

			walls_global[0] = wallLeft();
			walls_global[1] = wallFront();
			walls_global[2] = wallRight();
			haveSensorReading = false;
			return;
		}
		turnCorrection();
		break;
	case 'l': // Fall-through"
		moveType = TURN_LEFT;
		turnCorrection();
		break;
	case 'a':
		leftTicks = 0;
		rightTicks = 0;
		turnAround();
		currentMoveDone = true;
		haveSensorReading = false;
		return;
	default:
		moveType = NO;

		// Don't need to do anything here if we're turning around.
	}

	haveSensorReading = false;
}
//
//void Continuous::moveForward() {
//	if (firstCell) {
//		rightTicks = 70;
//		leftTicks = 70;
//		firstCell = false;
//		leftBaseSpeed = 0;
//		rightBaseSpeed = 0;
//		accelerate = true;
//	}
//	else if (afterTurnAround) {
//		leftBaseSpeed = 0;
//		rightBaseSpeed = 0;
//		accelerate = true;
//		rightTicks = 100;
//		leftTicks = 100;
//		afterTurnAround = false;
//	}
//	else {
//		rightTicks = 0;
//		leftTicks = 0;
//	}
//
//	rightValid = wallRight();
//	leftValid = wallLeft();
//	moveType = FORWARD;
//}

void Continuous::turnAround() {
	const int frontLeftStop = 1400;
	const int frontRightStop = 1400;
	bool leftStop = false;
	bool rightStop = false;
	int tickCount = 180;
	int errorP;
	int errorD;
	int totalError;
	bool front;
	leftBaseSpeed = 240;
	rightBaseSpeed = 240;
	moveType = NO;
	if (wallFront()) {
		front = true;
	}
	else {
		afterTurnAround = true;
		front = false;
	}
	if (front) {
		while (rightFront <= frontRightStop || leftFront <= frontLeftStop) {
			if (wallRight() && wallLeft()) {
				errorP = leftSensor - rightSensor - 100; // 100 is the offset between left and right sensor when mouse in the
														 // middle of cell
			}
			else if (wallRight()) {
				const int wallDist = rightWallDist; //Make this bigger to move closer to the wall
													// Only right wall
				errorP = 20 * (angle)-.5 * (rightSensor - wallDist);
				errorD = errorP;
			}
			else if (wallLeft()) {
				const int wallDist = leftWallDist; // Make this bigger to move closer to the wall
												   // Only left wall
												   // errorP = 2 * (leftMiddleValue - leftSensor + 1200) + 100 * (angle - targetAngle);

				errorP = 20 * (angle)+.5 * (leftSensor - wallDist);
				errorD = errorP;
			}
			else {
				errorP = 20 * (angle);
				errorD = errorP;
			}
			//      errorP += 3*(rightFront - leftFront);
			errorD = errorP;
			totalError = straightKp * errorP + Kd * errorD;

			// Calculate PWM based on Error
			currentLeftPWM = leftBaseSpeed + totalError / 124;
			currentRightPWM = rightBaseSpeed - totalError / 124;

			// Update Motor PWM values
			setSpeed(currentLeftPWM, currentRightPWM);
		}
	}

	//Turn Around with no wall in front
	else {
		const int tickValue = 100;
		while ((rightTicks + leftTicks) / 2 < tickValue) {
			if (wallRight() && wallLeft()) {
				errorP = leftSensor - rightSensor - 100; // 100 is the offset between left and right sensor when mouse in the
														 // middle of cell
			}
			else if (wallRight()) {
				// Only right wall
				errorP = 20 * (angle)-.5 * (rightSensor - rightWallDist);
				errorD = errorP;
			}
			else if (wallLeft()) {
				// Only left wall
				// errorP = 2 * (leftMiddleValue - leftSensor + 1200) + 100 * (angle - targetAngle);
				errorP = 20 * (angle)+.5 * (leftSensor - leftWallDist);
				errorD = errorP;
			}
			else {
				errorP = 20 * (angle);
				errorD = errorP;
			}
			errorD = errorP;
			totalError = straightKp * errorP + Kd * errorD;

			// Calculate PWM based on Error
			currentLeftPWM = leftBaseSpeed + totalError / 124;
			currentRightPWM = rightBaseSpeed - totalError / 124;

			// Update Motor PWM values
			setSpeed(currentLeftPWM, currentRightPWM);
		}
	}
	setSpeed(0, 0);
	delay(200);
	leftStop = false;
	rightStop = false;

	pivotTurnRight();

	angle = 0.0;
	delay(200);
	if (front) {
		setSpeed(-150, -150);
		delay(350);
		firstCell = true;
	}
	else {
		delay(200);
		afterTurnAround = true;

	}

}

void Continuous::forwardCorrection() {
	const int oneCellTicks = 327;
	const int noWallRight = 250; // check this value (250)
	const int noWallLeft = 450; // check this value (450)

	const int pegWallBack = 800; // check this value
	const int pegNoWalls = 1000;
	const int pegWallFront = 1000;

	const int wallBackTicks = 232;
	const int noWallTicks = 209;
	const int frontWallTicks = 204;

	// encoder tick value when we check walls a cell ahead
	const int readingTicks = 173; // check this value (163)
								  // encoder tick value when we switch to next cell's values
	const int newSideTicks = 230; // check this value (200)

	static bool nextRightValid;
	static bool nextLeftValid;
	static bool nextCellDecided = false;
	int errorP;
	int errorD;
	int oldErrorP = 0;
	int totalError;
	static int lastTicksL;
	static int lastTicksR;
	static float straightAngle = 0.0;
	static bool endCell = false;
	static bool currentWallLeft = true;
	static bool currentWallRight = true;
	static bool ticksDecided = false;
	static int count = 0;

	if (accelerate) {

		if (leftBaseSpeed == 0) {
			leftBaseSpeed = 30;
			rightBaseSpeed = 30;
		}
		count++;

		if (count % 1 == 0) {
			if (leftBaseSpeed < goalSpeed) {
				leftBaseSpeed++;
				rightBaseSpeed++;
			}
			else if (leftBaseSpeed > goalSpeed) {
				leftBaseSpeed = goalSpeed;
				rightBaseSpeed = goalSpeed;
			}
			else {
				count = 0;
			}
		}

	}
	// Next Cell Wall Detection
	if ((rightTicks + leftTicks) / 2 >= readingTicks && !nextCellDecided) {

		nextRightValid = rightMiddleValue > noWallRight;
		nextLeftValid = leftMiddleValue > noWallLeft;
		nextCellDecided = true;
		if (leftValid != nextLeftValid) {
			leftValid = false;
		}
		if (rightValid != nextRightValid) {
			rightValid = false;
		}
	}

	if ((rightTicks + leftTicks) / 2 >= newSideTicks) {
		leftValid = nextLeftValid;
		rightValid = nextRightValid;
	}

	if (leftValid && rightValid) {
		angle = 0.0;

		// Has both wall, so error correct with both (working, just need to adjust PD constants when final mouse is built)
		errorP = leftSensor - rightSensor - 100; // 100 is the offset between left and right sensor when mouse in the
												 // middle of cell
		errorD = errorP - oldErrorP;
		//        getGres();
		//        gz = (float)readGyroData() * gRes - gyroBias[2];
		//        straightAngle += 2 * (gz) * 0.001;
	}
	else if (leftValid) {

		// Only left wall
		// errorP = 2 * (leftMiddleValue - leftSensor + 1200) + 100 * (angle - targetAngle);
		errorP = 20 * (angle - 5) + .5 * (leftSensor - leftWallDist);
		errorD = errorP - oldErrorP;
	}
	else if (rightValid) {
		// Only right wall
		errorP = 20 * (angle)-.5 * (rightSensor - rightWallDist);
		errorD = errorP - oldErrorP;
	}
	else {
		static int targetAngle = 0;
		// No walls, use gyro to correct
		//    if (!wallFront) {
		//      errorP = -20 * (leftFront - rightFront) + 20*angle;
		//    }
		//    else {
		//    if (rightSensor > 1800 && !currentWallRight) {
		//      targetAngle+=2;
		//    }
		//    if (leftSensor > 1800 && !currentWallLeft) {
		//      targetAngle-=2;
		//    }

		errorP = 20 * (angle - targetAngle);

		//    }
		errorD = errorP - oldErrorP;
	}

	//Peg Correction
#if 0
	// No walls in next cell or current cell
	if (!currentWallRight && !nextRightValid && !ticksDecided) {
		if (rightSensor >= pegNoWalls) {
			leftTicks = noWallTicks;
			rightTicks = noWallTicks;
			ticksDecided = true;
		}
	}

	// No walls in next cell, walls in current cell
	else if (!currentWallRight && !nextRightValid && !ticksDecided) {
		if (rightSensor <= pegWallBack) {
			leftTicks = wallBackTicks;
			rightTicks = wallBackTicks;
			ticksDecided = true;
		}
	}

	// No walls in current cell, wall in next cell
	else if (!currentWallRight && nextRightValid && !ticksDecided) {
		if (rightSensor >= pegWallFront) {
			leftTicks = frontWallTicks;
			rightTicks = frontWallTicks;
			ticksDecided = true;
		}
	}

	// No walls in next cell or current cell
	if (!currentWallLeft && !nextLeftValid && !ticksDecided) {
		if (leftSensor >= pegNoWalls) {
			leftTicks = noWallTicks;
			rightTicks = noWallTicks;
			ticksDecided = true;
		}
	}

	// No walls in next cell, walls in current cell
	else if (currentWallLeft && !nextLeftValid && !ticksDecided) {
		if (leftSensor <= pegWallBack) {
			leftTicks = wallBackTicks;
			rightTicks = wallBackTicks;
			ticksDecided = true;
		}
	}

	// No walls in current cell, wall in next cell
	else if (!currentWallLeft && nextLeftValid && !ticksDecided) {
		if (leftSensor >= pegWallFront) {
			leftTicks = frontWallTicks;
			rightTicks = frontWallTicks;
			ticksDecided = true;
		}
	}
#endif

	if ((rightTicks + leftTicks) / 2 >= oneCellTicks) {
		endCell = true;
	}
	if (endCell) {
		currentWallLeft = nextLeftValid;
		currentWallRight = nextRightValid;
		oldErrorP = 0;
		ticksDecided = false;
		walls_global[0] = wallLeft();
		walls_global[1] = wallFront();
		walls_global[2] = wallRight();
		currentMoveDone = true;
		//        needMove = true;
		nextCellDecided = false;
		moveType = NO;
		endCell = false;
	}


	totalError = straightKp * errorP + Kd * errorD;
	oldErrorP = errorP;

	// Calculate PWM based on Error
	currentLeftPWM = leftBaseSpeed + totalError / 124;
	currentRightPWM = rightBaseSpeed - totalError / 124;

	// Update Motor PWM values
	setSpeed(currentLeftPWM, currentRightPWM);
}

//Curve Turn
void Continuous::turnCorrection() {
	int errorP;
	int errorD = 0;
	int oldErrorP = 0;
	int totalError;
	static float targetAngle;
	static int i = 0;
	static bool turn = false;
	static bool straight = 0;
	const int targetTicks = 140;
	const int frontOffset = 0; // difference between left and right front sensors when lined up with the wall

	if (!straight) {
		if (turn) {
			//targetAngle = curve2[i];	TODO

			//Smaller value = overturn
			if (moveType == TURN_RIGHT) {
				errorP = 100 * (angle + targetAngle);
			}
			else {
				errorP = 100 * (angle - targetAngle);
			}


			errorD = errorP - oldErrorP;

			rightTicks = 0;
			leftTicks = 0;
			totalError = turnKp * errorP + Kd * errorD;
			oldErrorP = errorP;

			// Calculate PWM based on Error
			currentLeftPWM = leftBaseSpeed + totalError / 124;
			currentRightPWM = rightBaseSpeed - totalError / 124;

			// Update Motor PWM values
			setSpeed(currentLeftPWM, currentRightPWM);
			++i;
		}
		//end if (turn)
		else {
			const int frontStop = 650;
			if (wallFront()) {
				if (wallLeft()) {
					// Only left wall
					// errorP = 2 * (leftMiddleValue - leftSensor + 1200) + 100 * (angle - targetAngle);
					//                    errorP = 20 * (angle - targetAngle) + .5 * (leftSensor - wallDist);
					errorP = .5 * (leftSensor - leftWallDist) + 3 * (rightFront - leftFront - frontOffset);
					//            errorP = .5 * (leftSensor - leftWallDist);
					//                    errorP = 3 * (rightFront - leftFront - frontOffset);
					errorD = errorP - oldErrorP;
				}
				else if (wallRight()) {
					// Only right wall
					//                    errorP = 20 * (angle - targetAngle) - .5 * (rightSensor - wallDist);
					//                    errorP = 3 * (rightFront - leftFront - frontOffset);
					errorP = -.5 * (rightSensor - rightWallDist) + 3 * (rightFront - leftFront - frontOffset);
					errorD = errorP - oldErrorP;
				}
				else {
					errorP = 3 * (rightFront - leftFront - frontOffset);
					//                    errorP = 20 * (angle - targetAngle);
					errorD = errorP - oldErrorP;
				}
				//                getGres();
				//                gz = (float)readGyroData() * gRes - gyroBias[2];
				//                angle += 1 * (gz) * 0.001;
				errorP = 3 * (rightFront - leftFront - frontOffset);

				errorD = errorP - oldErrorP;

				totalError = straightKp * errorP + Kd * errorD;
				oldErrorP = errorP;

				// Calculate PWM based on Error
				currentLeftPWM = leftBaseSpeed + totalError / 124;
				currentRightPWM = rightBaseSpeed - totalError / 124;

				// Update Motor PWM values
				setSpeed(currentLeftPWM, currentRightPWM);
				if ((leftFront + rightFront) / 2 >= frontStop) {
					turn = true;
					i = 30;
					rightBaseSpeed = 240;
					angle = 0;
				}
			}
			else {
				turn = true;
				rightBaseSpeed = 240;
				angle = 0;
			}
		}
	}
	//end if (!straight)
	else {
		const int wallFrontValue = 300;
		//        if (rightFront > wallFrontValue && leftFront > wallFrontValue) {
		//            errorP = 3 * (rightFront - leftFront - frontOffset);
		//        }
		if (wallFront()) {
			if (wallLeft()) {
				// Only left wall

				//                    errorP = 20 * (angle - targetAngle) + .5 * (leftSensor - wallDist);
				errorP = .5 * (leftSensor - leftWallDist) + 3 * (rightFront - leftFront - frontOffset);
				//                    errorP = 3 * (rightFront - leftFront - frontOffset);
				errorD = errorP - oldErrorP;
			}
			else if (wallRight()) {
				// Only right wall
				//                    errorP = 20 * (angle - targetAngle) - .5 * (rightSensor - wallDist);
				//                    errorP = 3 * (rightFront - leftFront - frontOffset);
				errorP = -.5 * (rightSensor - rightWallDist) + 3 * (rightFront - leftFront - frontOffset);
				errorD = errorP - oldErrorP;
			}
			else {

				errorP = 3 * (rightFront - leftFront - frontOffset);
				//                    errorP = 20 * (angle - targetAngle);
				errorD = errorP - oldErrorP;
			}
		}
		else {
			errorP = 20 * (angle - targetAngle);
		}

		totalError = straightKp * errorP + Kd * errorD;
		oldErrorP = errorP;

		// Calculate PWM based on Error
		currentLeftPWM = leftBaseSpeed + totalError / 124;
		currentRightPWM = rightBaseSpeed - totalError / 124;

		// Update Motor PWM values
		setSpeed(currentLeftPWM, currentRightPWM);
	}

	/*if (i >= curve2Time && !straight) { TODO
		straight = true;

		leftValid = false;
		rightValid = false;

		if (moveType == TURN_RIGHT) {
			targetAngle = -110;
		}
		else {
			targetAngle = 110;
		}
	}*/

	//Detects when turn is done
	if (straight) {
		if (wallFront()) {
			const int frontStop = 500;
			if ((leftFront + rightFront) / 2 >= frontStop) {
				i = 0;
				angle = 0.0;
				oldErrorP = 0;
				rightTicks = 0;
				leftTicks = 0;
				moveType = NO;
				walls_global[0] = wallLeft();
				walls_global[1] = wallFront();
				walls_global[2] = wallRight();
				currentMoveDone = true;
				//                needMove = true;
				straight = false;
				turn = false;
			}
		}
		else {
			if ((rightTicks + leftTicks) / 2 >= targetTicks) {
				i = 0;
				angle = 0.0;
				oldErrorP = 0;
				rightTicks = 0;
				leftTicks = 0;
				moveType = NO;
				walls_global[0] = wallLeft();
				walls_global[1] = wallFront();
				walls_global[2] = wallRight();
				currentMoveDone = true;
				//                needMove = true;
				straight = false;
				turn = false;
			}
		}
	}
}

void Continuous::pivotTurnRight() {
	int errorP;
	int errorD;
	int totalError;
	int oldErrorP;
	int tickCount = 190;
	// Gyro calibrated for each speed or turning is not accurate
	float degreesTraveled = 0;
	const int turnSpeed = 400;
	const float targetDegrees = 137;
	float initialZ;

	//initialZ = gz; // May not be necessary
	//count = millis();
	setSpeed(turnSpeed, -turnSpeed);
	//while (degreesTraveled >= -targetDegrees) {//FIX
	//	degreesTraveled += (gz - 0) * 0.001;
	//	//count = millis();
	//	delay(1);
	//}

	// Needs to deccelerate for the motors to stop correctly
	delay(200);
}

void Continuous::pivotTurnRight90() {
	int errorP;
	int errorD;
	int totalError;
	int oldErrorP;
	int tickCount = 190;
	// Gyro calibrated for each speed or turning is not accurate
	float degreesTraveled = 0;
	const int turnSpeed = 450;
	const float targetDegrees = 76;
	// const int turnSpeed = 45;
	// const int targetDegrees = 85.5
	// const int turnSpeed = 40;
	// const int targetDegrees = 86
	float initialZ;
	//    rightTicks = 0;
	//    leftTicks = 0;
	//    delay(200);

	//    while (rightTicks > -90 || leftTicks < 90) {
	//    }

	//initialZ = gz; // May not be necessary
	//count = millis();
	setSpeed(turnSpeed, -turnSpeed);
	while (degreesTraveled >= -targetDegrees) {
		//uint32_t deltat = millis() - count;
		/*if (deltat > 1) {
			degreesTraveled += 2 * (gz - 0) * 0.001;
			count = millis();
		}*/
	}
}

// If mouse starts facing a wall, this turns in the first cell to face the opening
//void rightTurnFirstCell() {
//	const int frontLeftStop = 1900;
//	const int frontRightStop = 1900;
//	bool leftStop = false;
//	bool rightStop = false;
//	int tickCount = 180;
//	int errorP;
//	int errorD;
//	int totalError;
//	bool front;
//	leftBaseSpeed = exploreSpeed;
//	rightBaseSpeed = exploreSpeed;
//	moveType = NO;
//
//	//Turn Around with no wall in front
//
//	const int tickValue = 50;
//	while (leftFront < frontLeftStop && rightFront < frontRightStop) {
//		// Only left wall
//		// errorP = 2 * (leftMiddleValue - leftSensor + 1200) + 100 * (angle - targetAngle);
//		getGres();
//		gz = (float)readGyroData() * gRes - gyroBias[2];
//		angle += 2 * (gz)* 0.001;
//		errorP = 20 * (angle)+.5 * (leftSensor - leftWallDist);
//		errorD = errorP;
//
//		errorD = errorP;
//		totalError = straightKp * errorP + Kd * errorD;
//
//		// Calculate PWM based on Error
//		currentLeftPWM = leftBaseSpeed + totalError / 124;
//		currentRightPWM = rightBaseSpeed - totalError / 124;
//
//		// Update Motor PWM values
//		setLeftPWM(currentLeftPWM);
//		setRightPWM(currentRightPWM);
//
//		//TODO (this is a hack and shouldn't be here, but it makes it work)
//		haveSensorReading = false;
//		while (!haveSensorReading) {
//			readSensors();
//			delayMicroseconds(80);
//		}
//
//	}
//	setRightPWM(0);
//	setLeftPWM(0);
//	delay(200);
//
//	pivotTurnRight90();
//
//	angle = 0.0;
//	delay(200);
//	setLeftPWM(-150);
//	setRightPWM(-150);
//	delay(350);
//	for (int i = -150; i < 0; ++i) {
//		setLeftPWM(i);
//		setRightPWM(i);
//	}
//	delay(200);
//	firstCell = true;
//	movesBuffer[0] = 'f';
//
//}

void Continuous::wallFollow() {
	while (!movesDoneAndWallsSet) {
	}
	bool walls[3];
	walls[0] = walls_global[0];
	walls[1] = walls_global[1];
	walls[2] = walls_global[2];
	movesDoneAndWallsSet = false;

	if (!walls[0]) {
		movesBuffer[0] = 'l';
		movesBuffer[1] = 0;
	}
	else if (!walls[1]) {
		movesBuffer[0] = 'f';
		movesBuffer[1] = 0;
	}
	else if (!walls[2]) {
		movesBuffer[0] = 'r';
		movesBuffer[1] = 0;
	}
	else {
		movesBuffer[0] = 'a';
		movesBuffer[1] = 'f';
		movesBuffer[2] = 0;
	}


	movesReady = true;
}

bool Continuous::wallRight() {
    return m_mouse->readSensor("right-side") > 0.5;
}

bool Continuous::wallLeft() {
    return m_mouse->readSensor("left-side") > 0.5;
}

bool Continuous::wallFront() {
    return m_mouse->readSensor("right-front") > 0.9;
}

void Continuous::turnRight() {
  // // m_mouse->setWheelSpeed("left-front", 5*M_PI);
  ////  m_mouse->setWheelSpeed("right-front", 5*M_PI);
  //  delay(350);
  //  m_mouse->setWheelSpeed("left-lower", -10*M_PI);
  //  m_mouse->setWheelSpeed("right-lower", -10*M_PI);
  //  m_mouse->delay(350);
  //  m_mouse->setWheelSpeed("left-lower", 0);  
  //  m_mouse->setWheelSpeed("right-lower", 0);  

	long long start = millis();
	double angle = 0;
	int timeConst = 1; //ms
	setSpeed(-25, -25);
	while (true) {//void loop
		long long elapsed = millis() - start;
		if (elapsed >= timeConst) {
			angle += readGyro() * timeConst / 1000;
			start = millis();
			if (angle <= -90) {
				setSpeed(0, 0);
				break;
			}
		}
	}
}

void Continuous::turnLeft() {
    m_mouse->setWheelSpeed("left-lower", 5*M_PI);
    m_mouse->setWheelSpeed("right-lower", 5*M_PI);
    m_mouse->delay(290);
    m_mouse->setWheelSpeed("left-lower", -10*M_PI);
    m_mouse->setWheelSpeed("right-lower", 10*M_PI);
    m_mouse->delay(100);
    m_mouse->setWheelSpeed("left-lower", 0);  
    m_mouse->setWheelSpeed("right-lower", 0);  
}

void Continuous::simpleTurnAround() { 

	long long start = millis();
	double angle = 0;
	int timeConst = 1; //ms
	setSpeed(-25, -25);
	while (true) {//void loop
		long long elapsed = millis() - start;
		if (elapsed >= timeConst) {
			angle += readGyro() * timeConst / 1000;
			start = millis();
			if (angle <= -180) {
				break;
			}
		}
	}
}

void Continuous::curveTurnRight() {
	double error;
	double totalError;
	int Kp = 2;
	double targetAngle;
	long long start = millis();
	double angle = 0;
	int timeConst = 3; //ms
	int i = 0;
	while (true) {//void loop
		long long elapsed = millis() - start;
		if (elapsed >= timeConst) {
			targetAngle = -curve[i];
			angle += readGyro() * timeConst / 1000;
			error = angle - targetAngle;
			start = millis();
			totalError = Kp * error;
			//m_mouse->setSpeed(-(10*M_PI + totalError), 10*M_PI - totalError);
			m_mouse->setWheelSpeed("left-lower", -(25 * M_PI + totalError));
			m_mouse->setWheelSpeed("right-lower", 25 * M_PI - totalError);
			if (angle <= -90 || i >= curveTime-1) {
				break;
			}
			if (i < curveTime) {
				i++;
			}
			
		}
	}
}

void Continuous::curveTurnLeft() {
	double error;
	double totalError;
	int Kp = 2;
	double targetAngle;
	long long start = millis();
	double angle = 0;
	int timeConst = 3; //ms
	int i = 0;
	while (true) {//void loop
		long long elapsed = millis() - start;
		if (elapsed >= timeConst) {
			targetAngle = curve[i];
			angle += readGyro() * timeConst / 1000;
			error = angle - targetAngle;
			start = millis();
			totalError = Kp * error;
			//m_mouse->setSpeed(-(10*M_PI + totalError), 10*M_PI - totalError);
			m_mouse->setWheelSpeed("left-lower", -(25 * M_PI + totalError));
			m_mouse->setWheelSpeed("right-lower", 25 * M_PI - totalError);
			if (angle >= 90 || i >= curveTime - 1) {
				break;
			}
			if (i < curveTime) {
				i++;
			}
			
		}
	}
}

void Continuous::moveForward() {
    //m_mouse->setWheelSpeeds(-10*M_PI, 10*M_PI);
    //m_mouse->delay(280);
int Kp;
    double error;
    double totalError;
    if (wallRight() && wallLeft()) {
        //error = m_mouse->readSensor("right-side") - m_mouse->readSensor("left-side");
		error = m_mouse->readSensor("left-side") - m_mouse->readSensor("right-side");
        Kp = 20;
    }

    else if (wallRight()) {
        error = .5 * (0.776 - m_mouse->readSensor("right-side"));
	Kp = 5;
    }
    
    else if (wallLeft()) {
        error = .5 * (m_mouse->readSensor("left-side") - 0.825);//.776
        Kp = 20;
    }

    else {
        error = 0;
    }
    totalError = Kp * error;
    //m_mouse->setSpeed(-(10*M_PI + totalError), 10*M_PI - totalError);
	m_mouse->setWheelSpeed("left-lower", -(25 * M_PI + totalError));
	m_mouse->setWheelSpeed("right-lower", 25 * M_PI - totalError);
    //m_mouse->delay(1);
    //std::cout << m_mouse->readSensor("left") << std::endl;
    //std::cout << m_mouse->readSensor("leftMiddle") << std::endl;
    //std::cout << std::endl;
    //m_mouse->setWheelSpeeds(0, 0);
    //m_mouse->delay(0);
}

void Continuous::setSpeed(double left, double right) {
	m_mouse->setWheelSpeed("left-lower", left);
	m_mouse->setWheelSpeed("right-lower", right);
}

void Continuous::delay(int ms) {
	m_mouse->delay(ms);
}

void Continuous::readSensors() {

}

float Continuous::readGyro() {
	return .7 * m_mouse->readGyro();
}


long long Continuous::millis() {
#ifdef __WIN32 // TODO: KYLE
	//http://gamedev.stackexchange.com/questions/26759/best-way-to-get-elapsed-time-in-miliseconds-in-windows
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount();
	}
#else
    return 0;
#endif
}

} // namespace continuous
