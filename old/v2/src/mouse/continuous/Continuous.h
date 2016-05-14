#pragma once

#include "../IMouseAlgorithm.h"

namespace continuous {

class Continuous : public IMouseAlgorithm {

public:
    std::string mouseFile() const;
    std::string interfaceType() const;
    void solve(int mazeWidth, int mazeHeight, bool isOfficialMaze, char initialDirection, sim::MouseInterface* mouse);

private:
	//Thresholds for left and right sensors detecting side walls
#define hasLeftWall .5
#define hasRightWall .5

	//Seperate speeds for explore and solve (not currently implemented)
	int exploreSpeed = 620;
	int solveSpeed = 620;

	int leftBaseSpeed = exploreSpeed;
	int rightBaseSpeed = exploreSpeed;

	//Setpoint for left and right sensors detecting side walls
	const float rightWallDist = .825;
	const float leftWallDist = .825;

	// PID Constants
#define straightKp 200  //TODO
#define turnKp 16 //TODO
#define Kd 600

	/* Variables for interface between drive code and algorithm */
	volatile char movesBuffer[256];
	char bluetoothBuffer[5];
	volatile bool walls_global[3] = { false, false, false }; // Left, Front, Right
	volatile bool movesReady = false; // Set to true by algorithm, set to false by drive.
	volatile bool movesDoneAndWallsSet = false; // Set to true by drive, set to false by algorithm.
												/* End of variables for interface */

												//Max speed for acceleration
	const int maxSpeed = 1500;

	bool currentMoveDone = false;
	bool firstMove = true;
	bool accelerate = true;
	bool solving = 1;
	int goalSpeed = 0;

	//RX and TX pins for software serial
	const int rxPin = 0;
	const int txPin = 16;


	volatile bool firstCell = true;
	volatile bool afterTurnAround = false;

	//Walls currently on left or right
	volatile bool rightValid = true;
	volatile bool leftValid = true;

	volatile bool haveSensorReading = false;

	//SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

	//Current angle of the robot
	volatile float angle = 0.0;

	//Different move types
	volatile enum {
		NO = 0,
		FORWARD = 1,
		TURN_LEFT = 2,
		TURN_RIGHT = 3,
		TURN_AROUND = 4
	} moveType;

	int rightTicks = 0;
	int leftTicks = 0;
	double leftSensor;
	double rightSensor;
	double rightMiddleValue;
	double leftMiddleValue;
	double leftFront;
	double rightFront;
	int currentRightPWM = 0;
	int currentLeftPWM = 0;
    bool wallRight();
    bool wallFront();
    bool wallLeft();
	void moveForward();
	void forwardCorrection();
	void turnCorrection();
	void pivotTurnRight();
	long long millis();
	void pivotTurnRight90();
	void setSpeed(double left, double right);
	void turnAround();
	void readSensors();
	void gyroTest();
	void curveTurnRight();
	void delay(int ms);
	void correction();
	bool allowOmniscience() const;
    sim::MouseInterface* m_mouse;
};

} // namespace continuous
