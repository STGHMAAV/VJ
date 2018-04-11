#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, FALLING_LEFT, FALLING_RIGHT, DYING, DEATH, DIG, BASH_LEFT, BASH_RIGHT, CLIMB, BUILD_LEFT, BUILD_RIGHT, EXPLODE, BLOCK
};

void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/Pikmin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125/2, 0.25/2), &spritesheet, &shaderProgram);
	tipusLemming = rand()%4;
	sprite->setNumberAnimations(12);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 16, tipusLemming/4.0 + 0.0f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 16, tipusLemming /4.0 +  0.125f));

		sprite->setAnimationSpeed(FALLING_LEFT, 12);
		sprite->addKeyframe(FALLING_LEFT, glm::vec2(0.5, tipusLemming / 4.0 ));

		sprite->setAnimationSpeed(FALLING_RIGHT, 12);
		sprite->addKeyframe(FALLING_RIGHT, glm::vec2(0.5, tipusLemming / 4.0 ));

		sprite->setAnimationSpeed(DIG, 2);
		for (int i = 0; i < 2; i++)
			sprite->addKeyframe(EXPLODE, glm::vec2(0.875 + float(i) / 16.0, tipusLemming / 4.0));

		sprite->setAnimationSpeed(BASH_LEFT, 2);
		for (int i = 0; i < 2; i++)
			sprite->addKeyframe(EXPLODE, glm::vec2(0.625 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));

		sprite->setAnimationSpeed(BASH_RIGHT, 2);
		for (int i = 0; i < 2; i++)
			sprite->addKeyframe(EXPLODE, glm::vec2(0.625 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));

		sprite->setAnimationSpeed(CLIMB, 2);
		sprite->addKeyframe(EXPLODE, glm::vec2(0.625, tipusLemming / 4.0));

		sprite->setAnimationSpeed(BUILD_LEFT, 2);
		for (int i = 0; i < 2; i++)
			sprite->addKeyframe(EXPLODE, glm::vec2(0.5 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));

		sprite->setAnimationSpeed(BUILD_RIGHT, 2);
		for (int i = 0; i < 2; i++)
			sprite->addKeyframe(EXPLODE, glm::vec2(0.625 + float(i) / 16.0, tipusLemming / 4.0 + 0.125));


		sprite->setAnimationSpeed(EXPLODE, 2);
		for (int i = 0; i < 4; i++)
			sprite->addKeyframe(EXPLODE, glm::vec2(0.75 + float(i) / 16.0, tipusLemming / 4.0 ));

		sprite->setAnimationSpeed(DYING, 4);
		for (int i = 0; i < 4; i++)
			sprite->addKeyframe(DYING, glm::vec2(0.75+ float(i)/16.0, tipusLemming / 4.0 + 0.125));
		
		sprite->setAnimationSpeed(DEATH, 4);
		sprite->addKeyframe(DYING, glm::vec2(0.9375, tipusLemming / 4.0 + 0.125));
		
	sprite->changeAnimation(WALKING_RIGHT);
	sprite->setPosition(initialPosition);
}

void Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	switch(state)
	{
	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		if(fall > 0){
			sprite->position() += glm::vec2(0, fall);
			sprite->changeAnimation(FALLING_LEFT);
		}
		else {
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}
		break;
	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		if(fall > 0) {
			sprite->position() += glm::vec2(0, fall);
		    sprite->changeAnimation(FALLING_RIGHT);
		}
		else {
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
		break;
	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if(collision())
		{
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if(fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if(fall > 2)
				state = FALLING_LEFT_STATE;
		}
		break;
	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if(collision())
		{
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else
				state = FALLING_RIGHT_STATE;
		}
		break;
	case DIG_STATE:
		sprite->changeAnimation(DIG);
	case DYING_STATE:
		sprite->changeAnimation(DYING);
		sprite->changeAnimation(DEATH);
		state = DEATH_STATE;
		break; 
	case DEATH_STATE:
		//kill lemming, erase, unrender
		break; 
	}
}

void Lemming::render()
{
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

int Lemming::getTipus()
{
	return tipusLemming;
}







