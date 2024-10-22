#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

#include "Scene.h"
#include "KinematicBody.h"

using namespace std;

class Character
{
private:
	class KinematicBody* body;
	class Scene* scene;

	float sep_thresh = 0.7f;
	float sep_weight = 1.0f;
	float all_weight = 1.0f;
	float coh_weight = 1.0f;

public:
	Character()
	{
		body = NULL;
		scene = NULL;
	};

	~Character() {};

	bool OnCreate(Scene* scene_);
	void OnDestroy();
	bool setTextureWith(string file);
	void Update(float time);
	void HandleEvents(const SDL_Event& event);
	void render(float scale = 1.0f) const;
	Body* getBody() { return body; }
	void handleEdges();

};

#endif

