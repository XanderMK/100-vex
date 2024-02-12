#pragma once
#include "vex.h"

class Auton {
public:
    Auton(vex::brain* Brain,
          vex::motor_group* MotorGroupLeft, 
          vex::motor_group* MotorGroupRight, 
          vex::motor_group* LauncherMotorGroup, 
          vex::motor* LiftMotor,
          vex::pneumatics* ClawPiston);
    ~Auton(void);

    void Run();

private:
    vex::brain* Brain;
    vex::controller* Controller;
    vex::motor_group* MotorGroupLeft;
    vex::motor_group* MotorGroupRight;
    vex::motor_group* LauncherMotorGroup;
    vex::motor* LiftMotor;
    vex::pneumatics* ClawPiston;

    const float LauncherSpeedPercent = 52.5f;
};