#pragma once
#include "SteeringBehaviour.h"

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

//class Align : public SteeringBehaviour {
//    const Body* target;
//
//    float targetRadius;
//    float slowRadius;
//    float timeToTarget = 0.1;
//public:
//    Align(const Body* npc_, const Body* target_, float targetRadius_, float slowRadius_, float timeToTraget_);
//    ~Align();
//
//    SteeringOutput* getSteering();
//
//};

class Align : public SteeringBehaviour {
public:
    const Body* target;

    float targetRadius;
    float slowRadius;
    float timeToTarget = 0.1f;

    Align(const Body* npc_, const Body* target_, float targetRadius_, float slowRadius_, float timeToTarget_) 
        : target(target_), targetRadius(targetRadius_), slowRadius(slowRadius_), timeToTarget(timeToTarget_){
        npc = npc_;
    }

    SteeringOutput* getSteering() override {
        float maxAngularAcceleration = npc->getMaxAcceleration();
        float maxRotation = npc->getMaxRotation();

        // Create the structure to hold our output
        SteeringOutput* steering = new SteeringOutput();

        // Get the naive direction to the target
        float rotation = target->getOrientation() - npc->getOrientation();

        rotation = mapToRange(rotation);

        float rotationSize = fabs(rotation);

        // Check if we are there, return no steering
        if (rotationSize < targetRadius) {
            delete steering;
            return nullptr;
        }

        // If we are outside the slowRadius, use maximum rotation
        float targetRotation;
        if (rotationSize > slowRadius) {
            targetRotation = maxRotation;
        } else {
            // Otherwise calculate a scaled rotation
            targetRotation = maxRotation * rotationSize / slowRadius;
        }

        // The final target rotation combines speed and direction
        targetRotation *= rotation / rotationSize;

        // Acceleration tries to get to the target rotation
        steering->angular = (targetRotation - npc->getRotation()) / timeToTarget;

        // Check if the acceleration is too great
        if (fabs(steering->angular) > maxAngularAcceleration) {
            steering->angular = (steering->angular / fabs(steering->angular)) * maxAngularAcceleration;
        }

        // Output the steering, no linear acceleration
        steering->linear = Vec3(50,0.0,0.0);
        return steering;
    }
};
