#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"
#include "MPU6050.h"
#include "MPU6050_getdata.h"

DeviceDriverSet_Motor AppMotor;
Application_xxx Application_ConquerorCarxxx0;
MPU6050_getdata AppMPU6050getdata;


// Constants

const int BASE_SPEED = 150;
const int TURN_SPEED = 55;
const float KP_STRAIGHT = 2.0;
const float ANGLE_TOL = 1.0;

const unsigned long OUT_SEG1_TIME = 6500;
const unsigned long OUT_SEG2_TIME = 6000;
const unsigned long OUT_SEG3_TIME = 6000;

const unsigned long RET_SEG1_TIME = 6500;
const unsigned long RET_SEG2_TIME = 6000;
const unsigned long RET_SEG3_TIME = 6000;
const unsigned long RET_SEG4_TIME = 6000;
const unsigned long RET_SEG5_TIME = 6000;
const unsigned long RET_SEG6_TIME = 6500;

// Global variables

bool runComplete = false;


// SETUP

void setup()
{
  Serial.begin(9600);

  // Initialize motors
  AppMotor.DeviceDriverSet_Motor_Init();

  // Initialize gyro
  AppMPU6050getdata.MPU6050_dveInit();
  delay(2000);
  AppMPU6050getdata.MPU6050_calibration();

  stopRobot();
  delay(1000);
}


// LOOP (MAIN LOGIC)

void loop()
{
  if (runComplete)
  {
    stopRobot();
    return;
  }

  // OUTBOUND PATH 
  driveStraightGyro(OUT_SEG1_TIME);
  turnLeft90();

  driveStraightGyro(OUT_SEG2_TIME);
  turnLeft90();

  driveStraightGyro(OUT_SEG3_TIME);
  turnRight90();

  driveStraightGyro(OUT_SEG2_TIME);
  turnRight90();

   driveStraightGyro(OUT_SEG1_TIME);
  // TURN AROUND 
  uTurn180();

  // RETURN PATH 
  driveStraightGyro(RET_SEG1_TIME);
  turnLeft90();

  driveStraightGyro(RET_SEG4_TIME);
  turnLeft90();

  driveStraightGyro(RET_SEG3_TIME);
  turnRight90();

  driveStraightGyro(RET_SEG5_TIME);
  turnRight90();

   driveStraightGyro(RET_SEG6_TIME);

  stopRobot();

  runComplete = true;
}


// HELPER FUNCTIONS BELOW 

// Gyro 
float getYaw()
{
  float Yaw = 0;
  AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
  return Yaw;
}

float normalizeAngle(float angle)
{
  while (angle > 180.0) angle -= 360.0;
  while (angle < -180.0) angle += 360.0;
  return angle;
}

// Basic motion
void stopRobot()
{
  ApplicationFunctionSet_ConquerorCarMotionControl(stop_it, 0);
  delay(120);
}

void moveForwardBasic(int speedValue)
{
  ApplicationFunctionSet_ConquerorCarMotionControl(Forward, speedValue);
}

void turnRightBasic(int speedValue)
{
  ApplicationFunctionSet_ConquerorCarMotionControl(Right, speedValue);
}

void turnLeftBasic(int speedValue)
{
  ApplicationFunctionSet_ConquerorCarMotionControl(Left, speedValue);
}

// Straight driving (gyro corrected)
void driveStraightGyro(unsigned long durationMs)
{
  float targetYaw = getYaw();
  unsigned long startTime = millis();

  while (millis() - startTime < durationMs)
  {
    float currentYaw = getYaw();
    float error = normalizeAngle(targetYaw - currentYaw);

    if (error > 1.5)
    {
      ApplicationFunctionSet_ConquerorCarMotionControl(Left, BASE_SPEED);
    }
    else if (error < -1.5)
    {
      ApplicationFunctionSet_ConquerorCarMotionControl(Right, BASE_SPEED);
    }
    else
    {
      moveForwardBasic(BASE_SPEED);
    }

    delay(15);
  }

  stopRobot();
}

// Turning (gyro-based)
void rotateByAngle(float targetDelta)
{
  float startYaw = getYaw();
  float targetYaw = normalizeAngle(startYaw + targetDelta);

  if (targetDelta > 0)
  {
    while (true)
    {
      float error = normalizeAngle(targetYaw - getYaw());
      if (abs(error) <= ANGLE_TOL) break;

      turnRightBasic(TURN_SPEED);
      delay(10);
    }
  }
  else
  {
    while (true)
    {
      float error = normalizeAngle(targetYaw - getYaw());
      if (abs(error) <= ANGLE_TOL) break;

      turnLeftBasic(TURN_SPEED);
      delay(10);
    }
  }

  stopRobot();
  delay(150);
}

void turnRight90()
{
  rotateByAngle(90.0);
}

void turnLeft90()
{
  rotateByAngle(-90.0);
}

void uTurn180()
{
  rotateByAngle(180.0);
}

