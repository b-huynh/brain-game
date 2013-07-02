#include <vector>
#include "Direction.h"
#include "Vine.h"

using namespace std;

class Player
{
private:
	string name;
	bool mouseLeft;
	bool keyUp;
	bool keyDown;
	bool keyLeft;
	bool keyRight;

	vector<Vine *> vines;
	
	Direction dir;
	Vector2 mousePos;
	Vector3 camPos;
public:
	Player();
	Player(const string & name, Vector3 camPos);
	
	bool getMouseLeft() const;
	Direction getDir() const;
	Vector2 getMousePos() const;
	Vector3 getCamPos() const;

	void setMouseLeft(bool value);
	void setKeyUp(bool value);
	void setKeyDown(bool value);
	void setKeyLeft(bool value);
	void setKeyRight(bool value);
	
	void setDir(Direction value);
	void setMousePos(Vector2 value);
	void setCamPos(Vector3 value);
	
	void move(Vector3 delta);

	void addVine(Vine *vine);

	void update(Number elapsed);
};