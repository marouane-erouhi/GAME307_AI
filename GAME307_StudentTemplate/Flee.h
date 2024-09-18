#pragma once
#include "SteeringBehaviour.h"
class Flee : public SteeringBehaviour {

protected:
	const Body* target;

public:
	Flee(const Body* npc_, const Body* target_);
	virtual ~Flee();

	SteeringOutput* getSteering();

};

