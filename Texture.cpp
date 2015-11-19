#include <cstdlib>
#include <cassert>
#include <iostream>

// Use this for FreeImage
#include <FreeImagePlus.h>
// END FreeImage

#include "Texture.h"

Texture::Texture ()
{
  glGenTextures (1, &m_textureId);
  // Wrap modes default to GL_REPEAT
  // Min filter defaults to GL_NEAREST_MIPMAP_LINEAR
  // Mag filter defaults to GL_LINEAR
}

Texture::~Texture ()
{
  glDeleteTextures (1, &m_textureId);
}

// Options are GL_REPEAT (default), GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE,
//   and GL_CLAMP_TO_BORDER
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

// Options are GL_NEAREST, GL_LINEAR,
//   GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
//   GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
void
Texture::setMinFilter (GLint minFilter)
{
  this->bind ();
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  this->unbind ();
}

// Options are GL_NEAREST, GL_LINEAR
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
  // If using FreeImage the next two lines will load the image.
  // You can access the pixels with the "accessPixels" method of fipImage.
  // See FreeImageDemo.cc under the Code directory. 
  fipImage image;
  bool ok=image.load (textureFile.c_str ());

  if(!ok){
      //std::cout<<
  }

  unsigned int width = image.getWidth ();
  unsigned int height = image.getHeight ();

  // Level should be 0, unless manually specifying mipmaps
  const GLint level = 0;
  GLint internalFormat = GL_RGB;
  // Border is a legacy parameter and should be set to 0
  const GLint border = 0;
  GLenum format = GL_BGR;
  GLenum type = GL_UNSIGNED_BYTE;
  // Upload texture data to OpenGL.
  GLubyte* texData = image.accessPixels();
  glTexImage2D (GL_TEXTURE_2D, level, internalFormat, width, height, border,
		format, type, texData);

  //this->setMinFilter(GL_LINEAR);
  //this->setMagFilter(GL_LINEAR);
  // Don't generate mipmaps yet. Still testing.
  glGenerateMipmap (GL_TEXTURE_2D);
  unbind ();
  // No longer need texture data since OpenGL has it.

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
