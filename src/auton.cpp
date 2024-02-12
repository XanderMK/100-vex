#include "auton.h"

Auton::Auton(vex::brain* Brain, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight,
             vex::motor_group* LauncherMotorGroup, vex::motor* LiftMotor, vex::pneumatics* ClawPiston)
   : Brain(Brain), MotorGroupLeft(MotorGroupLeft), MotorGroupRight(MotorGroupRight),
     LauncherMotorGroup(LauncherMotorGroup), LiftMotor(LiftMotor), ClawPiston(ClawPiston) {}

Auton::~Auton() = default;



#define _SKILLS



void Auton::Run() {
  vex::drivetrain Drivetrain(*MotorGroupLeft, *MotorGroupRight);

#ifdef _SKILLS
    LauncherMotorGroup->setVelocity(LauncherSpeedPercent, vex::percent);
    LauncherMotorGroup->spin(vex::forward);
#endif

#ifdef _NO_MATCH_LOAD
    Drivetrain.setDriveVelocity(50, vex::percent);
    Drivetrain.setTurnVelocity(50, vex::percent);

    // Drive away from wall to orient
    Drivetrain.drive(vex::reverse);
    wait(0.5, vex::seconds);
    Drivetrain.stop();

    // Turn to face the goal
    Drivetrain.turnFor(vex::right, 50, vex::degrees);

    // Drives ball into the goal
    Drivetrain.drive(vex::reverse);
    wait(1.7, vex::seconds);
    Drivetrain.stop();

    // Move back from the goal
    Drivetrain.drive(vex::forward);
    wait(0.7, vex::seconds);
    Drivetrain.stop();
#endif

#ifdef _MATCH_LOAD
    Drivetrain.setDriveVelocity(50, vex::percent);
    Drivetrain.setTurnVelocity(50, vex::percent);

    Drivetrain.driveFor(vex::reverse, 6, vex::inches);
    Drivetrain.driveFor(vex::forward, 6, vex::inches);

/*
    

    // Drive away from wall to orient
    Drivetrain.drive(vex::reverse);
    wait(0.5, vex::seconds);
    Drivetrain.stop();

    // Turn to face the goal
    Drivetrain.turnFor(vex::left, 50, vex::degrees);

    // Drives ball into the goal
    Drivetrain.drive(vex::reverse);
    wait(1.7, vex::seconds);
    Drivetrain.stop();

    // Move back from the goal
    Drivetrain.drive(vex::forward);
    wait(0.7, vex::seconds);
    Drivetrain.stop();

    // Turn around
    Drivetrain.turnFor(vex::right, 180, vex::degrees);

    // Drive in small increments while turning to get in between the wall and the elevation bar
    Drivetrain.drive(vex::reverse);
    wait(0.75, vex::seconds);
    Drivetrain.stop();

    Drivetrain.turnFor(vex::left, 25, vex::degrees);

    Drivetrain.drive(vex::reverse);
    wait(0.5, vex::seconds);
    Drivetrain.stop();

    Drivetrain.turnFor(vex::left, 25, vex::degrees);

    Drivetrain.drive(vex::reverse);
    wait(0.5, vex::seconds);
    Drivetrain.stop();

    Drivetrain.turnFor(vex::left, 19, vex::degrees);

    // Drive into the elevation bar
    Drivetrain.drive(vex::reverse);
    wait(2, vex::seconds);
    Drivetrain.stop();
*/
#endif
   
}