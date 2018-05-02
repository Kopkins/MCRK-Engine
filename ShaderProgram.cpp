#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram () :
    m_vertexShader (), m_fragmentShader (), m_programId ()
{

}

ShaderProgram::~ShaderProgram ()
{

}

void
ShaderProgram::createShader (GLuint shadertype, std::string file)
{
  const char* src = readFile (file);
  glShaderSource (shadertype, 1, &src, NULL);
  glCompileShader (shadertype);
  checkCompileSuccess (shadertype, file);
}

const char*
ShaderProgram::readFile (std::string file)
{
  std::ifstream infile;
  infile.open (file.c_str ());
  infile.seekg (0, infile.end);
  int length = infile.tellg ();
  infile.seekg (0, infile.beg);
  char* data = new char[length + 1];
  infile.read (data, length);
  infile.close ();
  data[length] = '\0';
  return data;

}

void
ShaderProgram::checkCompileSuccess (GLuint shader, std::string file)
{
  GLint success = 0;
  glGetShaderiv (shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE)
    {
      int arraylength = 0;
      int writelength;
      glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &arraylength);
      char log[arraylength];
      glGetShaderInfoLog (shader, arraylength, &writelength, log);
      if (shader == m_fragmentShader)
	{
	  std::ofstream fragfile;
	  std::string filename = file + ".log";
	  fragfile.open (filename);
	  fragfile << log;
	  glDeleteShader (m_fragmentShader);
	  std::cout
	      << "Error! Could not compile m_fragmentShader shader. Please see "
	      << file << ".log for details." << std::endl;
	}
      else if (shader == m_vertexShader)
	{
	  std::ofstream vertfile;
	  std::string filename = file + ".log";
	  vertfile.open (filename);
	  vertfile << log;
	  glDeleteShader (m_vertexShader);
	  std::cout
	      << "Error! Could not compile m_vertexShader shader. Please see "
	      << file << ".log for details." << std::endl;
	}
    }
}

void
ShaderProgram::checkLinkSuccess ()
{
  GLint success = 0;
  glGetProgramiv (m_programId, GL_LINK_STATUS, &success);
  if (success == GL_FALSE)
    {
      int arraylength = 0;
      int writelength;
      glGetProgramiv (m_programId, GL_INFO_LOG_LENGTH, &arraylength);
      char log[arraylength];
      glGetProgramInfoLog (m_programId, arraylength, &writelength, log);
      std::ofstream progfile;
      progfile.open ("Vec2.log");
      progfile << log;
      glDeleteProgram (m_programId);
      std::cout
	  << "Error! Could not attach shaders. Please see Vec2.log for details."
	  << std::endl;
    }

}

void
ShaderProgram::createProgramId ()
{
  m_programId = glCreateProgram ();
}

void
ShaderProgram::createVertexShader (std::string file)
{
  m_vertexShader = glCreateShader (GL_VERTEX_SHADER);
  createShader (m_vertexShader, file);
}

void
ShaderProgram::createFragmentShader (std::string file)
{
  m_fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
  createShader (m_fragmentShader, file);
}

void
ShaderProgram::link ()
{
  glAttachShader (m_programId, m_vertexShader);
  glAttachShader (m_programId, m_fragmentShader);
  glLinkProgram (m_programId);
  checkLinkSuccess ();
}

GLint
ShaderProgram::getAttributeLocation (std::string position) const
{
  return glGetAttribLocation (m_programId, position.c_str ());
}

GLint
ShaderProgram::getUniformLocation (std::string position) const
{
  return glGetUniformLocation (m_programId, position.c_str ());
}

void
ShaderProgram::setUniformMatrix4fv (int uniformlocation, int count,
				    unsigned char transpose, float* matrix)
{
  glUniformMatrix4fv (uniformlocation, count, transpose, matrix);
}

void
ShaderProgram::setUniform3fv (int uniformlocation, int count, float* vector)
{
  glUniform3fv (uniformlocation, count, vector);
}

void
ShaderProgram::setUniform1f (int uniformlocation, float value)
{
  glUniform1f (uniformlocation, value);
}

void
ShaderProgram::setUniform1i (int uniformlocation, int value)
{
  glUniform1i(uniformlocation,value);
}

void
ShaderProgram::enable ()
{
  glUseProgram (m_programId);
}

void
ShaderProgram::disable ()
{
  glUseProgram (0);
}

