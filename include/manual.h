#pragma once
#include "vex.h"

class Manual {
public:
    Manual(vex::controller* Controller, 
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
    void ControlIntake();
    void ControlLift();

    vex::controller* Controller;
    vex::motor_group* MotorGroupLeft;
    vex::motor_group* MotorGroupRight;
    vex::motor* IntakeMotor;
    vex::motor* LiftMotor;
    vex::motor* ClawMotor;

    int IntakeSpeed;
};