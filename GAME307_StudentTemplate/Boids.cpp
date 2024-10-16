#include "Boids.h"

Vec3 Boid::align(std::vector<Boid*> boids) {
	float perception = 5.0f;

	Vec3 steering = Vec3();

	int count = 0;
	for (Boid* other : boids) {
		float dist = VMath::distance(pos, other->pos);
		if (dist > perception && other == this)	continue;
		steering += other->vel;
		count++;
	}

	if (count > 0) {
		steering /= count;
		steering -= vel;
	}

	return steering;
}
