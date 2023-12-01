#include "manual.h"
#include "vex.h"
#include "utils.h"
#include <cmath>

Manual::Manual(vex::brain* Brain, vex::controller* Controller, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight, 
               vex::motor* IntakeMotor, vex::motor* LiftMotor, vex::motor* ClawMotor)
    : Brain(Brain), Controller(Controller), MotorGroupLeft(MotorGroupLeft), MotorGroupRight(MotorGroupRight), 
      IntakeMotor(IntakeMotor), LiftMotor(LiftMotor), ClawMotor(ClawMotor) {

    IntakeMotor->setVelocity(IntakeSpeedPercent, vex::percent);

    Brain->Screen.clearScreen();
    Brain->Screen.setCursor(1, 1);
}

Manual::~Manual() = default;

void Manual::Run() {
    Move();
    SetDriveSpeed();
    ControlIntake();
    ControlLift();
    PrintOnBrain();
    PrintOnController();
}

void Manual::Move() {
    ControlMotorGroup(MotorGroupLeft, Controller->Axis3);
    ControlMotorGroup(MotorGroupRight, Controller->Axis2);
}

void Manual::ControlMotorGroup(vex::motor_group* MotorGroup, const vex::controller::axis& ControllerAxis) {
    float axisPosition = ControllerAxis.position(vex::percent);
    float motorGroupSpeed = axisPosition * lerp(MinDrivePercent, MaxDrivePercent, currentDriveInterval);

    bool driveReverse = axisPosition < 0;

    if (axisPosition != 0) {
        MotorGroup->setVelocity(std::abs(motorGroupSpeed), vex::percent);
        MotorGroup->spin(driveReverse ? vex::reverse : vex::forward);
    } else {
        MotorGroup->stop();
    }
}

void Manual::SetDriveSpeed() {
  // Toggles for changing drivetrain speed
    static bool buttonUpLatch = false;
    static bool buttonDownLatch = false;


    if (Controller->ButtonUp.pressing() && !buttonUpLatch) {
        currentDriveInterval += 1.f / DriveIntervals;
        buttonUpLatch = true;
    }
    else if (!Controller->ButtonUp.pressing() && buttonUpLatch)
        buttonUpLatch = false;
        

    if (Controller->ButtonDown.pressing() && !buttonDownLatch) {
        currentDriveInterval -= 1.f / DriveIntervals;
        buttonDownLatch = true;
    }
    else if (!Controller->ButtonDown.pressing() && buttonDownLatch)
        buttonDownLatch = false;

    if (currentDriveInterval > 1.f)
        currentDriveInterval = 1.f;
    else if (currentDriveInterval < 0.f)
        currentDriveInterval = 0.f;
}

void Manual::ControlIntake() {
  static bool buttonL1Latch = false;
  static bool buttonR1Latch = false;

  static short currentIntakeState = 0; // 0 = off, 1 = in, 2 = out

  if (Controller->ButtonL1.pressing() && !buttonL1Latch) {
    if (currentIntakeState == 1) {
      currentIntakeState = 0;
    } else {
      currentIntakeState = 1;
    }

    buttonL1Latch = true;
  }
  else if (!Controller->ButtonL1.pressing() && buttonL1Latch)
    buttonL1Latch = false;

  if (Controller->ButtonR1.pressing() && !buttonR1Latch) {
    if (currentIntakeState == 2) {
      currentIntakeState = 0;
    } else {
      currentIntakeState = 2;
    }

    buttonR1Latch = true;
  }
  else if (!Controller->ButtonR1.pressing() && buttonR1Latch)
    buttonR1Latch = false;

  // Move intake motor
  switch (currentIntakeState) {
    case 0:
      IntakeMotor->stop();
      break;
    case 1:
      IntakeMotor->spin(vex::forward);
      break;
    case 2:
      IntakeMotor->spin(vex::reverse);
      break;
  }
}

void Manual::ControlLift() {
  if (Controller->ButtonX.pressing()) {
    LiftMotor->spin(vex::forward, LiftSpeedPercentUp, vex::percent);
  }
  else if (Controller->ButtonB.pressing()) {
    LiftMotor->spin(vex::reverse, LiftSpeedPercentDown, vex::percent);
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
    ClawMotor->stop();
  }
}

void Manual::PrintOnBrain() {
    Brain->Screen.clearScreen();
    Brain->Screen.setCursor(1, 1);
    Brain->Screen.print("Manual");
    Brain->Screen.setCursor(2, 1);
    Brain->Screen.print("Drive Speed: %f", currentDriveInterval);
}

void Manual::PrintOnController() {
    static int cyclesRan = 0;

    cyclesRan++;
    if (cyclesRan < 15) return;

    cyclesRan = 0;

    Controller->Screen.clearScreen();
    Controller->Screen.setCursor(1, 1);
    Controller->Screen.print("Manual");
    Controller->Screen.setCursor(2, 1);
    Controller->Screen.print("Drive Speed: %d%", int(round(currentDriveInterval * 100)));
}