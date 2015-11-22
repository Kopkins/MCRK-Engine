#include <vector>
#include <GL/glew.h>
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer () :
    m_vertexData (), m_vbo()
{
  glGenBuffers(1,&m_vbo);
}

void
VertexBuffer::addFloat (float added)
{
  m_vertexData.push_back (added);
}

float*
VertexBuffer::data ()
{
  return m_vertexData.data ();
}

std::vector<float>
VertexBuffer::getVector(){
  return m_vertexData;
}

unsigned int
VertexBuffer::size ()
{
  return m_vertexData.size ();
}

GLuint
VertexBuffer::getVbo(){
  return m_vbo;
}

void
VertexBuffer::setData(std::vector<float> vertices){

}






