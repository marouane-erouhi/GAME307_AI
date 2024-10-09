#pragma once
#include "Body.h"
#include "SteeringBehaviour.h"
#include <math.h>
#include "VMath.h"

class VelocityMatch : public SteeringBehaviour {
public:
    const Body* target;
    float maxAcceleration;
    float timeToTarget = 0.1f;

    VelocityMatch(const Body* npc_, const Body* target_) : target(target_) {
        npc = npc_;
    }

    SteeringOutput* getSteering() override {
        // Create the structure to hold our output
        SteeringOutput* steering = new SteeringOutput();

        // Acceleration tries to get to the target velocity
        Vec3 velocityDiff = target->getVel() - npc->getVel();
        steering->linear = velocityDiff / timeToTarget;

        // Check if the acceleration is too fast
        if (VMath::mag(steering->linear) > npc->getMaxAcceleration()) {
            steering->linear = VMath::normalize(steering->linear) * npc->getMaxAcceleration();
        }

        // No angular acceleration for this behavior
        steering->angular = 0;
        return steering;
    }
};
