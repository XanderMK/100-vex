#pragma once
#include "vex.h"

class Manual {
public:
    Manual(vex::brain* Brain,
           vex::controller* Controller, 
           vex::motor_group* MotorGroupLeft, 
           vex::motor_group* MotorGroupRight, 
           vex::motor* FirstStageLiftMotor,
           vex::motor* SecondStageLiftMotor,
           vex::motor* LiftRatchetMotor,
           vex::motor* LauncherMotor,
           vex::motor* WingMotor);
    ~Manual(void);

    void Run();

private:
    void Move();
    void ControlMotorGroup(vex::motor_group* MotorGroup, const vex::controller::axis& ControllerAxis);
    void SetDriveSpeed();
    void ControlLift();
    void RaiseLift();
    void LowerLift();
    void RatchetLiftDown();
    void SpinLauncher();
    void ControlWing();
    void PrintOnBrain();
    void PrintOnController();

    vex::brain* Brain;
    vex::controller* Controller;
    vex::motor_group* MotorGroupLeft;
    vex::motor_group* MotorGroupRight;
    vex::motor* FirstStageLiftMotor;
    vex::motor* SecondStageLiftMotor;
    vex::motor* LiftRatchetMotor;
    vex::motor* LauncherMotor;
    vex::motor* WingMotor;

    float currentDriveInterval = .5f;
    bool isLiftBeingUsed = false;

    const float MaxDrivePercent = 1.f;
    const float MinDrivePercent = .25f;
    const int DriveIntervals = 4;
    const float LauncherSpeedPercent = 80.f;
};