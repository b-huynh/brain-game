#pragma once

#include "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"
#include "Selectable.h"

class Pot : public Selectable {
public: 
	Pot(Vector3 pos = Vector3(0,0,0));

	int getType() { return Selectable::TYPE_POT; }
	bool hasEntity(SceneEntity * entity);

	void setColor(int r, int g, int b, int a);
	void setColor(Color color);
	void setPosition(Number x, Number y, Number z);
	void setPosition(Vector3 vec);
	Vector3 getPosition() { return pos; }
	void addToCollisionScene(CollisionScene * scene);

private:
	Vector3 pos;
	ScenePrimitive * body;
};