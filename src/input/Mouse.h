#pragma once
#include <glm/glm.hpp>

class Mouse
{
public:
	static const glm::vec2& getMousePosition();
	static void setMousePosition(float x, float y);
};
