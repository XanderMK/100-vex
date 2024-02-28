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

  InertialSensor->calibrate();
  while (InertialSensor->isCalibrating()) {
    vex::task::sleep(100);
  }

  InertialSensor->setHeading(45, vex::degrees);

#ifdef _SKILLS
    LauncherMotor->setVelocity(LauncherSpeedPercent, vex::percent);
    LauncherMotor->spin(vex::forward);
#endif

#ifdef _NO_MATCH_LOAD
    Drivetrain.setDriveVelocity(100, vex::percent);
    Drivetrain.setTurnVelocity(25, vex::percent);

    Drivetrain.driveFor(72, vex::inches);
#endif

#ifdef _MATCH_LOAD
    Drivetrain.setDriveVelocity(40, vex::percent);
    Drivetrain.setTurnVelocity(40, vex::percent);
    Drivetrain.setStopping(vex::brakeType::hold);
    Drivetrain.driveFor(vex::reverse, 6, vex::inches);
    Drivetrain.stop();
    WingMotor->spinToPosition(120, vex::degrees);
    Drivetrain.turnFor(-124, vex::degrees); // Compensating for weird turn inconsistensies
    Drivetrain.stop();
    WingMotor->spinToPosition(0, vex::degrees);
    Drivetrain.setDriveVelocity(10, vex::percent);
    Drivetrain.driveFor(2, vex::inches);
    Drivetrain.stop();
    Drivetrain.setDriveVelocity(40, vex::percent);
    Drivetrain.turnFor(85, vex::degrees);
    Drivetrain.stop();
    Drivetrain.driveFor(48, vex::inches);

#endif
   
}