#pragma once
#include "SteeringOutput.h"
#include <Body.h>

class SteeringBehaviour {
protected:
	SteeringOutput* result;
	const Body* npc;
public:
	SteeringBehaviour() {
		result = new SteeringOutput();
		npc = NULL;
	}
	~SteeringBehaviour() {
		if (result)	delete result;
	}

	virtual SteeringOutput* getSteering() = 0;

};