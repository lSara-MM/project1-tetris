#ifndef __POINTS_H__
#define __POINTS_H__

#include "Module.h"
#include "Animation.h"
#include "ModuleWindow.h"


class Points : public Module
{
public:
	//Constructor
	Points(bool startEnabled);
	//Destructor
	~Points();

	// Called when the module is activated
	// Loads the necessary textures for the map backgroun
	bool Start() override;

	void calcScore(int value);
	int addCreditsStart();
	int addCreditsLvl();

	void RainbowStack();

	void Reset();

public:

	// Credit (coins to add 0-9)
	int credits;
	uint fxAdd_Credits = 0;

	// points
	int p_drop, p_stack, h, value;		// p_stack = rainbow stack 
	int score;

	int lines;
	int lvl;
};

#endif