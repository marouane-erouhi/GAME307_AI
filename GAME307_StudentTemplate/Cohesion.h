#pragma once
#include "Vector.h"
#include "SteeringBehaviour.h"
#include <vector>


class Cohesion : public SteeringBehaviour {
private:
    std::vector<const Body*> neighbors; // List of nearby boids
    float weight; // Steering weight
    const Body* target;
    float radius = 3.0f;

public:
    Cohesion(const Body* npc_, const Body* target_, float weight_) : weight(weight_) {
        npc = npc_;
        target = target_;
    }

    void setNeighbors(const std::vector<const Body*>& neighbors_) {
        neighbors = neighbors_;
    }

    SteeringOutput* getSteering() override {
        Vec3 dir = npc->getPos() - target->getPos();
        float dist = VMath::mag(dir);
        // too far to care
        if (dist > radius)   return result;

        result->linear = dir * weight;

        return result;
    }
};
