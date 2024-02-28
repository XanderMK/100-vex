/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       xandermkleiber                                            */
/*    Created:      8/16/2023, 12:18:46 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#define _DEBUG

#include "vex.h"
#include "auton.h" 
#include "manual.h"

// A global instance of competition
vex::competition Competition;

vex::brain Brain;

vex::motor LeftFront(vex::PORT19, true);
vex::motor LeftBack(vex::PORT20, true);

vex::motor RightFront(vex::PORT11);
vex::motor RightBack(vex::PORT12);

vex::motor_group MotorGroupLeft(LeftFront, LeftBack);
vex::motor_group MotorGroupRight(RightFront, RightBack);

vex::motor FirstStageLiftMotor(vex::PORT15, true);
vex::motor SecondStageLiftMotor(vex::PORT14);
vex::motor LiftRatchetMotor(vex::PORT13);

vex::motor LauncherMotor(vex::PORT16);
vex::motor WingMotor(vex::PORT17);

vex::inertial InertialSensor(vex::PORT18);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  FirstStageLiftMotor.resetPosition();
  SecondStageLiftMotor.resetPosition();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  Auton auton(&Brain, &MotorGroupLeft, &MotorGroupRight, &LauncherMotor, &WingMotor, &InertialSensor);
  auton.Run();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  vex::controller Controller;
  Manual manual(&Brain, &Controller, &MotorGroupLeft, &MotorGroupRight, &FirstStageLiftMotor, 
                &SecondStageLiftMotor, &LiftRatchetMotor, &LauncherMotor, &WingMotor);

  while (1) {
    manual.Run();

    wait(20, vex::msec); // Sleep the task for a short amount of time to
                         // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomus function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, vex::msec);
  }
}
