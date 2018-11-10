#include "MathTools.h"

Vector2D getDirectionAsVector(float direction)
{
	float x = cos(direction);
	float y = sin(direction);

	return Vector2D(x, y);
}