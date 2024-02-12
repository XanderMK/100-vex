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

vex::motor FrontLeft(vex::PORT2);
vex::motor FrontRight(vex::PORT1, true);
vex::motor BackLeft(vex::PORT10);
vex::motor BackRight(vex::PORT9, true);

vex::motor_group MotorGroupLeft(FrontLeft, BackLeft);
vex::motor_group MotorGroupRight(FrontRight, BackRight);

vex::motor LauncherMotor1(vex::PORT5);
vex::motor LauncherMotor2(vex::PORT8);
vex::motor_group LauncherMotorGroup(LauncherMotor1, LauncherMotor2);

vex::motor LiftMotor(vex::PORT4);
vex::pneumatics ClawPiston(Brain.ThreeWirePort.G);

vex::motor TheMechanism(vex::PORT20);

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
  // nope
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
  Auton auton(&Brain, &MotorGroupLeft, &MotorGroupRight, &LauncherMotorGroup, &LiftMotor, &ClawPiston);
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
  Manual manual(&Brain, &Controller, &MotorGroupLeft, &MotorGroupRight, &LauncherMotorGroup, &LiftMotor, &ClawPiston, &TheMechanism);

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
