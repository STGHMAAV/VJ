#pragma once
#include "Scene.h"

#include "PowersBar.h"

class Level :	public Scene
{
public:
	Level();
	~Level();

	void init();
	void setValues(); 
	void render(); 
	void update(int deltaTime);


	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void mouseRelease(int button);

	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);

public: 
	vector<Lemming> lemmings;
	int maxPikmins;
	vector <int> actualment{ 0,0,0,0 };
	int survived;
	int Time;
	Lemming lemming;
	glm::vec2 spawnPoint;
	glm::vec2 exitPoint;
	bool mapPressed;
	PowersBar powersBar;
};

