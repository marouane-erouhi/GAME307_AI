#pragma once
#include "SteeringBehaviour.h"


class Align : public SteeringBehaviour {
    const Body* target;

    float targetRadius;
    float slowRadius;
    float timeToTarget = 0.1;
public:
    Align(const Body* npc_, const Body* target_, float targetRadius_, float slowRadius_, float timeToTraget_);
    ~Align();

    SteeringOutput* getSteering();

};
