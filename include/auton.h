#pragma once
#include "vex.h"

class Auton {
public:
    Auton(vex::brain* Brain,
          vex::motor_group* MotorGroupLeft, 
          vex::motor_group* MotorGroupRight, 
          vex::motor* LauncherMotor,
          vex::motor* WingMotor,
          vex::inertial* InertialSensor
         );
    ~Auton(void);

    void Run();

private:
    vex::brain* Brain;
    vex::controller* Controller;
    vex::motor_group* MotorGroupLeft;
    vex::motor_group* MotorGroupRight;
    vex::motor* LauncherMotor;
    vex::motor* WingMotor;
    vex::inertial* InertialSensor;

    const float LauncherSpeedPercent = 80.f;
};