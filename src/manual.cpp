#include "manual.hpp"
#include "vex.h"
#include <cmath>
#include <iostream>

Manual::Manual(vex::controller* Controller, vex::motor_group* MotorGroupLeft, vex::motor_group* MotorGroupRight) {
    this->Controller = Controller;
    this->MotorGroupLeft = MotorGroupLeft;
    this->MotorGroupRight = MotorGroupRight;
}

Manual::~Manual(void) {
    this->Controller = nullptr;
    this->MotorGroupLeft = nullptr;
    this->MotorGroupRight = nullptr;
}

void Manual::Run() {
    Move();
}

void Manual::Move() {
    ControlMotorGroup(MotorGroupLeft, Controller->Axis3);
    ControlMotorGroup(MotorGroupRight, Controller->Axis2);
}

void Manual::ControlMotorGroup(vex::motor_group* MotorGroup, const vex::controller::axis& ControllerAxis) {
    float axisPosition = ControllerAxis.position(vex::percent);
    bool driveReverse = axisPosition < 0;

    if (axisPosition != 0) {
        MotorGroup->setVelocity(std::abs(axisPosition), vex::percent);
        MotorGroup->spin(driveReverse ? vex::reverse : vex::forward);
    } else {
        MotorGroup->stop();
    }

    #ifndef _DEBUG
    std::cout << "Axis Position: " << axisPosition << "%\n";
    #endif
}