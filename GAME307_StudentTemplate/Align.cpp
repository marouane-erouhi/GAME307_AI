#include "Align.h"
#include <math.h>

double mapToRange(double r) {
    r = fmod(r, 2 * M_PI);
    if (r < -M_PI) {
        r += 2 * M_PI;
    }
    if (r >= M_PI) {
        r -= 2 * M_PI;
    }
    return r;
}

Align::Align(const Body* npc_, const Body* target_, float targetRadius_, float slowRadius_, float timeToTraget_) {
    npc = npc_;
    target = target_;
    targetRadius = targetRadius_;
    slowRadius = slowRadius_;
    timeToTarget = timeToTraget_;
}

Align::~Align() {
}

SteeringOutput* Align::getSteering() {
    SteeringOutput* out = new SteeringOutput();
    //Get the naive direction to the target
    auto rotation = target->getOrientation() - npc->getOrientation();

    rotation = mapToRange(rotation);
    float rotationSize = fabs(rotation);

    //Check if we are there, return no steering
    if (rotationSize < targetRadius) {
        return out;
    }

    float targetRotation;
    //outside the slowRadius, then use maximum rotation
    if (rotationSize > slowRadius) {
        targetRotation = npc->getMaxRotation();
    } else {
        targetRotation = npc->getMaxRotation() * rotationSize / slowRadius;
    }

    //The final target rotation combines speed and dir
    targetRotation *= rotation / rotationSize;
    out->angular = targetRotation - npc->getAngular();
    out->angular /= timeToTarget;

    //Check if the acceleration is too great
    auto angularAcceleration = abs(out->angular);
    if (angularAcceleration > npc->getMaxAngular()) {
        out->angular /= angularAcceleration;
        out->angular *= npc->getMaxAngular();
    }
    out->linear = Vec3();
    return out;
}