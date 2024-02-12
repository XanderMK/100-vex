#include "manual.h"
#include "vex.h"
#include "utils.h"
#include <cmath>

Manual::Manual(vex::brain* Brain, vex::controller* Controller, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight, 
               vex::motor_group* LauncherMotorGroup, vex::motor* LiftMotor, vex::pneumatics* ClawPiston, vex::motor* TheMechanism)
    : Brain(Brain), Controller(Controller), MotorGroupLeft(MotorGroupLeft), MotorGroupRight(MotorGroupRight), 
      LauncherMotorGroup(LauncherMotorGroup), LiftMotor(LiftMotor), ClawPiston(ClawPiston), TheMechanism(TheMechanism) {

    Brain->Screen.clearScreen();
    Brain->Screen.setCursor(1, 1);

    LauncherMotorGroup->setVelocity(LauncherSpeedPercent, vex::percent);
}

Manual::~Manual() = default;

void Manual::Run() {
    Move();
    SetDriveSpeed();
    ControlLauncher();
    ControlLift();
    ControlTheMechanism();
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

void Manual::ControlLauncher() {
  static bool buttonR1Latch = false;

  static bool intakeRunning;

  if (Controller->ButtonR1.pressing() && !buttonR1Latch) {
    intakeRunning = !intakeRunning;

    buttonR1Latch = true;
  }
  else if (!Controller->ButtonR1.pressing() && buttonR1Latch)
    buttonR1Latch = false;

  if (intakeRunning) {
    LauncherMotorGroup->spin(vex::forward);
  }
  else {
    LauncherMotorGroup->stop();
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

  if (Controller->ButtonA.pressing() && Controller->ButtonY.pressing()) {
    ClawPiston->open();
  }
  
}

void Manual::ControlTheMechanism() {
  static bool buttonRightLatch = false;

  // Toggle holding the motor in place. 
  if (Controller->ButtonRight.pressing() && !buttonRightLatch) {
        HoldTheMechanism = !HoldTheMechanism;
        buttonRightLatch = true;
  }
  else if (!Controller->ButtonRight.pressing() && buttonRightLatch)
      buttonRightLatch = false;


  if (Controller->ButtonR2.pressing()) {
    TheMechanism->spin(vex::forward, 100, vex::percent);
  }
  else if (Controller->ButtonL2.pressing()) {
    TheMechanism->spin(vex::reverse, 100, vex::percent);
  }
  else {
    TheMechanism->stop(HoldTheMechanism ? vex::hold : vex::coast);
  }
}

void Manual::PrintOnBrain() {
    Brain->Screen.clearScreen();  
}

void Manual::PrintOnController() {
    static int cyclesRan = 0;

    cyclesRan++;
    if (cyclesRan < 15) return;

    cyclesRan = 0;

    Controller->Screen.clearScreen();
    Controller->Screen.setCursor(1, 1);
    Controller->Screen.print("teeheehee! now");
    Controller->Screen.setCursor(2, 1);
    Controller->Screen.print("Drive Speed: %d%", int(round(currentDriveInterval * 100)));
    Controller->Screen.setCursor(3, 1);
    Controller->Screen.print("Holding: %s", HoldTheMechanism ? "true" : "false");
}