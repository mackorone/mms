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

	double startAngle = 90;
	int forwardOffset = 840;

	void Continuous::solve(int mazeWidth, int mazeHeight, bool isOfficialMaze, char initialDirection, sim::MouseInterface* mouse) {

		m_mouse = mouse;
		//read initial sensor values to determine first cell move
		walls_global[0] = wallLeft();
		walls_global[1] = wallFront();
		walls_global[2] = wallRight();

		haveSensorReading = false;
		movesDoneAndWallsSet = true;

		//First cell: 840 (360 tpr)
		//Once Cell: 1680
	/*cout << m_mouse->readSensor("right-side") << "\n";
	cout << m_mouse->readSensor("right-middle") << "\n";
	moveForward(840);
	while (true) {
		if (!wallRight()) {
			curveTurnRight();
		}
		else {
			while (wallFront()) {
				curveTurnLeft();
			}
			moveForward(1680);
		}
	}*/

		long long start = millis();
		int timeConst = 1; //ms
		while (true) {//void loop
			long long elapsed = millis() - start;
			if (elapsed >= timeConst) {
				if (movesDoneAndWallsSet) {
					/*while (true) {
						setSpeed(0, 0);
						delay(100);
						break;
					}*/
					bool walls[3];
					walls[0] = walls_global[0];
					walls[1] = walls_global[1];
					walls[2] = walls_global[2];
					movesDoneAndWallsSet = false;

					/***************Acceleration test (longpath.maz, works up until the second turn)******************/
						/*movesBuffer[0] = 'f';
						movesBuffer[1] = 'f';
						movesBuffer[2] = 'f';
						movesBuffer[3] = 'f';
						movesBuffer[4] = 'f';
						movesBuffer[5] = 'f';
						movesBuffer[6] = 'f';
						movesBuffer[7] = 'f';
						movesBuffer[8] = 'f';
						movesBuffer[9] = 'f';
						movesBuffer[10] = 'f';
						movesBuffer[11] = 'f';
						movesBuffer[12] = 'f';
						movesBuffer[13] = 'f';
						movesBuffer[14] = 'f';
						movesBuffer[15] = 'r';*/
						/***********************************************/

					/***************Right Wall Follow******************/

					if (!walls[2]) {
						cout << "right\n";
						movesBuffer[0] = 'r';
						movesBuffer[1] = 0;
					}
					else if (!walls[1]) {
						cout << "front\n";
						cout << leftBaseSpeed << "\n";
						movesBuffer[0] = 'f';
						movesBuffer[1] = 0;
					}
					else if (!walls[0]) {
						cout << "left\n";
						movesBuffer[0] = 'l';
						movesBuffer[1] = 0;
					}
					else {
						cout << "around\n";
						movesBuffer[0] = 'a';
						movesBuffer[1] = 'f';
						movesBuffer[2] = 0;
					}

					/***********************************************/
					movesReady = true;
				}
				correction();
				//cout << leftBaseSpeed << "\n";
				start = millis();
			}
		}

		/*int start = m_mouse->millis();
		for (int i = 0; i < 1000000; i++) {
			m_mouse->readSensor("left-front");
		}
		int end = m_mouse->millis();
		m_mouse->info(std::to_string(end - start));*/

	}


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
		angle = m_mouse->readGyro();
		/*if ((startAngle >= 0 && startAngle < 45) || (startAngle > 315 && startAngle <= 350)) {
		offsetAngle = 0;
		}
		else if (startAngle > 45 && startAngle < 135) {
		offsetAngle = 90;
		}
		else if (startAngle > 135 && startAngle < 225) {
		offsetAngle = 180;
		}
		else {
		offsetAngle = 270;
		}*/
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
				//if((float)totalForwardCount / (float)(forwardCount) == totalForwardCount) {
				if (forwardCount == 2 + (int)((totalForwardCount + (5 - 1)) / 5)) {
					goalSpeed = 620;
				}
			}
			moveType = FORWARD;

			if (!movedForward) {
				movedForward = true;
				moveForward();	//TODO
			}
			forwardCorrection();
			break;
		case 'r':
			moveType = TURN_RIGHT;
			if (firstMove) {
				leftTicks = 0;
				rightTicks = 0;
				m_mouse->resetWheelEncoder("left-lower");
				m_mouse->resetWheelEncoder("right-lower");
				//rightTurnFirstCell();//TODO

				walls_global[0] = wallLeft();
				walls_global[1] = wallFront();
				walls_global[2] = wallRight();
				haveSensorReading = false;
				return;
			}
			curveTurnRight();//TODO should be turnCorrection()
			break;
		case 'l': // Fall-through"
			moveType = TURN_LEFT;
			curveTurnRight();
			break;
		case 'a':
			leftTicks = 0;
			rightTicks = 0;
			m_mouse->resetWheelEncoder("left-lower");
			m_mouse->resetWheelEncoder("right-lower");
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

	void Continuous::moveForward() {
		if (firstCell) {
			/*rightTicks = 840;
			leftTicks = 840;*/
			forwardOffset = 840;
			firstCell = false;
			leftBaseSpeed = 0;
			rightBaseSpeed = 0;
			accelerate = true;
		}
		else if (afterTurnAround) {
			leftBaseSpeed = 0;
			rightBaseSpeed = 0;
			accelerate = true;
			/*rightTicks = 100;
			leftTicks = 100;*/
			forwardOffset = 840;
			afterTurnAround = false;
		}
		else {
			rightTicks = 0;
			leftTicks = 0;
			forwardOffset = 0;
			m_mouse->resetWheelEncoder("left-lower");
			m_mouse->resetWheelEncoder("right-lower");
		}

		rightValid = wallRight();
		leftValid = wallLeft();
		moveType = FORWARD;
	}

	void Continuous::turnAround() {
		const double frontLeftStop = .8;
		const double frontRightStop = .8;
		bool leftStop = false;
		bool rightStop = false;
		int tickCount = 180;
		int errorP;
		int errorD;
		int totalError;
		bool front;
		leftBaseSpeed = 620;
		rightBaseSpeed = 620;
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
				readSensors();
				//cout << rightFront << " " << leftFront << "\n";
				if (wallRight() && wallLeft()) {
					errorP = leftSensor - rightSensor; // 100 is the offset between left and right sensor when mouse in the
															 // middle of cell
				}
				else if (wallRight()) {
					const int wallDist = rightWallDist; //Make this bigger to move closer to the wall
														// Only right wall
					errorP = 20 * (angle)-.5 * (rightSensor - wallDist);
					errorP = 0;//TODO
					errorD = errorP;
				}
				else if (wallLeft()) {
					const int wallDist = leftWallDist; // Make this bigger to move closer to the wall
													   // Only left wall
													   // errorP = 2 * (leftMiddleValue - leftSensor + 1200) + 100 * (angle - targetAngle);

					errorP = 20 * (angle)+.5 * (leftSensor - wallDist);
					errorP = 0;//TODO
					errorD = errorP;
				}
				else {
					errorP = 20 * (angle);
					errorP = 0; //TODO
					errorD = errorP;
				}
				//      errorP += 3*(rightFront - leftFront);
				errorD = errorP;
				totalError = straightKp * errorP + Kd * errorD;

				// Calculate PWM based on Error

				// Update Motor PWM values
				m_mouse->setWheelSpeed("left-lower", -(leftBaseSpeed + totalError));//TODO variable speed
				m_mouse->setWheelSpeed("right-lower", rightBaseSpeed - totalError);
			}
		}

		//Turn Around with no wall in front - TODO
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
		while (true) {
			setSpeed(0, 0);
			//delay(200);
			leftStop = false;
			rightStop = false;

			pivotTurnRight();

			angle = 0.0;
			if (front) {
				//setSpeed(-150, -150);
				//delay(350);
				firstCell = true;
			}
			else {
				//delay(200);
				afterTurnAround = true;
			}
			m_mouse->resetWheelEncoder("left-lower");
			m_mouse->resetWheelEncoder("right-lower");
			break;
		}
	}

	void Continuous::forwardCorrection() {
		const int oneCellTicks = int(327 * 5.1376);
		const double noWallRight = .5; // check this value (250)
		const double noWallLeft = .5; // check this value (450)

		// encoder tick value when we check walls a cell ahead
		const int readingTicks = int(215*5.1376);
		// encoder tick value when we switch to next cell's values
		const int newSideTicks = int(260*5.1376);

		static bool nextRightValid;
		static bool nextLeftValid;
		static bool nextCellDecided = false;
		double errorP;
		double errorD;
		static int oldErrorP = 0;//TODO (probably should be double)
		double totalError;
		static int lastTicksL;
		static int lastTicksR;
		static float straightAngle = 0.0;
		static bool endCell = false;
		static bool currentWallLeft = true;
		static bool currentWallRight = true;
		static bool ticksDecided = false;
		static int count = 0;
		static double targetDegrees;
		double angle;
		static int i = 0;
		if (i == 0) {
			startAngle = m_mouse->currentRotationDegrees();
			//cout << "START: " << startAngle << "\n";
			//delay(1000);
			if ((startAngle >= 0 && startAngle < 45) || (startAngle > 315 && startAngle <= 360)) {
				targetDegrees = 360;
			}
			else if (startAngle > 45 && startAngle < 135) {
				targetDegrees = 90;
			}
			else if (startAngle > 135 && startAngle < 225) {
				targetDegrees = 180;
			}
			else {
				targetDegrees = 270;
			}
		}
		i++;
		angle = m_mouse->currentRotationDegrees();
		if (targetDegrees == 360) {
			if (angle <= 45) {
				angle += 360;
			}
		}
		angle -= targetDegrees;
		leftTicks = -m_mouse->readWheelEncoder("left-lower") + forwardOffset;
		rightTicks = m_mouse->readWheelEncoder("right-lower") + forwardOffset;
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
					/*leftBaseSpeed = goalSpeed;
					rightBaseSpeed = goalSpeed;*/
					leftBaseSpeed--;
					rightBaseSpeed--;
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
			errorP = leftSensor - rightSensor;
			errorD = errorP - oldErrorP;
			//        getGres();
			//        gz = (float)readGyroData() * gRes - gyroBias[2];
			//        straightAngle += 2 * (gz) * 0.001;
		}
		else if (leftValid) {
			// Only left wall
			// errorP = 2 * (leftMiddleValue - leftSensor + 1200) + 100 * (angle - targetAngle);
			//errorP = 20 * (angle - 5) + .5 * (leftSensor - leftWallDist);//TODO
			errorP = 1 * angle;//TODO
			errorD = errorP - oldErrorP;
		}
		else if (rightValid) {
			// Only right wall
			//errorP = 20 * (angle)-.5 * (rightSensor - rightWallDist);//TODO
			errorP = 1 * angle;//TODO 
			errorD = errorP - oldErrorP;
		}
		else {
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

			//errorP = 20 * (angle - targetAngle);
			errorP = 1 * angle;//TODO Gyro Correction
			//    }
			errorD = errorP - oldErrorP;//TODO
		}

		//Peg Correction TODO
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

		if (((rightTicks + leftTicks) / 2) >= oneCellTicks) {
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
			i = 0;
		}


		totalError = straightKp * errorP + Kd * errorD;
		oldErrorP = errorP;

		// Update Motor PWM values
		m_mouse->setWheelSpeed("left-lower", -(leftBaseSpeed + totalError));//TODO variable speed
		m_mouse->setWheelSpeed("right-lower", rightBaseSpeed - totalError);
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
		const int targetTicks = 170 * 5.1;
		const int frontOffset = 0; // difference between left and right front sensors when lined up with the wall

		if (!straight) {
			if (turn) {
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
				m_mouse->resetWheelEncoder("left-lower");
				m_mouse->resetWheelEncoder("right-lower");
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
						rightBaseSpeed = 620;
						angle = 0;
					}
				}
				else {
					turn = true;
					rightBaseSpeed = 620;
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
					m_mouse->resetWheelEncoder("left-lower");
					m_mouse->resetWheelEncoder("right-lower");
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
					m_mouse->resetWheelEncoder("left-lower");
					m_mouse->resetWheelEncoder("right-lower");
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
		double degreesTraveled = 0;
		const int turnSpeed = 400;
		double targetDegrees = 137;
		double startAngle;
		double degrees;
		startAngle = m_mouse->currentRotationDegrees();
		//cout << "START: " << startAngle << "\n";
		//delay(1000);
		if ((startAngle >= 0 && startAngle < 45) || (startAngle > 315 && startAngle <= 360)) {
			targetDegrees = 180;
		}
		else if (startAngle > 45 && startAngle < 135) {
			targetDegrees = 270;
		}
		else if (startAngle > 135 && startAngle < 225) {
			targetDegrees = 0;
		}
		else {
			targetDegrees = 90;
		}
		if (wallFront()) {
			double wallValue = .958;
			if ((leftFront + rightFront) / 2 >= wallValue) {
				while ((leftFront + rightFront) / 2 >= wallValue) {
					m_mouse->setWheelSpeed("left-lower", 400);
					m_mouse->setWheelSpeed("right-lower", -400);
					readSensors();
				}
			}
			else {
				while ((leftFront + rightFront) / 2 < wallValue) {
					m_mouse->setWheelSpeed("left-lower", -400);
					m_mouse->setWheelSpeed("right-lower", 400);
					readSensors();
				}
			}
		}
		setSpeed(0, 0);
		delay(200);
		setSpeed(-turnSpeed, -turnSpeed);
		degrees = m_mouse->currentRotationDegrees();
		while (abs(degrees - targetDegrees-90) > 1.5) {
			degrees = m_mouse->currentRotationDegrees();
			//cout << abs(degrees - targetDegrees-90) << "\n";
		}
		setSpeed(0, 0);
		// Needs to deccelerate for the motors to stop correctly
		readSensors();
		delay(200);
		if (wallFront()) {
			double wallValue = .958;
			if ((leftFront + rightFront) / 2 >= wallValue) {
				while ((leftFront + rightFront) / 2 >= wallValue) {
					m_mouse->setWheelSpeed("left-lower", 400);
					m_mouse->setWheelSpeed("right-lower", -400);
					readSensors();
				}
			}
			else {
				while ((leftFront + rightFront) / 2 < wallValue) {
					m_mouse->setWheelSpeed("left-lower", -400);
					m_mouse->setWheelSpeed("right-lower", 400);
					readSensors();
				}
			}
		}
		
		setSpeed(-turnSpeed, -turnSpeed);
		degrees = m_mouse->currentRotationDegrees();
		while (abs(degrees - targetDegrees) > 1.5) {
			degrees = m_mouse->currentRotationDegrees();
			//cout << degrees << " " << targetDegrees << "\n";
		}
		setSpeed(0, 0);
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

	bool Continuous::wallRight() {
		return m_mouse->readSensor("right-side") > 0.5;
	}

	bool Continuous::wallLeft() {
		return m_mouse->readSensor("left-side") > 0.5;
	}

	bool Continuous::wallFront() {
		return m_mouse->readSensor("right-front") > 0.4;
	}

	void Continuous::curveTurnRight() {
		double error;
		double totalError;
		double turnSpeed = 1; // smaller is faster (.5, 1, 2 ,3, etc)
		double Kp = 70 / turnSpeed; //35
		double targetAngle;
		//long long start = millis();
		//int timeConst = 2 * turnSpeed; //ms 2
		static int i = 0;
		static int offsetAngle;
		bool continueTurn = true;
		static bool turn = false;
		//cout << "LOOP" << "\n";
		//cout << startAngle << "\n";

		if (i == 0) {
			startAngle = m_mouse->currentRotationDegrees();	//TODO Local?
			if (!wallFront()) {
				turn = true;
			}
			if ((startAngle >= 0 && startAngle < 45) || (startAngle > 315 && startAngle <= 360)) {
				if (moveType == TURN_RIGHT) {//TODO Test all cases
					offsetAngle = 360;
				}
				else {
					offsetAngle = 0;
				}
			}
			else if (startAngle > 45 && startAngle < 135) {
				offsetAngle = 90;
			}
			else if (startAngle > 135 && startAngle < 225) {
				offsetAngle = 180;
			}
			else {
				offsetAngle = 270;
			}
		}
		if (turn == false) {
			targetAngle = offsetAngle;
			cout << "YES";
			if ((rightFront + leftFront) / 2 >= .567) {//.567
				turn = true;
				cout << "YES\n";
				i = 1;
			}
			//turn = true;
		}
		else {
			if (moveType == TURN_RIGHT) {
				targetAngle = offsetAngle - curve[i];
			}
			else {
				targetAngle = offsetAngle + curve[i];
			}
		}
		angle = m_mouse->currentRotationDegrees();
		if (moveType == TURN_RIGHT) {
			if (offsetAngle == 360 && angle < 45) {
				angle += 360;
			}
		}
		else {

			if (offsetAngle == 0 && angle > 315) {
				angle -= 360;
			}
		}

		//cout << targetAngle << "\n";
		//cout << angle << "\n";
		error = angle - targetAngle;
		totalError = Kp * error;
		int leftSpeed = -(620 / turnSpeed + totalError);//310
		int rightSpeed = (620 / turnSpeed - totalError);

		//m_mouse->info(std::string("L: ") + std::to_string(leftSpeed));
		//m_mouse->info(std::string("R: ") + std::to_string(rightSpeed));
		m_mouse->setWheelSpeed("left-lower", leftSpeed);
		m_mouse->setWheelSpeed("right-lower", rightSpeed);
		if (moveType == TURN_RIGHT) {
			continueTurn = i < curveTime - 1 && angle > offsetAngle - 90;
		}
		else {
			continueTurn = i < curveTime - 1 && angle < offsetAngle + 90;
		}
		if (continueTurn) {
			i++;
		}
		else {
			i = 0;
			angle = 0.0;
			//oldErrorP = 0;
			m_mouse->resetWheelEncoder("left-lower");
			m_mouse->resetWheelEncoder("right-lower");
			rightTicks = 0;
			leftTicks = 0;
			moveType = NO;
			walls_global[0] = wallLeft();
			walls_global[1] = wallFront();
			walls_global[2] = wallRight();
			currentMoveDone = true;
			//needMove = true;
			//straight = false;
			turn = false;
		}
	}

	void Continuous::setSpeed(double left, double right) {
		m_mouse->setWheelSpeed("left-lower", left);
		m_mouse->setWheelSpeed("right-lower", right);
	}

	void Continuous::delay(int ms) {
		m_mouse->delay(ms);
	}

	void Continuous::readSensors() {
		//if (!haveSensorReading) {
		leftSensor = m_mouse->readSensor("left-side");
		rightSensor = m_mouse->readSensor("right-side");
		leftFront = m_mouse->readSensor("left-front");
		rightFront = m_mouse->readSensor("right-front");
		rightMiddleValue = m_mouse->readSensor("right-middle");
		leftMiddleValue = m_mouse->readSensor("left-middle");

		//TODO read the rest of them
		//haveSensorReading = true;
	//}
	}

	bool Continuous::allowOmniscience() const {
		return true;
	}

	long long Continuous::millis() {
		return m_mouse->millis();
		/*
		#ifdef _WIN32
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
		*/
	}

	void Continuous::gyroTest() {
		/***********************************GYRO TEST**************************************************/
		double angle = 0;
		long long start = millis();
		//double angle = 0;
		int timeConst = 1; //ms
		setSpeed(200, 200);
		//clock_t start;
		//clock_t elapsed;
		//start = clock();
		//angle = m_mouse->currentRotationDegrees();
		//cout << "start" << "\n";
		while (true) {//void loop
			// ....
			long long elapsed = millis() - start;
			//elapsed = clock() - start;
			if (elapsed >= timeConst) {
				//start = millis();
				//start = clock();
				angle = m_mouse->currentRotationDegrees();
				//cout << angle << "\n";
				//start = millis();
				//correction();
				if (angle >= 270) {
					setSpeed(0, 0);
					readSensors();
					//cout << rightFront << " : " << leftFront << "\n";
					while (true);
				}
			}
		}
	}

} // namespace continuous
