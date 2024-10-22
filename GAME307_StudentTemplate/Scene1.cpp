#include "Scene1.h"


Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 30.0f;
	yAxis = 30.0f;

}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);


	{
		SDL_Surface* image;
		SDL_Texture* texture;
		image = IMG_Load("pacman.png");
		texture = SDL_CreateTextureFromSurface(renderer, image);
		game->getPlayer()->setImage(image);
		game->getPlayer()->setTexture(texture);
	}

	// Set up characters, choose good values for the constructor
	// or use the defaults, like this
	for (int i = 0; i < numberOfBoids; i++) {
		Character* boid = new Character();
		boid->OnCreate(this);
		boid->setTextureWith("Blinky.png");
		//boid.ma
		// set locations

		
		/*int randX = rand() % int(xAxis - 1);
		int randY = rand() % int(yAxis - 1);
		*/
		float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / xAxis));
		float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / yAxis));
		//boid->getBody()->setPos(Vec3(randX, randY, 15));
		boid->getBody()->setPos(Vec3(r1, r2,15));
		//boid->getBody().
		boids.push_back(boid);
	}

	return true;
}

void Scene1::OnDestroy() 
{
	for (Character* boid : boids) {
		boid->OnDestroy();
		boid = nullptr;
	}
	boids.clear();
}

void Scene1::Update(const float deltaTime) {
	// Calculate and apply any steering for npc's
	//blinky->bo angular = 1.0f;
	for (Character* boid : boids) {
		boid->Update(deltaTime);
	}
	// Update player
	game->getPlayer()->Update(deltaTime);
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// render any npc's
	for (Character* boid : boids) {
		boid->render(0.15f);
	}

	// render the player
	game->RenderPlayer(0.10f);

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to npc's as needed

	// send events to player as needed
	for (auto boid : boids) {
		boid->HandleEvents(event);
	}
	game->getPlayer()->HandleEvents(event);
}

std::vector<Character*> Scene1::getCharacters() {
	return boids;
}
