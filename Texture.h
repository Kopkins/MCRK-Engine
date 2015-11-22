#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture
{
public:
  Texture ();

  ~Texture ();

  void
  setSWrapMode (GLint sWrapMode);

  void
  setTWrapMode (GLint tWrapMode);

  void
  setMinFilter (GLint minFilter);

  void
  setMagFilter (GLint magFilter);

  void
  setData (const std::string& textureFile);

  void
  bind () const;

  void
  unbind () const;

private:
  GLuint m_textureId;
};

#endif 
