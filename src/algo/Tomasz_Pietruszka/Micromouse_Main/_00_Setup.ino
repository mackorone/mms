#define LEFT_FORWARD_ANALOG   A9      // PINS
#define RIGHT_FORWARD_ANALOG  A4
#define LEFT_BACK_ANALOG      A13
#define RIGHT_BACK_ANALOG     A4
#define FRONT_ANALOG          A2

#define FRONT_DIGITAL         38
#define LEFT_BACK_DIGITAL     18 
#define RIGHT_BACK_DIGITAL    19 

#define LEFT_PULSE_PIN        15
#define RIGHT_PULSE_PIN       17

#define LEFT_DIR_PIN          45
#define RIGHT_DIR_PIN         11            // END PINS

#define LEFTFORWARDSENSOR 0 // above
#define RIGHTFORWARDSENSOR 1
#define FRONTFORWARDSENSOR 2
#define LEFTBACKSENSOR 3
#define RIGHTBACKSENSOR 4

#define TURN_STEPS              600
#define FORWARD_STEPS           379

#define RESETSTEPAMOUNT            379

#define FRONTSENSORWALLSTOPSTART 320 // Front Sensor value at which to initate wall stop
#define FRONTSTEPSWALLSTOP 125      // Number of steps to take after wall is detected
#define SIDESTEPWALLSTOP 107

#define SIDESTEPWALLSTOP  107

#define TWISTMOVEMENTMULTIPLIER2WALLS  0.0575 // Value to multiply sensor difference by//used to be 0.0625
#define TWISTMOVEMENTMULTIPLIER1WALL  0.0575 // Value to multiply sensor difference by//used to be 0.0525

#define MOVEMENT_MULTIPLY_CORRECTION 0.1725 // DONT TOUCH

//#define ANGLECENTERVALUE 275 // average wall value
#define ANGLELEFTVALUE 298 // average wall value
#define ANGLERIGHTVALUE 268 // average wall value

#define ALLOWEDRANGE 55      // how far sensor can be off by before its deemed unreliable

volatile bool correct = 0;

volatile bool resetRunRequested = 0;

volatile bool stoppingFrontWall = 0; // Method mouse is using to stop in the middle of a cell
volatile bool stoppingSideWall = 0;  // ^^

volatile boolean stepper_enabled = false;

volatile int moveBuffered = 0;
                                 
volatile int right_stepper_target = 0; //How many steps you want to take
volatile int left_stepper_target = 0;
volatile int right_stepper_position=0; //How many steps have been taken
volatile int left_stepper_position=0;

volatile int left_correction = 0;
volatile int right_correction = 0;

volatile int right_speed=0; //used to access timer table
volatile int left_speed=0;

volatile int speed_max=175;  //147; //max table position
volatile int table_increment = 2; // value by which to increment table values by

volatile int bufferedTurn = 0;

const byte avgSamples = 2; // Number of samples to sum up
volatile byte sensorIndex = 0;
volatile int sensorAverage[5][avgSamples+2] = { 0 }; // [sensor][index] +1 - second to last is average. last in dependability for that move
volatile long timestamp = 0;                         // Time Stamp for new data is available

volatile int front_wall_min = 145;                   //sensor value will be less if there is not a wall a cell and a half ahead
volatile int left_wall_min = ANGLELEFTVALUE - 55;    //will be less if there is not a wall
volatile int right_wall_min = ANGLERIGHTVALUE - 55;    //will be less if there is not a wall


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setupPins(){
  
  pinMode(5, INPUT_PULLUP);
  
  pinMode(RIGHT_PULSE_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  
  digitalWrite(RIGHT_PULSE_PIN, LOW);
  digitalWrite(RIGHT_DIR_PIN, LOW);
  
  pinMode(LEFT_PULSE_PIN, OUTPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);
  
  digitalWrite(LEFT_PULSE_PIN, LOW);
  digitalWrite(LEFT_DIR_PIN, LOW);
  
  pinMode(12, OUTPUT); // MS1 right
  digitalWrite(12, HIGH);
  pinMode(51, OUTPUT); // MS2 right
  digitalWrite(51, LOW);
  pinMode(53, OUTPUT); //PFID ?
  digitalWrite(53, HIGH);
  pinMode(50, OUTPUT); // right enable
  digitalWrite(50, LOW);
  pinMode(52, OUTPUT); // right reset
  digitalWrite(52, HIGH);
  
  pinMode(44, OUTPUT); // MS1 left
  digitalWrite(44, HIGH);
  pinMode(48, OUTPUT); // MS2 left
  digitalWrite(48, LOW);
  pinMode(49, OUTPUT); //PFID ?
  digitalWrite(49, HIGH);
  pinMode(47, OUTPUT); // left enable
  digitalWrite(47, LOW);
  pinMode(43, OUTPUT); // left reset
  digitalWrite(43, HIGH);
  
  
  pinMode(2, OUTPUT); // left reset
  
  
pinMode(LEFT_BACK_DIGITAL, INPUT);
pinMode(RIGHT_BACK_DIGITAL, INPUT);
  
}

void setupTimers(){

  cli(); //stop interrupts
  //http://www.instructables.com/id/Arduino-Timer-Interrupts/?ALLSTEPS
                                   
  //RIGHT TIMER///////////////////////////////////////////////////////////////////////////////////////////////////
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  OCR1A = 100;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 64 prescaler (250 kHz)
  TCCR1B |= (1 << CS11); // 
  TCCR1B |= (1 << CS10); 
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  
  //LEFT TIMER///////////////////////////////////////////////////////////////////////////////////////////////////
  TCCR3A = 0;// set entire TCCR1A register to 0
  TCCR3B = 0;// same for TCCR1B
  TCNT3  = 0;//initialize counter value to 0
  OCR3A = 100;
  // turn on CTC mode
  TCCR3B |= (1 << WGM32);
  // Set CS12 and CS31 bits for 64 prescaler (2MHz)
  TCCR3B |= (1 << CS31);// | (1 << CS30); 
  TCCR3B |= (1 << CS30);  
  // enable timer compare interrupt
  TIMSK3 |= (1 << OCIE3A);

  //Measure Timer///////////////////////////////////////////////////////////////////////////////////////////////////
  TCCR4A = 0;// set entire TCCR1A register to 0
  TCCR4B = 0;// same for TCCR1B
  TCNT4  = 0;//initialize counter value to 0
  OCR4A = 10000;
  // turn on CTC mode
  TCCR4B |= (1 << WGM42);
  // Set CS12 and CS31 bits for 64 prescaler (2MHz)
  TCCR4B |= (1 << CS41);// | (1 << CS30); 
  TCCR4B |= (1 << CS40);  
  // enable timer compare interrupt
  TIMSK4 |= (1 << OCIE4A);  
  
  sei();//enable interrupts
}

//********************************************************************************************************************************
