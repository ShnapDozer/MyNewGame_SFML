#pragma once


#include <SFML/Graphics.hpp>

#include <Box2D/Box2D.h>

#include <iostream>
#include <String> 
#include <vector>

using namespace sf;


class Animation
{
	std::string mirror = "flip";
	std::map<int, std::string > numList =
	{
		{0, "0.png"},
		{1, "1.png"},
		{2, "2.png"},
		{3, "3.png"},
		{4, "4.png"},
		{5, "5.png"},
		{6, "6.png"},
		{7, "7.png"},
		{8, "8.png"},
		{9, "9.png"},
		{10, "10.png"},
	};

public:

	int framov;
	float speed, currentf;
	bool isplay;


	Sprite sprite;

	std::vector <Texture> TextureList;

	Animation()
	{
		isplay = true;
		currentf = 0;
	}
	Animation(std::string file, float speeed, int frameov)
	{
		for (int i = 0; i < frameov; i++)
		{
			Texture t;

			t.loadFromFile(file + numList[i]);

			TextureList.push_back(t);
		}
		speed = speeed;
		framov = frameov;
		currentf = 0;
		isplay = true;
	}
	void tik(float time)
	{
		if (!isplay)return;
		currentf += speed * time;
		if (currentf > framov)currentf = 0;
	}
	Sprite CurSprtSet(b2Vec2 posA = { 0,0 }, float angle = 0)
	{
		const float SCALE = 30.f;
		const float DEG = 57.29577f;

		sprite.setTexture(TextureList[currentf]);
		sprite.setOrigin(sprite.getGlobalBounds().height / 2, sprite.getGlobalBounds().width / 2);
		sprite.setPosition(posA.x*SCALE, posA.y*SCALE);
		sprite.setRotation(angle*DEG);
		return sprite;
	}
};


class AnimManager
{
	bool isplay;


public:
	std::string currentAnim;
	std::string Name;
	std::map<std::string, Animation> animList;


	AnimManager(std::string name) { Name = name; }

	void create(std::string name, std::string file, float speed, int frameov)
	{
		Animation Anim(file, speed/100, frameov);
		animList[name] = Anim;
		currentAnim = name;
	}

	void tick(float time)
	{
		if (animList.size() == 0) return;
		animList[currentAnim].tik(time);
	}

	void set(std::string name)
	{
		currentAnim = name;
	}

	void draw(RenderWindow &window, b2Vec2 posA = { 0,0 }, float angle = 0)
	{
		if (animList.size() == 0) return;
		window.draw(animList[currentAnim].CurSprtSet(posA, angle));
	}


	/*
	void flip(bool b = 1) { animList[currentAnim].flip = b; }

	void pause() { animList[currentAnim].isplay = false; }

	void play() { animList[currentAnim].isplay = true; }

	void play(std::string name) { animList[name].isplay = true; }

	bool isPlaying() { return animList[currentAnim].isplay; }*/

};


