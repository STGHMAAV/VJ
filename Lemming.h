#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setMapMask(VariableTexture *mapMask);
	int getTipus();
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DYING_STATE, DEATH_STATE, DIG_STATE, BASH_LEFT_STATE,
		BASH_RIGHT_STATE, CLIMB_STATE, BUILD_LEFT_STATE, BUILD_RIGHT_STATE, EXPLODE_STATE, BLOCK_STATE, COLOR_BLOCK_STATE
	};
	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	int tipusLemming;

};


#endif // _LEMMING_INCLUDE


