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

class Alignment : public SteeringBehaviour {
private:
    std::vector<const Body*> neighbors;
    const Body* target;
    float weight;
    float radius;
    float orientationTolerance = 0.1f;
    float rotationSlowRadius = 0.3f;
    float timeToTarget = 0.1f;
public:
    Alignment(const Body* npc_, const Body* target_, float weight_,
        float radius_, float orientationTolerance_ = 0.1f, float rotationSlowRadius_ = 0.3f, float timeToTarget_ = 0.1f
    ) :
        weight(weight_), radius(radius_),
        orientationTolerance(orientationTolerance_),
        rotationSlowRadius(rotationSlowRadius_),
        timeToTarget(timeToTarget_) {
        npc = npc_;
        target = target_;
    }

    void setNeighbors(const std::vector<const Body*>& neighbors_) {
        neighbors = neighbors_;
    }

    SteeringOutput* getSteering() override {
        Vec3 dir = npc->getPos() - target->getPos();
        float dist = VMath::mag(dir);

        // trget is outside the view of this npc
        //if (dist > radius) return result;

        float rotation = mapToRange(target->getOrientation() - npc->getOrientation());
        float rotationSize = fabs(rotation);

        // prevent rotation if close enouph
        if (rotationSize < orientationTolerance) return result;

        // slow the rotation speed when close to target rotation
        float targetRotation;
        if (rotationSize > rotationSlowRadius) {
            targetRotation = npc->getMaxRotation();
        } else {
            targetRotation = npc->getMaxRotation() * (rotationSize / rotationSlowRadius);
        }

        result->angular = (targetRotation * rotation / rotationSize - npc->getAngular()) / timeToTarget;

        result->linear = Vec3();
        return result;
    }
};



//
//double mapToRange(double r) {
//    r = fmod(r, 2 * M_PI);
//    if (r < -M_PI) {
//        r += 2 * M_PI;
//    }
//    if (r >= M_PI) {
//        r -= 2 * M_PI;
//    }
//    return r;
//}
//
////class Align : public SteeringBehaviour {
////    const Body* target;
////
////    float targetRadius;
////    float slowRadius;
////    float timeToTarget = 0.1;
////public:
////    Align(const Body* npc_, const Body* target_, float targetRadius_, float slowRadius_, float timeToTraget_);
////    ~Align();
////
////    SteeringOutput* getSteering();
////
////};
//
//class Align : public SteeringBehaviour {
//public:
//    const Body* target;
//
//    float targetRadius;
//    float slowRadius;
//    float timeToTarget = 0.1f;
//
//    Align(const Body* npc_, const Body* target_, float targetRadius_, float slowRadius_, float timeToTarget_) 
//        : target(target_), targetRadius(targetRadius_), slowRadius(slowRadius_), timeToTarget(timeToTarget_){
//        npc = npc_;
//    }
//
//    SteeringOutput* getSteering() override {
//        float maxAngularAcceleration = npc->getMaxAcceleration();
//        float maxRotation = npc->getMaxRotation();
//
//        // Create the structure to hold our output
//        SteeringOutput* steering = new SteeringOutput();
//
//        // Get the naive direction to the target
//        float rotation = target->getOrientation() - npc->getOrientation();
//
//        rotation = mapToRange(rotation);
//
//        float rotationSize = fabs(rotation);
//
//        // Check if we are there, return no steering
//        if (rotationSize < targetRadius) {
//            delete steering;
//            return nullptr;
//        }
//
//        // If we are outside the slowRadius, use maximum rotation
//        float targetRotation;
//        if (rotationSize > slowRadius) {
//            targetRotation = maxRotation;
//        } else {
//            // Otherwise calculate a scaled rotation
//            targetRotation = maxRotation * rotationSize / slowRadius;
//        }
//
//        // The final target rotation combines speed and direction
//        targetRotation *= rotation / rotationSize;
//
//        // Acceleration tries to get to the target rotation
//        steering->angular = (targetRotation - npc->getRotation()) / timeToTarget;
//
//        // Check if the acceleration is too great
//        if (fabs(steering->angular) > maxAngularAcceleration) {
//            steering->angular = (steering->angular / fabs(steering->angular)) * maxAngularAcceleration;
//        }
//
//        // Output the steering, no linear acceleration
//        steering->linear = Vec3(50,0.0,0.0);
//        return steering;
//    }
//};
