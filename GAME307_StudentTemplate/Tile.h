#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Vector.h"
#include "Scene.h"

class Tile {
	float width, height;
	Uint8 r, g, b, a;
	Vec3 pos;
	Scene* scene;

	void setColor(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}
public:
	Tile(Vec3 pos_, float width_, float height_, Scene* scene_) {
		pos = pos_;
		width = width_;
		height = height_;
		scene = scene_;
	}

	void render() {
		auto renderer = scene->game->getRenderer();
		auto projectionMatrix = scene->getProjectionMatrix();

		SDL_Rect rect;
		Vec3 topLeft = Vec3(pos.x - 0.5f*width, pos.y + 0.5f*height, 0.0f);
		Vec3 bottomRight = Vec3(pos.x + 0.5f * width, pos.y - 0.5f * height, 0.0f);

		Vec3 topLeftScreen = projectionMatrix * topLeft;
		Vec3 bottomRightScreen = projectionMatrix * bottomRight;

		rect = {
			static_cast<int>(topLeftScreen.x),
			static_cast<int>(topLeftScreen.y),
			static_cast<int>(bottomRightScreen.x),
			static_cast<int>(bottomRightScreen.y),
		};
		setColor(51, 123, 138, 255);

		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderFillRect(renderer, &rect);

		// make a boarder
		//SDL_SetRenderDrawColor();

	}
};