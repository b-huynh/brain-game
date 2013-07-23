#include "Stage.h"

// Simple inefficient algorithm that pushes poppies away from each other if they collide
void Stage::handlePoppyCollisions(Number elapsed)
{
    for (int i = 0; i < poppies.size(); ++i)
        for (int j = i + 1; j < poppies.size(); ++j)
        {
            poppies[i]->handleCollision(elapsed, scene, poppies[j]);
        }
}

void Stage::handlePotCollisions(Number elapsed)
{
    for (int i = 0; i < pots.size(); ++i)
        for (int j = i + 1; j < pots.size(); ++j)
        {
            pots[i]->handleCollision(elapsed, scene, pots[j]);
        }
}

// Returns input data for clicking
ClickedResult Stage::getClicked(Vector3 origin, Vector3 dest)
{
	ClickedResult res;
	res.poppy = NULL;
    res.pot = NULL;
	RayTestResult ray = scene->getFirstEntityInRay(origin, dest);
	for (int i = 0; i < poppies.size(); ++i) {
		if (poppies[i]->hasEntity(ray.entity)) {
            res.poppy = poppies[i];
		}
	}
    
	for (int i = 0; i < pots.size(); ++i)
		if (pots[i]->hasEntity(ray.entity))
            res.pot = pots[i];
    
	return res;
}
