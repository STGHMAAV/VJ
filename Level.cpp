#include "Level.h"



Level::Level()
{
}


Level::~Level()
{
}

void Level::init()
{
	setValues();
	Scene::init(); 
	lemming.init(glm::vec2(60, 30), simpleTexProgram);
	++actualment[lemming.getTipus()];
	lemming.setMapMask(&maskTexture);
	lemmings.push_back(lemming);


	powersBar.init();
}

void Level::setValues() {
	maxPikmins = 50;
	LevelTextureLocation = "images/Levels/fun1.png";
	LevelMaskLocation = "images/Levels/fun1_mask.png"; 
	spawnPoint = glm::vec2(60,30); 
	exitPoint = glm::vec2(60, 80);
	Time = 650;
	survived = 0;
	maxPikmins = 50;
}

void Level::initShaders()
{
	Scene::initShaders(); 
}

void Level::eraseMask(int mouseX, int mouseY)
{
	Scene::eraseMask(mouseX, mouseY);
}

void Level::applyMask(int mouseX, int mouseY)
{
	applyMask(mouseX, mouseY);
}

void Level::render() {
	Scene::render();

	powersBar.render();
	for (unsigned int i = 0; i < lemmings.size(); ++i)
		lemmings[i].render();
}

void Level::update(int deltaTime)
{
	Scene::update(deltaTime);
	for (unsigned int i = 0; i < lemmings.size(); ++i)
		lemmings[i].update(deltaTime);
}

void Level::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	mapPressed = powersBar.mouseMoved(mouseX, mouseY, bLeftButton);
}

void Level::mouseRelease(int button)
{
	powersBar.mouseRelease(button);
}
