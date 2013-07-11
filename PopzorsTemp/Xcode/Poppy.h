#pragma once

#include "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"
#include "Selectable.h"

class Poppy : public Selectable {
public:
	~Poppy();
	Poppy(Vector3 pos = Vector3(0,0,0), Color color = Color(0,0,0,0), 
		  Color blinkColor = Color(0,0,0,0), Number blinktime = 0);

	void addToCollisionScene(CollisionScene * scene);
	void setColor(int r, int g, int b, int a);
	void setColor(Color color);
	void setPosition(Number x, Number y, Number z);
	void setPosition(Vector3 vec);

	int getType() { return Selectable::TYPE_POPPY; }
	bool hasEntity(SceneEntity * entity);

	bool blink(Number elapsed);
	void unblink();

private:
	Color color;
	Vector3 pos;

	Number timeBlinked;
	Color blinkColor;

	ScenePrimitive * body;
};
