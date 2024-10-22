#include "Character.h"
#include <Seek.h>
#include <Flee.h>
#include <Arrive.h>
#include <Align.h>
#include <VelocityMatching.h>
#include <Pursue.h>
#include <Scene1.h>
#include "Seperation.h"
#include "Cohesion.h"

MATH::Vec3 randomVec() {
	float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	return VMath::normalize(Vec3(
		static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)),
		static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)),
		static_cast <float> (rand()) / (static_cast <float> (RAND_MAX))
	));
}

bool Character::OnCreate(Scene* scene_)
{
	scene = scene_;

	// Configure and instantiate the body to use for the demo
	if (!body)
	{
		float radius = 0.2;
		float orientation = 0.0f;
		float rotation = 0.0f;
		float angular = 0.0f;
		float maxSpeed = 4.0f;
		float maxAcceleration = 10.0f;
		float maxRotation = 2.0f;
		float maxAngular = 10.0f;
		body = new KinematicBody(
			Vec3(1.0f, 5.0f, 0), randomVec(), Vec3(0, 0, 0), 1.0f,
			radius,
			orientation,
			rotation,
			angular,
			maxSpeed,
			maxAcceleration,
			maxRotation,
			maxAngular
		);
	}

	if (!body)
	{
		return false;
	}

	return true;
}

void Character::OnDestroy()
{
	if (body)
	{
		if (body->getTexture())
		{
			SDL_DestroyTexture(body->getTexture());
		}
		delete body;
	}
};

bool Character::setTextureWith(string file)
{
	SDL_Surface* image = IMG_Load(file.c_str());
	if (image == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}
	SDL_Window* window = scene->getWindow();
	SDL_Renderer* renderer = SDL_GetRenderer(window);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	if (!texture) 
	{
		std::cerr << "Can't create texture" << std::endl;
		return false;
	}
	if (body->getTexture()) SDL_DestroyTexture(body->getTexture());
	body->setTexture(texture);
	SDL_FreeSurface(image);
	return true;
}

void Character::Update(float deltaTime)
{
	// create a new overall steering output
	SteeringOutput* steering;
	steering = NULL;

	//body->setAngular(50.0f);
	// set the target for steering; target is used by the steerTo... functions
	// (often the target is the Player)

	// Arrive example
	//SteeringBehaviour* steering_algo = new Arrive(body, scene->game->getPlayer(), 1.0f,3.0f, 0.1f);

	//SteeringBehaviour* steering_algo = new Align(body, scene->game->getPlayer(), 0.50f, 1.0f, 0.1f);

	Scene1* scene1 = ((Scene1*)scene);

	auto boids = scene1->getCharacters();
	auto player = scene1->game->getPlayer();
	std::vector<const Body*> bodies;
	for (auto boid : boids) {
		if (boid->getBody() == body)	continue;
		bodies.push_back(boid->getBody());
	}

	//SteeringBehaviour* steering_algo = new VelocityMatch(body, scene->game->getPlayer());
	/*SteeringBehaviour* steering_algo = new Separation(body, bodies, 2.0f, 0.5f);
	steering = steering_algo->getSteering();*/

	for (auto target : bodies) {
		Separation* seperation = new Separation(body, target, sep_thresh, 0.001f);
		body->Update(deltaTime, seperation->getSteering());

		Alignment* alignment = new Alignment(body, target, 1.0f, 10.0f);
		body->Update(deltaTime, alignment->getSteering());

		Cohesion* cohesion = new Cohesion(body,target, coh_weight);
		body->Update(deltaTime, cohesion->getSteering());

		delete seperation;
		delete alignment;
		delete cohesion;
	}

	//SteeringOutput* combined = new SteeringOutput();
	//combined->linear +=		seperationSteering->linear ;//* 0.33;
	//combined->linear +=		alignmentSteering->linear ;//* 0.33;
	//combined->linear +=		cohesionSteering->linear ;//* 0.33;

	//body->Update(deltaTime, combined);


	delete steering;
	handleEdges();
}

void Character::HandleEvents(const SDL_Event& event)
{
	// handle events here, if needed
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_Y:
			//sep_thresh, sep_weight
			sep_thresh += 0.1f;
			printf("%f\n",sep_thresh);
			break;
		case SDL_SCANCODE_U:
			//sep_thresh, sep_weight
			sep_weight += 1.1f;
			printf("%f\n", sep_weight);
			break;
		case SDL_SCANCODE_I:
			//sep_thresh, sep_weight
			all_weight += 0.1f;
			printf("%f\n", all_weight);
			break;
		case SDL_SCANCODE_O:
			//sep_thresh, sep_weight
			coh_weight += 0.01f;
			printf("%f\n", coh_weight);
			break;


		}
	}
}

void Character::render(float scale) const
{
	SDL_Renderer* renderer = scene->game->getRenderer();
	Matrix4 projectionMatrix = scene->getProjectionMatrix();

	SDL_Rect square;
	Vec3 screenCoords;
	int    w, h;

	// notice use of "body" in the following
	SDL_QueryTexture(body->getTexture(), nullptr, nullptr, &w, &h);
	w = static_cast<int>(w * scale);
	h = static_cast<int>(h * scale);
	screenCoords = projectionMatrix * body->getPos();
	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = w;
	square.h = h;

	// Convert character orientation from radians to degrees.
	float orientation = body->getOrientation() * 180.0f / M_PI;

	SDL_RenderCopyEx(renderer, body->getTexture(), nullptr, &square,
		orientation, nullptr, SDL_FLIP_NONE);
}

void Character::handleEdges() {
	Vec3 pos = body->getPos();
	if (pos.x > scene->getxAxis()) {
		pos.x = 0;
	}
	if (pos.x < 0) {
		pos.x = scene->getxAxis();
	}
	if (pos.y > scene->getyAxis()) {
		pos.y = 0;
	}
	if (pos.y < 0) {
		pos.y = scene->getyAxis();
	}
	body->setPos(Vec3(pos));
}
