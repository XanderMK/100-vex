#pragma once
#include "vex.h"

class Manual {
public:
    Manual(vex::brain* Brain,
           vex::controller* Controller, 
           vex::motor_group* MotorGroupLeft, 
           vex::motor_group* MotorGroupRight, 
           vex::motor* IntakeMotor, 
           vex::motor* LiftMotor,
           vex::motor* ClawMotor);
    ~Manual(void);

    void Run();

private:
    void Move();
    void ControlMotorGroup(vex::motor_group* MotorGroup, const vex::controller::axis& ControllerAxis);
    void SetDriveSpeed();
    void ControlIntake();
    void ControlLift();
    void PrintOnBrain();
    void PrintOnController();

    vex::brain* Brain;
    vex::controller* Controller;
    vex::motor_group* MotorGroupLeft;
    vex::motor_group* MotorGroupRight;
    vex::motor* IntakeMotor;
    vex::motor* LiftMotor;
    vex::motor* ClawMotor;

    float currentDriveInterval = .5f;
    const float MaxDrivePercent = 1.f;
    const float MinDrivePercent = .25f;
    const int DriveIntervals = 4;
    const int IntakeSpeedPercent = 100;

    const float LiftSpeedPercentUp = 100.f;
    const float LiftSpeedPercentDown = 65.f;
};