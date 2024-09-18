#include "Flee.h"

Flee::Flee(const Body* npc_, const Body* target_) {
	npc = npc_;
	target = target_;
}

Flee::~Flee() {
	if (npc)	delete npc;
	if (target)	delete target;
}

SteeringOutput* Flee::getSteering() {
	// get dir to target
	result->linear = npc->getPos() - target->getPos();

	result->linear = VMath::normalize(result->linear);
	result->linear *= npc->getMaxAcceleration();
	result->angular = 0;

	return result;
}
