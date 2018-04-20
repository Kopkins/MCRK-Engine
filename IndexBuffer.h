#ifndef IndexBuffer_h
#define IndexBuffer_h

#include <GL/glew.h>
#include <vector>

class IndexBuffer {

public:

IndexBuffer();

void add(unsigned int added);

unsigned int* data();

std::vector<unsigned int> getVector();

unsigned int size();

void setData(std::vector<unsigned int> vertices);

GLuint getVbo();

private:

std::vector<unsigned int> m_vertexData;
GLuint	m_ibo;

};

#endif
