#include <vector>
#include <GL/glew.h>
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer () :
    m_vertexData (), m_ibo()
{
  glGenBuffers(1,&m_ibo);
}

void
IndexBuffer::add (unsigned int added)
{
  m_vertexData.push_back (added);
}

unsigned int*
IndexBuffer::data ()
{
  return m_vertexData.data ();
}

std::vector<unsigned int>
IndexBuffer::getVector(){
  return m_vertexData;
}

unsigned int
IndexBuffer::size ()
{
  return m_vertexData.size ();
}

GLuint
IndexBuffer::getVbo(){
  return m_ibo;
}

void
IndexBuffer::setData(std::vector<unsigned int> vertices){

}
