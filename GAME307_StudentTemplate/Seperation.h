#pragma once
#include "Vector.h"
#include "SteeringBehaviour.h"
#include <vector>

class Separation : public SteeringBehaviour {
private:
    std::vector<const Body*> neighbors; // List of nearby boids
    const Body* target;
    float threshold; // Distance threshold for separation
    float weight; // Steering weight
    float radius = 3.0f;

public:
    Separation(const Body* npc_, const Body* target_, float threshold_, float weight_)
        :
        threshold(threshold_), weight(weight_), target(target_) {
        npc = npc_;
    }

    void setNeighbors(const std::vector<const Body*>& neighbors_) {
        neighbors = neighbors_;
    }

    SteeringOutput* getSteering() override {
        Vec3 dir = npc->getPos() - target->getPos();
        float dist = VMath::mag(dir);

        // too far to care
        if (dist > radius)   return result;

        result->linear = dir * (1 / (dist * dist));

        // check if seperation for is too high
        float mag = VMath::mag(result->linear);
        if (mag > npc->getMaxAcceleration()) {
            result->linear = VMath::normalize(result->linear) * npc->getMaxAcceleration();
        }

        result->linear *= weight;
        return result;
    }
};
