#include "manual.h"
#include "vex.h"
#include <cmath>
#include <iostream>

Manual::Manual(vex::controller* Controller, 
               vex::motor_group* MotorGroupLeft, 
               vex::motor_group* MotorGroupRight, 
               vex::motor* IntakeMotor,
               vex::motor* LiftMotor,
               vex::motor* ClawMotor) {
    this->Controller = Controller;
    this->MotorGroupLeft = MotorGroupLeft;
    this->MotorGroupRight = MotorGroupRight;
    this->IntakeMotor = IntakeMotor;
    this->LiftMotor = LiftMotor;
    this->ClawMotor = ClawMotor;

    this->IntakeSpeed = 75;
}

Manual::~Manual(void) {
    this->Controller = nullptr;
    this->MotorGroupLeft = nullptr;
    this->MotorGroupRight = nullptr;
    this->IntakeMotor = nullptr;
    this->LiftMotor = nullptr;
    this->ClawMotor = nullptr;
}

void Manual::Run() {
    Move();
    ControlIntake();
    ControlLift();
}

void Manual::Move() {
    ControlMotorGroup(MotorGroupLeft, Controller->Axis3);
    ControlMotorGroup(MotorGroupRight, Controller->Axis2);
}

void Manual::ControlMotorGroup(vex::motor_group* MotorGroup, const vex::controller::axis& ControllerAxis) {
    float axisPosition = ControllerAxis.position(vex::percent);
    bool driveReverse = axisPosition < 0;

    if (axisPosition != 0) {
        MotorGroup->setVelocity(std::abs(axisPosition), vex::percent);
        MotorGroup->spin(driveReverse ? vex::reverse : vex::forward);
    } else {
        MotorGroup->stop();
    }
}

void Manual::ControlIntake() {
  // Set intake speed
  if (Controller->ButtonUp.pressing()) IntakeSpeed = 100;
  else if (Controller->ButtonRight.pressing()) IntakeSpeed = 75;
  else if (Controller->ButtonLeft.pressing()) IntakeSpeed = 50;
  else if (Controller->ButtonDown.pressing()) IntakeSpeed = 25;
  
  IntakeMotor->setVelocity(IntakeSpeed, vex::percent);

  // Move intake motor
  if (Controller->ButtonL1.pressing()) {
    IntakeMotor->spin(vex::reverse);
  }
  else if (Controller->ButtonR1.pressing()) {
    IntakeMotor->spin(vex::forward);
  }
  else {
    IntakeMotor->stop();
  }
}

void Manual::ControlLift() {
  if (Controller->ButtonX.pressing()) {
    LiftMotor->spin(vex::forward);
  }
  else if (Controller->ButtonB.pressing()) {
    LiftMotor->spin(vex::reverse);
  }
  else {
    LiftMotor->stop();
  }

  if (Controller->ButtonA.pressing()) {
    ClawMotor->spin(vex::forward);
  }
  else if (Controller->ButtonY.pressing()) {
    ClawMotor->spin(vex::reverse);
  }
  else {
    ClawMotor->stop();
  }
}