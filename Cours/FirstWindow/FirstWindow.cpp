
#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

#ifdef WIN32
//This magic line is to force notebook computer that share NVidia and Intel graphics to use high performance GPU (NVidia).
//Intel graphics doesn't support OpenGL > 1.2
extern "C" _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
#endif

#include "Color.h"

const char* GetGLErrorString();
void keyfunct(GLFWwindow* pWnd, int key, int scancode, int action, int mods);
void posfunct(GLFWwindow* pWnd, double xpos, double ypos);

int main(void)
{
#ifdef WIN32
  SetConsoleOutputCP(65001);
#endif
  const char* pText;


  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow* pWnd = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
  if (!pWnd)
  {
    cerr << "Impossible de créer la fenêtre !" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(pWnd);
  glewExperimental = GL_TRUE;
  glewInit();

  pText = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  if (!pText) pText = GetGLErrorString();
  cout << "Company : " << pText << endl;

  pText = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  if (!pText) pText = GetGLErrorString();
  cout << "Renderer : " << pText << endl;

  pText = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  if (!pText) pText = GetGLErrorString();
  cout << "Version : " << pText << endl;

  CColor clearcolor(0.0f, 1.0f, 0.5f);
  glfwSetWindowUserPointer(pWnd, &clearcolor);
  glfwSetKeyCallback(pWnd, keyfunct);
  glfwSetCursorPosCallback(pWnd, posfunct);

  while(!glfwWindowShouldClose(pWnd))
  {
    glfwPollEvents();

    glClearColor(clearcolor.GetR(), clearcolor.GetG(), clearcolor.GetB(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(pWnd);
  }

  glfwTerminate();

  return 0;
}


const char* GetGLErrorString()
{
  switch (glGetError())
  {
  case GL_NO_ERROR:
    return "GL_NO_ERROR";
  case GL_INVALID_ENUM:
    return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE:
    return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION:
    return "GL_INVALID_OPERATION";
  case GL_STACK_OVERFLOW:
    return "GL_STACK_OVERFLOW";
  case GL_STACK_UNDERFLOW:
    return "GL_STACK_UNDERFLOW";
  case GL_OUT_OF_MEMORY:
    return "GL_OUT_OF_MEMORY";
    //case GL_INVALID_FRAMEBUFFER_OPERATION?:
    //	return "GL_INVALID_FRAMEBUFFER_OPERATION???";
    //case GL_TABLE_TOO_LARGE?:
    //	return "GL_TABLE_TOO_LARGE???";
  }
  return "Erreur inconnue";
}

void keyfunct(GLFWwindow* pWnd, int key, int scancode, int action, int mods)
{
  if(action == GLFW_RELEASE)
    return;

  CColor &clearcolor = *(reinterpret_cast<CColor*>(glfwGetWindowUserPointer(pWnd)));
  float h;
  switch(key)
  {
  case GLFW_KEY_UP:
    h = clearcolor.GetH() + 1.0f;
    if(h > 360.0f)
      h = 0.0f;
    clearcolor.SetHSV(h, clearcolor.GetS(), clearcolor.GetV());
    break;
  case GLFW_KEY_DOWN:
    h = clearcolor.GetH() - 1.0f;
    if(h < 0.0f)
      h = 359.0f;
    clearcolor.SetHSV(h, clearcolor.GetS(), clearcolor.GetV());
    break;
  }
}

void posfunct(GLFWwindow* pWnd, double xpos, double ypos) 
{
  CColor &clearcolor = *(reinterpret_cast<CColor*>(glfwGetWindowUserPointer(pWnd)));

  int nWidth, nHeight;
  glfwGetWindowSize(pWnd, &nWidth, &nHeight);
  
  clearcolor.SetHSV(clearcolor.GetH(), static_cast<float>(xpos / nWidth), static_cast<float>((nHeight - ypos) / nHeight));
}

