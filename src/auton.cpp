#include "auton.h"

Auton::Auton(vex::motor_group* MotorGroupLeft, 
             vex::motor_group* MotorGroupRight, 
             vex::motor* IntakeMotor, 
             vex::motor* LiftMotor,
             vex::motor* ClawMotor) {
    this->MotorGroupLeft = MotorGroupLeft;
    this->MotorGroupRight = MotorGroupRight;
    this->IntakeMotor = IntakeMotor;
    this->LiftMotor = LiftMotor;
    this->ClawMotor = ClawMotor;
}

Auton::~Auton(void) {
    this->MotorGroupLeft = nullptr;
    this->MotorGroupRight = nullptr;
    this->IntakeMotor = nullptr;
    this->LiftMotor = nullptr;
    this->ClawMotor = nullptr;
}

void Auton::Run() {
    
}