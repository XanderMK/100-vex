#include "auton.h"

Auton::Auton(vex::brain* Brain, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight,
             vex::motor* LauncherMotor, vex::motor* WingMotor, vex::inertial* InertialSensor)
   : Brain(Brain), MotorGroupLeft(MotorGroupLeft), MotorGroupRight(MotorGroupRight),
     LauncherMotor(LauncherMotor), WingMotor(WingMotor), InertialSensor(InertialSensor) {}

Auton::~Auton() = default;



#define _MATCH_LOAD



void Auton::Run() {
  vex::drivetrain Drivetrain(*MotorGroupLeft, *MotorGroupRight);
  Drivetrain.setGearRatio(2. + (1. / 3.));

#ifdef _SKILLS
    LauncherMotor->setVelocity(LauncherSpeedPercent, vex::percent);
    LauncherMotor->spin(vex::forward);

    vex::wait(45, vex::seconds);

    LauncherMotor->stop(vex::coast);

    Drivetrain.setDriveVelocity(75, vex::percent);
    Drivetrain.setTurnVelocity(30, vex::percent);

    Drivetrain.driveFor(12, vex::inches);
    Drivetrain.turnFor(90, vex::degrees);

    Drivetrain.setTimeout(2, vex::seconds);
    Drivetrain.driveFor(24, vex::inches);

    Drivetrain.setTimeout(60, vex::seconds);
    Drivetrain.driveFor(-3, vex::inches);

    Drivetrain.turnFor(-75, vex::degrees);

    Drivetrain.driveFor(66, vex::inches);
    Drivetrain.turnFor(-42, vex::degrees);

    WingMotor->spinToPosition(120, vex::degrees);
    Drivetrain.setDriveVelocity(80, vex::percent);
    Drivetrain.driveFor(36, vex::inches);
    
    
#endif

#ifdef _NO_MATCH_LOAD
    Drivetrain.setDriveVelocity(100, vex::percent);
    Drivetrain.setTurnVelocity(25, vex::percent);

    Drivetrain.driveFor(72, vex::inches);
#endif

#ifdef _MATCH_LOAD
    InertialSensor->startCalibration();
    while (InertialSensor->isCalibrating()) {
      vex::wait(0.05, vex::seconds);
    }

    Drivetrain.setDriveVelocity(40, vex::percent);
    Drivetrain.setTurnVelocity(40, vex::percent);
    Drivetrain.setStopping(vex::brakeType::hold);

    Drivetrain.driveFor(vex::reverse, 6, vex::inches);
    Drivetrain.stop();
    WingMotor->spinToPosition(120, vex::degrees);
    Drivetrain.turnFor(-135, vex::degrees); // Compensating for weird turn inconsistensies
    Drivetrain.stop();
    WingMotor->spinToPosition(0, vex::degrees);
    Drivetrain.setDriveVelocity(15, vex::percent);
    Drivetrain.driveFor(2, vex::inches);
    Drivetrain.stop();
    Drivetrain.setDriveVelocity(40, vex::percent);
    Drivetrain.setTurnVelocity(20, vex::percent);
    Drivetrain.turn(vex::right);
    while (InertialSensor->heading(vex::degrees) < 305.) {
      vex::wait(0.01, vex::seconds);
    }
    Drivetrain.stop();
    Drivetrain.driveFor(48, vex::inches);

#endif
   
}