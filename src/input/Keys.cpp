#include "Keys.h"

static bool keys[4]{ false, false, false, false };

bool Keys::getKey(char key)
{
	switch (key)
	{
	case 'W': return keys[0]; break;
	case 'A': return keys[1]; break;
	case 'S': return keys[2]; break;
	case 'D': return keys[3]; break;
	}
	return false;
}

void Keys::setKey(char key, bool pressed)
{
	switch (key)
	{
	case 'W': keys[0] = pressed; break;
	case 'A': keys[1] = pressed; break;
	case 'S': keys[2] = pressed; break;
	case 'D': keys[3] = pressed; break;
	}
}
