#ifndef ARM_ELEVATORMGR_H
#define ARM_ELEVATORMGR_H

#include "../library/Singleton.hpp"
#include "Elevator.h"
#include "../library/delay.h"
#include <../library/Uart.hpp>
#include "SensorMgr.h"

extern Uart<1> serial;
class ElevatorMgr : public Singleton<ElevatorMgr>
{
public:
    enum Position{
        UP, DOWN
    };

    ElevatorMgr();
    void control();
    void moveTo(Position);
    void stop();

    void enableAsserv(bool);

    bool elevatorIsMoving() const;
    bool elevatorMoveAbnormal() const;

    void getData();

    Position positionSetpoint;
    bool isMovementAbnormal;

private:
    Elevator elevator;
    SensorMgr* sensorMgr;
    volatile int32_t elevatorPWM;

    volatile bool positionControlled;
    volatile bool isElevatorMoving;
    Position position;
    bool isUp;
    bool isDown;
};

#endif //ARM_ELEVATORMGR_H