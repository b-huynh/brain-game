#include "Selectable.h"

void Selectable::reset()
{
    Object::reset();
    selectable = true;
}

void Selectable::update(double elapsed)
{
    Object::update(elapsed);
}