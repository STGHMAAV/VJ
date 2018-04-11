#include "Level.h"



Level::Level()
{
}


Level::~Level()
{
}

void Level::setValues() {
	maxPikmins = 50;
	LevelTextureLocation = "images/Levels/fun1.png";
	LevelMaskLocation = "images/Levels/fun1_mask.png"; 
}

void Level::render() {
	Scene::render();
}
