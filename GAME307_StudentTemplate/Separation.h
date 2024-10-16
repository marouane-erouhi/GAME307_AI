#pragma once
#include "SteeringBehaviour.h"
#include <vector>

class Separation : public SteeringBehaviour {
public:
    std::vector<const Body*> targets;
    float threshold;
    float decayCoefficient;

    Separation(const Body* npc_, const std::vector<const Body*> targets_, float threshold_, float decayCoefficient_)
        : threshold(threshold_), decayCoefficient(decayCoefficient_){
        npc = npc_;
        targets = targets_;
    }

    SteeringOutput* getSteering() override {
        // The steering variable holds the output
        SteeringOutput* steering = new SteeringOutput();
        steering->linear = Vec3();

        // Loop through each target
        for (const Body* target : targets) {
            // Check if the target is close
            Vec3 direction = target->getPos() - npc->getPos();
            float distance = VMath::mag(direction);

            if (distance < threshold) {
                // Calculate the strength of repulsion
                float strength = std::min(decayCoefficient / (distance * distance), npc->getMaxAcceleration());

                // Add the acceleration
                direction = VMath::normalize(direction);
                steering->linear += strength * direction;
            }
        }

        // No angular acceleration for separation behavior
        steering->angular = 0;
        return steering;
    }
};
