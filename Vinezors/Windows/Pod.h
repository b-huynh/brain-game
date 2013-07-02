#include "Polycode3DPhysics.h"
#include "Polycode.h"

using namespace Polycode;

// These are objects which are attached to the walls and may act as hints, boosters, or penalties.
// They are comprised of a stem (cylinder) and a head (sphere)
class Pod
{
private:
	ScenePrimitive *stem;
	ScenePrimitive *head;
public:
	Pod();
	Pod(CollisionScene *scene, Vector3 base, Vector3 tip, Number stemRadius, Number headRadius);
	
	void move(Vector3 delta);

	void addToCollisionScene(CollisionScene *scene);

	void removeFromCollisionScene(CollisionScene * scene);
};