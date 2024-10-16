#pragma once
#include "Body.h"
#include <vector>

struct Boid {
	Vec3 pos;
	Vec3 vel;
	Vec3 acc;

	Vec3 align(std::vector<Boid*> boids);

};