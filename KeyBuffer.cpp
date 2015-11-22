#include "KeyBuffer.h"

KeyBuffer::KeyBuffer():
m_keys(0)
{
	
}

void KeyBuffer::setKey(GLuint key, GLuint action){
	m_keys[key]=action;
}

std::bitset<GLFW_KEY_LAST> KeyBuffer::getBuffer(){
	return m_keys;
}
