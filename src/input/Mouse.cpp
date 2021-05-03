#include "Mouse.h"
#pragma once

static glm::vec2 m_MousePosition;

const glm::vec2& Mouse::getMousePosition()
{
	return m_MousePosition;
}

void Mouse::setMousePosition(float x, float y)
{
	m_MousePosition.x = x;
	m_MousePosition.y = y;
}
