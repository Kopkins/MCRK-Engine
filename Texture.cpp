#include <iostream>
#include <FreeImagePlus.h>
#include "Texture.h"

Texture::Texture ()
{
  glGenTextures (1, &m_textureId);
}

Texture::~Texture ()
{
  glDeleteTextures (1, &m_textureId);
}

void
Texture::setSWrapMode (GLint sWrapMode)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapMode);
  this->unbind ();
}

void
Texture::setTWrapMode (GLint tWrapMode)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapMode);
  this->unbind ();
}

void
Texture::setMinFilter (GLint minFilter)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  this->unbind ();
}

void
Texture::setMagFilter (GLint magFilter)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  this->unbind ();
}

void
Texture::setData (const std::string& textureFile)
{
  this->bind ();
  fipImage image;
  bool ok=image.load (textureFile.c_str ());

  if(!ok){
      std::cout<<"Failed to load texture from file\n";
  }

  unsigned int width = image.getWidth ();
  unsigned int height = image.getHeight ();
  const GLint level = 0;
  GLint internalFormat = GL_RGB;
  const GLint border = 0;
  GLenum format = GL_BGR;
  GLenum type = GL_UNSIGNED_BYTE;
  GLubyte* texData = image.accessPixels();
  glTexImage2D (GL_TEXTURE_2D, level, internalFormat, width, height, border,
		format, type, texData);
  glGenerateMipmap (GL_TEXTURE_2D);
  unbind ();
}

void
Texture::bind () const
{
  glBindTexture (GL_TEXTURE_2D, m_textureId);
}

void
Texture::unbind () const
{
  glBindTexture (GL_TEXTURE_2D, 0);
}
