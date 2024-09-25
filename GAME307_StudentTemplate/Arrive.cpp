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
	SteeringOutput* out = new SteeringOutput();
	Vec3 dir = target->getPos() - npc->getPos();
	float distance = VMath::mag(dir);

	if (distance < targetRadius) {
		// arrived
		return nullptr;
	}

	float targetSpeed = 0.0f;
	if (distance > slowRadius) {
		// max speed
		targetSpeed = npc->getMaxSpeed();
	} else {
		// within slow radius
		targetSpeed = npc->getMaxSpeed() * distance / slowRadius;
	}

	Vec3 targetVel = VMath::normalize(Vec3(dir)) * targetSpeed;
	out->linear = (targetVel - npc->getVel()) / timeToTraget;

	// clip to max acc if too high
	if (VMath::mag(out->linear) > npc->getMaxAcceleration()) {
		out->linear = VMath::normalize(out->linear) * npc->getMaxAcceleration();
	}
	out->angular = 0.0f;
	return out;
}
