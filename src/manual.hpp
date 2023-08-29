#pragma once
#include "vex.h"

class Manual {
public:
    Manual(vex::controller* Controller, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight);
    ~Manual(void);

    void Run();

private:
    void Move();
    void ControlMotorGroup(vex::motor_group* MotorGroup, const vex::controller::axis& ControllerAxis);

    vex::controller* Controller;
    vex::motor_group* MotorGroupLeft;
    vex::motor_group* MotorGroupRight;
};