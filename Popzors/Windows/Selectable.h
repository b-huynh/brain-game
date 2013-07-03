#pragma once

#include "PolycodeView.h"
#include "Polycode3DPhysics.h"
#include "Polycode.h"

class Selectable {
public:
	static const int TYPE_POPPY = 0;
	static const int TYPE_POT = 1;

	virtual int getType() = 0;
	virtual bool hasEntity(SceneEntity * entity) = 0;
};

