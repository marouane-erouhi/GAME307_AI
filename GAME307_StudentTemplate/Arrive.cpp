#include "Arrive.h"

using namespace std;

Arrive::Arrive(const Body* npc_, const Body* target_, float targetRadius_, float slowRadius_, float timeToTarget_) {
	npc = npc_;
	target = target_;
	targetRadius = targetRadius_;
	slowRadius = slowRadius_;
	timeToTraget = timeToTarget_;
}

Arrive::~Arrive() {
	if (npc)	delete npc;
	if (target)	delete target;
}

SteeringOutput* Arrive::getSteering() {
	// get distance from target
	float distance = VMath::distance(npc->getPos(), target->getPos());

	printf("Distance: %d.2\n", distance);

	// arrived
	if (distance <= targetRadius) {
		result->linear = Vec3();
		result->angular = 0;

		cout << "Arrived\n";

		return result;
	}

	result->linear = target->getPos() - npc->getPos();

	result->linear = VMath::normalize(result->linear);
	result->linear *= npc->getMaxAcceleration();
	result->angular = 0;

	// close enough to slow down
	if (distance < slowRadius) {
		//result->linear = npc->getMaxAcceleration() * (1 - timeToTraget);
		result->linear *= (1 - timeToTraget);
	}


	return result;
}
