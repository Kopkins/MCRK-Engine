/******************************************************************/
//Includes
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/trigonometric.hpp>
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
#include "Scene.h"
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
Camera g_cam;
KeyBuffer g_keybuffer;
bool g_isWire;
MouseHandler g_mouse;
Scene g_scene;
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
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  window = glfwCreateWindow (1200, 900, "OpenGL", nullptr, nullptr);
  glfwSetWindowPos (window, 0, 0);
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
  Material mat(Vector3(0.1, 0.1, 0.2),Vector3(0.75,0.75,0.8),Vector3(0.75,0.75,0.8),128.0f);
  g_scene.createMesh("first", "Sphere.obj", "EarthBath.png", mat, &g_shaderProgram);
  //g_meshVector.push_back (new Mesh);
  //std::vector<float> print = *sphere;
  //g_meshVector[0]->addGeometry (sphere);
  //g_meshVector[0]->setShaderPointer(&g_shaderProgram);
  //g_meshVector[0]->loadTexture ("EarthBath.png");
  //g_meshVector[0]->activateMaterial();
  //g_meshVector[0]->prepareVao ();

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
  g_cam.setEye (0.0f, 0.0f, 6.0f);
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
  Mesh* mesh = g_scene.getActive();
  mesh->moveRight (
      buffer[GLFW_KEY_1] * moveDelta);
  mesh->pitch (
      buffer[GLFW_KEY_2] * rotateDelta);
  mesh->yaw (
      buffer[GLFW_KEY_3] * rotateDelta);
  mesh->roll (
      buffer[GLFW_KEY_4] * rotateDelta);
  if (buffer[GLFW_KEY_5])
    mesh->alignWithWorldY ();
  if (buffer[GLFW_KEY_6])
    mesh->scaleLocal (shearDelta);
  if (buffer[GLFW_KEY_7])
    mesh->scaleWorld (shearDelta);
  mesh->shearLocalXByYz (
      buffer[GLFW_KEY_8] * shearDelta, buffer[GLFW_KEY_8] * shearDelta);
  mesh->shearLocalYByXz (
      buffer[GLFW_KEY_9] * shearDelta, buffer[GLFW_KEY_9] * shearDelta);
  mesh->shearLocalZByXy (
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
	float camTransform[16];
	g_cam.getTransform(camTransform);
	g_scene.getActive()->createModelViewMatrix(camTransform);
	g_scene.getActive()->draw();
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
      /*g_meshVector.push_back (g_meshVector[0]->clone ());
      g_meshVector[g_meshVector.size () - 1]->setPosition (
	  g_meshVector[g_meshVector.size () - 2]->getPosition ().x - .4,
	  g_meshVector[g_meshVector.size () - 2]->getPosition ().y + .4,
	  g_meshVector[g_meshVector.size () - 2]->getPosition ().z);
    	*/
    }
  if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
    {
      //delete g_meshVector[g_meshVector.size () - 1];
      //g_meshVector.pop_back ();
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

