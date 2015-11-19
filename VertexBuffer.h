#ifndef VertexBuffer_h
#define VertexBuffer_h

#include <cstdlib>
#include <GL/glew.h>

class VertexBuffer{

std::vector<float> m_vertexData;
GLuint	m_vbo;


public:

VertexBuffer();

void addFloat(float added);

float* data();

std::vector<float> getVector();

unsigned int size();

void setData(std::vector<float> vertices);

GLuint getVbo();

};

#endif
