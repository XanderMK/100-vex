#pragma once
#include "vex.h"

class Manual {
public:
    Manual(vex::brain* Brain,
           vex::controller* Controller, 
           vex::motor_group* MotorGroupLeft, 
           vex::motor_group* MotorGroupRight, 
           vex::motor_group* LauncherMotorGroup,
           vex::motor* LiftMotor,
           vex::pneumatics* ClawPiston,
           vex::motor* TheMechanism);
    ~Manual(void);

    void Run();

private:
    void Move();
    void ControlMotorGroup(vex::motor_group* MotorGroup, const vex::controller::axis& ControllerAxis);
    void SetDriveSpeed();
    void ControlLauncher();
    void ControlLift();
    void ControlTheMechanism();
    void PrintOnBrain();
    void PrintOnController();

    vex::brain* Brain;
    vex::controller* Controller;
    vex::motor_group* MotorGroupLeft;
    vex::motor_group* MotorGroupRight;
    vex::motor_group* LauncherMotorGroup;
    vex::motor* LiftMotor;
    vex::pneumatics* ClawPiston;
    vex::motor* TheMechanism;

    float currentDriveInterval = .5f;
    const float MaxDrivePercent = 1.f;
    const float MinDrivePercent = .25f;
    const int DriveIntervals = 4;
    const float LauncherSpeedPercent = 52.5f;

    const float LiftSpeedPercentUp = 100.f;
    const float LiftSpeedPercentDown = 65.f;

    bool HoldTheMechanism = false;
};