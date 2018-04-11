#pragma once
#include "Scene.h"
class Level :	public Scene
{
public:
	Level();
	~Level();

	void setValues(); 
	void render(); 
};

