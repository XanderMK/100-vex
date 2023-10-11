#include "manual.h"
#include "vex.h"
#include "utils.h"
#include <cmath>

Manual::Manual(vex::controller* Controller, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight, 
               vex::motor* IntakeMotor, vex::motor* LiftMotor, vex::motor* ClawMotor) {
    this->Controller = Controller;
    this->MotorGroupLeft = MotorGroupLeft;
    this->MotorGroupRight = MotorGroupRight;
    this->IntakeMotor = IntakeMotor;
    this->LiftMotor = LiftMotor;
    this->ClawMotor = ClawMotor;

    IntakeMotor->setVelocity(IntakeSpeed, vex::percent);
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
    static float currentDriveInterval = 1.f;

    float axisPosition = ControllerAxis.position(vex::percent);
    float motorGroupSpeed = axisPosition * lerp(MinDrivePercent, MaxDrivePercent, currentDriveInterval);

    bool driveReverse = axisPosition < 0;

    if (axisPosition != 0) {
        MotorGroup->setVelocity(std::abs(motorGroupSpeed), vex::percent);
        MotorGroup->spin(driveReverse ? vex::reverse : vex::forward);
    } else {
        MotorGroup->stop();
    }


    // Toggles for changing drivetrain speed
    static bool buttonUpLatch = false;
    static bool buttonDownLatch = false;
    if (Controller->ButtonUp.pressing() && !buttonUpLatch && currentDriveInterval <= MaxDrivePercent) 
        currentDriveInterval += (MaxDrivePercent - MinDrivePercent) / DriveIntervals;
    else if (Controller->ButtonDown.pressing() && !buttonDownLatch && currentDriveInterval >= MinDrivePercent) 
        currentDriveInterval -= (MaxDrivePercent - MinDrivePercent) / DriveIntervals;

    
    // Make sure buttons can't toggle things every cycle
    if (Controller->ButtonUp.pressing() && !buttonUpLatch) {
        buttonUpLatch = true;
    } else {
        buttonUpLatch = false;
    }

    if (Controller->ButtonDown.pressing() && !buttonDownLatch) {
        buttonDownLatch = true;
    } else {
        buttonDownLatch = false;
    }
}

void Manual::ControlIntake() {
  IntakeMotor->setVelocity(IntakeSpeed, vex::percent);

  // Move intake motor
  if (Controller->ButtonL1.pressing()) {
    IntakeMotor->spin(vex::reverse);
  }
  else if (Controller->ButtonR1.pressing()) {
    IntakeMotor->spin(vex::forward);
  }
  else {
    IntakeMotor->stop(vex::hold);
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
    LiftMotor->stop(vex::hold);
  }

  if (Controller->ButtonA.pressing()) {
    ClawMotor->spin(vex::forward);
  }
  else if (Controller->ButtonY.pressing()) {
    ClawMotor->spin(vex::reverse);
  }
  else {
    ClawMotor->stop(vex::hold);
  }
}