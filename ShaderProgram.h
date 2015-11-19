#ifndef ShaderProgram_h
#define ShaderProgram_h

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ShaderProgram
{

  GLuint m_vertexShader, m_fragmentShader, m_programId;

  void
  createShader (GLuint shadertype, std::string file);

  const char*
  readFile (std::string file);

  void
  checkCompileSuccess (GLuint shader, std::string file);

  void
  checkLinkSuccess ();

public:

  ShaderProgram();

  //~ShaderProgram();

  void
  createProgramId ();

  void
  createVertexShader (std::string file);

  void
  createFragmentShader (std::string file);

  void
  link ();

  GLint
 getAttributeLocation (std::string position) const;

  GLint
 getUniformLocation (std::string position) const;

  void
  setUniformMatrix4fv (int uniformlocation, int count, unsigned char transpose,
		       float* matrix);

  void
  enable ();

  void
  disable ();

};

#endif
