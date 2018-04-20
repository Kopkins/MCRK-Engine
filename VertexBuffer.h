#ifndef VertexBuffer_h
#define VertexBuffer_h

#include <GL/glew.h>
#include <vector>

class VertexBuffer {

public:

VertexBuffer();

void addFloat(float added);

float* data();

std::vector<float> getVector();

unsigned int size();

void setData(std::vector<float> vertices);

GLuint getVbo();

private:

std::vector<float> m_vertexData;
GLuint	m_vbo;

};

#endif
