#ifndef GAMEENGINE_H
#define GAMEENGINE_H

typedef struct {
	int x;
	int y;
} Vector2;
typedef struct {
	Vector2 position;
	Vector2 localScale;
} Transform;
typedef struct {
	void (*sprite)(Vector2);
	Transform transform;
} Object;



enum GameState{
	lost = -1,
	paused = 0,
	play = 1,
};


//Vector2 newVector2(int, int);
Vector2 newVector2(int x, int y)
{
	Vector2 vect2;
	vect2.x = x;
	vect2.y = y;
	return vect2;
}

#endif