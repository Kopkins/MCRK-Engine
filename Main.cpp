/* Z Engine 2.0
 Uses OpenGL 3.3 technology
 by Gary M. Zoppetti
 */

/******************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

// Use GLEW so we can access the latest OpenGL functionality
// Always include GLEW before GLFW!
#include <GL/glew.h>
// Use GLFW so we can create windows and handle events in a
//   platform-independent way
#include <GLFW/glfw3.h>

// GLM is a header-only library, so we do not need to link to it
// Vector and matrix classes
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
// Need for "value_ptr"
#include <glm/gtc/type_ptr.hpp>
// Transforms like rotation, view, and projection
#include <glm/gtc/matrix_transform.hpp>

/******************************************************************/
// Local includes
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Camera.h"
#include "Matrix3.h"
#include "AiScene.h"
#include "Transform.h"
#include "KeyBuffer.h"
/******************************************************************/
// Making a struct to hold our mouse data to prevent the use of too many global variables
namespace
{
  struct MouseHandler
  {
    MouseHandler () :
	x (), y (), lClick (false), rClick (false)
    {
    }

    float x;
    float y;
    bool lClick;
    bool rClick;
  };
}

/******************************************************************/
// Type declarations/globals vars/prototypes
ShaderProgram g_shaderProgram;
//Mesh* g_mesh;
std::vector<Mesh*> g_meshVector;
Camera g_cam;
KeyBuffer g_keybuffer;
bool g_isWire;
MouseHandler g_mouse;
/******************************************************************/

void
init (GLFWwindow*& window);

void
initGlfw ();

void
initGlew ();

void
initWindow (GLFWwindow*& window);

void
resetViewport (GLFWwindow* window, int width, int height);

void
initScene ();

void
initCamera ();

void
initShaders ();

void
updateScene (double time);

void
updateCamera (KeyBuffer keybuffer, float moveDelta, float rotateDelta);

void
updateMesh (KeyBuffer buffer, float moveDelta, float rotateDelta,
	    float shearDelta);

void
drawScene (GLFWwindow* window);

void
drawObject ();

void
processKey (GLFWwindow* window, int key, int scanCode, int action,
	    int modifiers);

void
processMousePosition (GLFWwindow* window, double x, double y);

void
processMouseButton (GLFWwindow* window, int button, int action, int modifiers);

void
processScrollWheel(GLFWwindow* window, double xoff, double yoff);

void
processWindowSize(GLFWwindow* window, int width, int height);

void
releaseGlResources ();

void
handleGlfwError (int error, const char* description);

/******************************************************************/

int
main (int argc, char* argv[])
{
  GLFWwindow* window;
  init (window);
  g_isWire=false;

  // Game/render loop
  double previousTime = glfwGetTime ();
  while (!glfwWindowShouldClose (window))
    {
      double currentTime = glfwGetTime ();
      double deltaTime = currentTime - previousTime;
      previousTime = currentTime;
      updateScene (deltaTime);
      drawScene (window);

      glfwPollEvents ();
    }

  releaseGlResources ();
  glfwDestroyWindow (window);
  glfwTerminate ();

  return EXIT_SUCCESS;
}

/******************************************************************/

void
init (GLFWwindow*& window)
{
  // Always initialize GLFW before GLEW
  initGlfw ();
  initWindow (window);
  initGlew ();
  initScene ();

}

/******************************************************************/

void
initGlfw ()
{
  glfwSetErrorCallback (handleGlfwError);
  if (!glfwInit ())
    {
      fprintf (stderr, "Failed to initialize GLFW\n");
      exit (-1);
    }
}

/******************************************************************/

void
initWindow (GLFWwindow*& window)
{
//#ifdef __APPLE__
//  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
//  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
//  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#endif
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  window = glfwCreateWindow (1200, 900, "OpenGL", nullptr, nullptr);
  glfwSetWindowPos (window, 200, 100);
  if (!window)
    {
      fprintf (stderr, "Failed to initialize the window\n");
      glfwTerminate ();
      exit (-1);
    }

  glfwMakeContextCurrent (window);
  // Swap buffers after 1 frame
  glfwSwapInterval (1);
  glfwSetKeyCallback (window, processKey);
  // GMZ: Don't track mouse movement yet
  glfwSetCursorPosCallback (window, processMousePosition);
  glfwSetMouseButtonCallback (window, processMouseButton);
  glfwSetFramebufferSizeCallback (window, resetViewport);
  glfwSetScrollCallback(window, processScrollWheel);
  glfwSetWindowSizeCallback(window, processWindowSize);
  // Specify background color
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
  // Enable depth testing so we don't draw occluded surfaces
  glEnable (GL_DEPTH_TEST);
  // Cull back-facing triangles
  //glEnable (GL_CULL_FACE);
  // Set initial viewport size
  int width, height;
  glfwGetFramebufferSize (window, &width, &height);
  glViewport (0, 0, width, height);
}

/******************************************************************/

void
initGlew ()
{

  glewExperimental = GL_TRUE;
  GLenum status = glewInit ();
  if (status != GLEW_OK)
    {
      fprintf (stderr, "Failed to initialize GLEW"
	       " (%s).\n",
	       glewGetErrorString (status));
      exit (-1);
    }
  auto version = glewGetString (GLEW_VERSION);
  fprintf (stderr, "Using GLEW version %s.\n", version);
  version = glGetString (GL_VERSION);
  fprintf (stderr, "Using OpenGL version %s\n", version);
}

/******************************************************************/

void
resetViewport (GLFWwindow* window, int width, int height)
{
  // Render into entire window
  // Origin for window coordinates is lower-left of window,
  //   NOT upper-left like screen coordinates
  glViewport (0, 0, width, height);
}

/******************************************************************/

void
initScene ()
{
  initShaders ();
  initCamera ();
  AiScene Model ("Sphere.obj");
  std::shared_ptr<VertexBuffer> sphere = Model.readVertexData (0);
  g_meshVector.push_back (new Mesh);
  //std::vector<float> print = *sphere;
  g_meshVector[0]->addGeometry (sphere);
  g_meshVector[0]->loadTexture ("EarthBath.png");

}

/******************************************************************/
void
initCamera ()
{
  g_cam.setVerticalFov (glm::radians (50.0f));
  g_cam.setAspectRatio (4.0f / 3);
  g_cam.setNearZ (0.01f);
  g_cam.setFarZ (40.0f);
  g_cam.createProjectionMatrix (g_shaderProgram);
  g_cam.setEye (0.0f, 0.0f, 12.0f);
  g_cam.setUp (0, 1, 0);

}
/******************************************************************/
void
initShaders ()
{
  g_shaderProgram.createProgramId ();
  g_shaderProgram.createVertexShader ("Vec3NormTexPhong.vert");
  g_shaderProgram.createFragmentShader ("Vec3TexMix.frag");
  g_shaderProgram.link ();
}

/******************************************************************/

void
updateScene (double time)
{
  const float MOVEMENT_DELTA = 5.0f * time;
  const float ROTATION_DELTA = 1.0f * time;
  const float SCALESHEAR_DELTA = 1.1f;
  updateCamera (g_keybuffer, MOVEMENT_DELTA, ROTATION_DELTA);
  updateMesh (g_keybuffer, MOVEMENT_DELTA, ROTATION_DELTA, SCALESHEAR_DELTA);
  //g_cam.updateCamera (g_keybuffer.getBuffer (), MOVEMENT_DELTA, ROTATION_DELTA);
  // g_meshVector[g_meshVector.size () - 1]->updateMesh (g_keybuffer.getBuffer (),
  //MOVEMENT_DELTA,
  //ROTATION_DELTA,
  //SCALESHEAR_DELTA);
}

/******************************************************************/
void
updateCamera (KeyBuffer keybuffer, float moveDelta, float rotateDelta)
{
  std::bitset<GLFW_KEY_LAST> buffer = keybuffer.getBuffer ();
  if (buffer[GLFW_KEY_R])
    g_cam.reset ();
  g_cam.moveBack (buffer[GLFW_KEY_W] * moveDelta * -1);
  g_cam.moveBack (buffer[GLFW_KEY_S] * moveDelta);
  g_cam.moveRight (buffer[GLFW_KEY_A] * moveDelta * -1);
  g_cam.moveRight (buffer[GLFW_KEY_D] * moveDelta);
  g_cam.moveUp (buffer[GLFW_KEY_C] * moveDelta * -1);
  g_cam.moveUp (buffer[GLFW_KEY_F] * moveDelta);
  g_cam.yaw (buffer[GLFW_KEY_Q] * moveDelta);
  g_cam.yaw (buffer[GLFW_KEY_E] * moveDelta * -1);
}

/******************************************************************/
void
updateMesh (KeyBuffer keybuffer, float moveDelta, float rotateDelta,
	    float shearDelta)
{
  std::bitset<GLFW_KEY_LAST> buffer = keybuffer.getBuffer ();
  g_meshVector[g_meshVector.size () - 1]->moveRight (
      buffer[GLFW_KEY_1] * moveDelta);
  g_meshVector[g_meshVector.size () - 1]->pitch (
      buffer[GLFW_KEY_2] * rotateDelta);
  g_meshVector[g_meshVector.size () - 1]->yaw (
      buffer[GLFW_KEY_3] * rotateDelta);
  g_meshVector[g_meshVector.size () - 1]->roll (
      buffer[GLFW_KEY_4] * rotateDelta);
  if (buffer[GLFW_KEY_5])
    g_meshVector[g_meshVector.size () - 1]->alignWithWorldY ();
  if (buffer[GLFW_KEY_6])
    g_meshVector[g_meshVector.size () - 1]->scaleLocal (shearDelta);
  if (buffer[GLFW_KEY_7])
    g_meshVector[g_meshVector.size () - 1]->scaleWorld (shearDelta);
  g_meshVector[g_meshVector.size () - 1]->shearLocalXByYz (
      buffer[GLFW_KEY_8] * shearDelta, buffer[GLFW_KEY_8] * shearDelta);
  g_meshVector[g_meshVector.size () - 1]->shearLocalYByXz (
      buffer[GLFW_KEY_9] * shearDelta, buffer[GLFW_KEY_9] * shearDelta);
  g_meshVector[g_meshVector.size () - 1]->shearLocalZByXy (
      buffer[GLFW_KEY_0] * shearDelta, buffer[GLFW_KEY_0] * shearDelta);
}

/******************************************************************/
void
drawScene (GLFWwindow* window)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  drawObject ();

  glfwSwapBuffers (window);
}

/******************************************************************/

void
drawObject ()
{
  float meshTransform[16];
  for (uint i = 0; i < g_meshVector.size (); i++)
    {
      g_meshVector[i]->prepareVao (g_shaderProgram);
      g_meshVector[i]->getTransform (meshTransform);
      g_cam.createModelViewMatrix (g_shaderProgram, meshTransform);
      g_meshVector[i]->draw (g_shaderProgram);
    }
  //g_meshVector[0]->getTransform(meshTransform);
  //g_cam.createModelViewMatrix(g_shaderProgram, meshTransform);
  //g_meshVector[0]->draw (g_shaderProgram);

}

/******************************************************************/

void
processKey (GLFWwindow* window, int key, int scanCode, int action,
	    int modifiers)
{

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      glfwSetWindowShouldClose (window, GL_TRUE);
      return;
    }
  if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
    {
      //Mesh* clone = g_meshVector[0]->clone();
      g_meshVector.push_back (g_meshVector[0]->clone ());
      g_meshVector[g_meshVector.size () - 1]->setPosition (
	  g_meshVector[g_meshVector.size () - 2]->getPosition ().x - .4,
	  g_meshVector[g_meshVector.size () - 2]->getPosition ().y + .4,
	  g_meshVector[g_meshVector.size () - 2]->getPosition ().z);
    }
  if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
    {
      delete g_meshVector[g_meshVector.size () - 1];
      g_meshVector.pop_back ();
    }
  if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
      if (g_cam.isOrtho ())
	g_cam.createProjectionMatrix (g_shaderProgram);
      else
	g_cam.createProjectionMatrix (g_shaderProgram, true);
    }
  if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
      if (g_isWire)
	{
	  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	  g_isWire=false;
	}
      else
	{
	  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	  g_isWire=true;
	}
    }

  g_keybuffer.setKey (key, action);
}

/******************************************************************/

void
processMousePosition (GLFWwindow* window, double x, double y)
{
  // "x" and "y" are screen (NOT window) coordinates, with
  //   (0.0, 0.0) at the upper left of the window
  float dx, dy, dz;
  if (g_mouse.lClick)
    {
      dx = x - g_mouse.x;
      dy = y - g_mouse.y;
    }
  if (g_mouse.rClick)
    {
      dz = x - g_mouse.x;
    }

  g_mouse.x = x;
  g_mouse.y = y;

  if (g_mouse.lClick)
    {
      g_cam.yaw (dx * 0.001f);
      g_cam.pitch (dy * 0.001f);
    }
  if (g_mouse.rClick)
    {
      g_cam.roll (dz * 0.001f);
    }
}


/******************************************************************/

void
processMouseButton (GLFWwindow* window, int button, int action, int modifiers)
{
  // Other relevant constants are GLFW_MOUSE_BUTTON_RIGHT and GLFW_RELEASE
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
	  g_mouse.lClick = action;

  } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      g_mouse.rClick = action;
  }
}
/******************************************************************/
void
processScrollWheel(GLFWwindow* window, double xoff, double yoff){
  float newFov = glm::radians(-yoff * 2) + g_cam.getVerticalFov();
  g_cam.setVerticalFov (newFov);
  g_cam.createProjectionMatrix (g_shaderProgram);
}
/******************************************************************/

void
processWindowSize(GLFWwindow* window, int width, int height){
  g_cam.setAspectRatio (static_cast<float>(width) / height);
  g_cam.createProjectionMatrix (g_shaderProgram);
}

void
releaseGlResources ()
{

}

/******************************************************************/

void
handleGlfwError (int error, const char* description)
{
  fprintf (stderr, "GLFW error: %s, code %d\n", description, error);
  exit (-1);
}

/******************************************************************/

