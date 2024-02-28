#include "manual.h"
#include "vex.h"
#include "utils.h"
#include <cmath>

Manual::Manual(vex::brain* Brain, vex::controller* Controller, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight, 
               vex::motor* FirstStageLiftMotor, vex::motor* SecondStageLiftMotor, vex::motor* LiftRatchetMotor, vex::motor* LauncherMotor, vex::motor* WingMotor)
    : Brain(Brain), Controller(Controller), MotorGroupLeft(MotorGroupLeft), MotorGroupRight(MotorGroupRight), FirstStageLiftMotor(FirstStageLiftMotor), 
      SecondStageLiftMotor(SecondStageLiftMotor), LiftRatchetMotor(LiftRatchetMotor), LauncherMotor(LauncherMotor), WingMotor(WingMotor) {
    
    LauncherMotor->setVelocity(LauncherSpeedPercent, vex::percent);

    FirstStageLiftMotor->setVelocity(100, vex::percent);
    SecondStageLiftMotor->setVelocity(100, vex::percent);
    LiftRatchetMotor->setVelocity(100, vex::percent);

    Brain->Screen.clearScreen();
    Brain->Screen.setCursor(1, 1);
}

Manual::~Manual() = default;

void Manual::Run() {
    Move();
    SetDriveSpeed();
    ControlLift();
    PrintOnBrain();
    PrintOnController();
}

void Manual::Move() {
    SetDriveSpeed();
    ControlMotorGroup(MotorGroupLeft, Controller->Axis3);
    ControlMotorGroup(MotorGroupRight, Controller->Axis2);

    ControlLift();
    RatchetLiftDown();

    SpinLauncher();

    ControlWing();

    PrintOnBrain();
    PrintOnController();
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

void Manual::ControlLift() {
    if (Controller->ButtonL1.pressing() && !isLiftBeingUsed) {
        RaiseLift();
    }

    else if (Controller->ButtonL2.pressing() && !isLiftBeingUsed) {
        LowerLift();
    }
}

void Manual::RaiseLift() {
    isLiftBeingUsed = true;

    FirstStageLiftMotor->spinToPosition(195, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
    SecondStageLiftMotor->spinToPosition(170, vex::rotationUnits::deg, 30, vex::velocityUnits::pct, false);

    isLiftBeingUsed = false;
}

void Manual::LowerLift() {
    isLiftBeingUsed = true;

    FirstStageLiftMotor->spinToPosition(0, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
    SecondStageLiftMotor->spinToPosition(0, vex::rotationUnits::deg, 30, vex::velocityUnits::pct, false);

    isLiftBeingUsed = false;
}

void Manual::RatchetLiftDown() {
    if (Controller->ButtonY.pressing() && Controller->ButtonA.pressing()) {
        FirstStageLiftMotor->stop(vex::coast);
        SecondStageLiftMotor->stop(vex::coast);

        LiftRatchetMotor->spin(vex::forward);
    }
    else {
        LiftRatchetMotor->stop(vex::coast);
    
    }
    
}

void Manual::SpinLauncher() {
    static bool launcherToggle = false;
    static bool r1Latch = false;

    if (Controller->ButtonR1.pressing() && !r1Latch) {
        launcherToggle = !launcherToggle;
        r1Latch = true;
    }
    else if (!Controller->ButtonR1.pressing() && r1Latch){
        r1Latch = false;
    }

    if (launcherToggle) {
        LauncherMotor->spin(vex::forward);
    }
    else {
        LauncherMotor->stop(vex::coast);
    }
    
}

void Manual::ControlWing() {
    static bool wingToggle = false;
    static bool r2Latch = false;

    if (Controller->ButtonR2.pressing() && !r2Latch) {
        wingToggle = !wingToggle;
        r2Latch = true;
    }
    else if (!Controller->ButtonR2.pressing() && r2Latch){
        r2Latch = false;
    }

    if (wingToggle) {
        WingMotor->spinToPosition(120, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
    }
    else {
        WingMotor->spinToPosition(0, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
    }
}

void Manual::PrintOnBrain() {
    Brain->Screen.clearScreen();

    Brain->Screen.setCursor(1, 1);
    Brain->Screen.setFont(vex::fontType::mono12);
    Brain->Screen.print("Drive Speed: %d%", int(round(currentDriveInterval * 100)));
    Brain->Screen.setCursor(2, 1);
    Brain->Screen.setFont(vex::fontType::mono60);
    Brain->Screen.print("it's time to vex");
    Brain->Screen.setCursor(3, 1);
    Brain->Screen.setFont(vex::fontType::mono20);
    Brain->Screen.print("Flywheel temp: %dº C", int(round(LauncherMotor->temperature(vex::temperatureUnits::celsius))));
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
}