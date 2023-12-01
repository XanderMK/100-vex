#include "auton.h"

Auton::Auton(vex::brain* Brain, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight,
             vex::motor* IntakeMotor, vex::motor* LiftMotor, vex::motor* ClawMotor)
   : Brain(Brain), MotorGroupLeft(MotorGroupLeft), MotorGroupRight(MotorGroupRight),
     IntakeMotor(IntakeMotor), LiftMotor(LiftMotor), ClawMotor(ClawMotor) {}

Auton::~Auton() = default;

void Auton::Run() {
    vex::drivetrain Drivetrain(*MotorGroupLeft, *MotorGroupRight);
    Drivetrain.setDriveVelocity(35, vex::percent);
    Drivetrain.setTurnVelocity(35, vex::percent);

    // SKILLS
    Drivetrain.driveFor(vex::directionType::fwd, 5 * 12, vex::distanceUnits::in);
    Drivetrain.turnFor(75, vex::rotationUnits::deg);
    IntakeMotor->spin(vex::directionType::fwd);
    vex::wait(1, vex::timeUnits::sec);
    Drivetrain.driveFor(vex::directionType::rev, 6, vex::distanceUnits::in);
    IntakeMotor->stop();
    Drivetrain.turnFor(150, vex::rotationUnits::deg);
    Drivetrain.driveFor(vex::directionType::rev, 96, vex::distanceUnits::in);
    // SKILLS
}