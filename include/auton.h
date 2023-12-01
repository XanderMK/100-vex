#pragma once
#include "vex.h"

class Auton {
public:
    Auton(vex::brain* Brain,
          vex::motor_group* MotorGroupLeft, 
          vex::motor_group* MotorGroupRight, 
          vex::motor* IntakeMotor, 
          vex::motor* LiftMotor,
          vex::motor* ClawMotor);
    ~Auton(void);

    void Run();

private:
    vex::brain* Brain;
    vex::controller* Controller;
    vex::motor_group* MotorGroupLeft;
    vex::motor_group* MotorGroupRight;
    vex::motor* IntakeMotor;
    vex::motor* LiftMotor;
    vex::motor* ClawMotor;
};