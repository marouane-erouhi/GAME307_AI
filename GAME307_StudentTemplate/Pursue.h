#pragma once
#include "Seek.h"

class Pursue : public Seek {
public:
    float maxPrediction;
    const Body* target; // Overrides Seek::target

    Pursue(const Body* npc_, const Body* target_, float maxPrediction_) 
        : Seek(npc_, target), target(target_), maxPrediction(maxPrediction_) {
    }

    SteeringOutput* getSteering() override {
        // 1. Calculate the target to delegate to Seek

        // Work out the distance to the target
        Vec3 direction = target->getPos() - npc->getPos();
        float distance = VMath::mag(direction);

        // Work out our current speed
        float speed = VMath::mag(npc->getVel());

        // Check if speed is too small to give a reasonable prediction time
        float prediction;
        if (speed <= distance / maxPrediction) {
            prediction = maxPrediction;
        } else {
            prediction = distance / speed;
        }

        // Put the predicted target position together
        explicitTarget = *target;
        explicitTarget.getPos() += target->getVel() * prediction;

        // 2. Delegate to Seek
        this->Seek::target = &explicitTarget;
        return Seek::getSteering();
    }

private:
    Body explicitTarget; // Used for the predicted target
};
