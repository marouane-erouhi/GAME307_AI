#pragma once
#include "SteeringBehaviour.h"

class Arrive : public SteeringBehaviour {

protected:
	const Body* target;
	float targetRadius; //The radius for arriving at the target
	float slowRadius;
	float timeToTarget = 0.1f;

public:
	Arrive(const Body* npc_, const Body* target_, float targetRadius_, float slowRadius_, float timeToTraget_);
	virtual ~Arrive();

	SteeringOutput* getSteering();

};
