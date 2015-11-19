#ifndef keybuffer_h
#define keybuffer_h

#include <cstdlib>
#include <bitset>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class KeyBuffer{

std::bitset<GLFW_KEY_LAST> m_keys;

public:

KeyBuffer();

void setKey(GLuint key, GLuint action);

std::bitset<GLFW_KEY_LAST> getBuffer();
	
};
#endif
