
#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
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
void printShaderCompileInfoLog(GLint shader);
void printProgramLinkInfoLog(GLint program);
bool isCompileOk(GLint shader);
bool isLinkOk(GLint program);
bool loadShader(GLint shader, const string& strFileName);
void terminate(const string& strErr);

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
  GLenum err;
  if((err = glewInit()) != GLEW_OK)
  {
    /* Problem: glewInit failed, something is seriously wrong. */
    cerr << "Error: " << glewGetErrorString(err) << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

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

  GLfloat vertices[] = {
    //Rectangle
    -0.5f, 0.25f, //A (id=0)
    0.5f, 0.25f, //B (id=1)
    0.5f, -0.25f, //C (id=2)
    -0.5f, -0.25f, //D (id=3)
  };

  GLuint indices[] = {
    0, 2, 3, //Triangle 1 (ACD)
    0, 1, 2  //Triangle 2 (ABC)
  };

  //Création du programme de shader
  // Le vertex shader
  cout << "Chargement du vertex shader" << endl;
  GLint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if(!loadShader(vertexShader, "vertex.vert"))
    terminate("Impossible d'ouvrir les sources du vertex shader");

  cout << "Compilation du vertex shader" << endl;
  glCompileShader(vertexShader);
  if(!isCompileOk(vertexShader))
  {
    printShaderCompileInfoLog(vertexShader);
    terminate("Compilation du vertex shader impossible");
  }
  // Le fragment shader
  cout << "Chargement du fragment shader" << endl;
  GLint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  if(!loadShader(fragmentShader, "fragment.frag"))
    terminate("Impossible d'ouvrir les sources du fragment shader");

  cout << "Compilation du fragment shader" << endl;
  glCompileShader(fragmentShader);
  if(!isCompileOk(fragmentShader))
  {
    printShaderCompileInfoLog(fragmentShader);
    terminate("Compilation du fragment shader impossible");
  }

  // Edition des liens
  cout << "Edition des liens des shaders" << endl;
  GLint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  if(!isLinkOk(shaderProgram))
  {
    printProgramLinkInfoLog(shaderProgram);
    terminate("Edition des liens des shaders impossible");
  }

  // Suppression des étapes intermédiare de compilation
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  //Activation du shader
  glUseProgram(shaderProgram);

  //Spécification des vertices pour ce programme shader
  // Le VAO
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Le VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //spécifie l'organisation de l'entrée du vertex shader
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(0));
  glEnableVertexAttribArray(0);

  // Le EBO
  GLuint EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  while(!glfwWindowShouldClose(pWnd))
  {
    glfwPollEvents();

    glClearColor(clearcolor.GetR(), clearcolor.GetG(), clearcolor.GetB(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));

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
  switch (key)
  {
  case GLFW_KEY_UP:
    h = clearcolor.GetH() + 1.0f;
    if (h > 360.0f)
      h = 360.0f;
    clearcolor.SetHSV(h, clearcolor.GetS(), clearcolor.GetV());
    break;
  case GLFW_KEY_DOWN:
    h = clearcolor.GetH() - 1.0f;
    if (h < 0.0f)
      h = 0.0f;
    clearcolor.SetHSV(h, clearcolor.GetS(), clearcolor.GetV());
    break;
  case GLFW_KEY_W:
    {
      static GLint mode = GL_FILL;
      if (mode == GL_LINE)
        mode = GL_FILL;
      else
        mode = GL_LINE;
      glPolygonMode(GL_FRONT_AND_BACK, mode);
    }
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

void printShaderCompileInfoLog(GLint shader)
{
  GLint length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  string infologbuffer;
  infologbuffer.resize(length);
  glGetShaderInfoLog(shader, length, nullptr, &infologbuffer.front());
  cout << infologbuffer << endl;
}

bool isCompileOk(GLint shader)
{
  GLint value;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &value);
  return value == GL_TRUE;
}

void printProgramLinkInfoLog(GLint program)
{
  GLint length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  string infologbuffer;
  infologbuffer.resize(length);
  glGetProgramInfoLog(program, length, nullptr, &infologbuffer.front());
  cout << infologbuffer << endl;
}

bool isLinkOk(GLint program)
{
  GLint value;
  glGetProgramiv(program, GL_LINK_STATUS, &value);
  return value == GL_TRUE;
}

bool loadShader(GLint shader, const string& strFileName)
{
  ifstream vertexShaderFile{ strFileName };
  if(!vertexShaderFile.is_open())
    return false;
  stringstream vertexShaderStream;
  vertexShaderStream << vertexShaderFile.rdbuf();
  string vertexShaderString{ vertexShaderStream.str() };
  const GLchar* vertexShaderSource = vertexShaderString.c_str(); //vertexShaderSource est correct tant que vertexShaderString n'est pas modifié

  glShaderSource(shader, 1, &vertexShaderSource, nullptr);
  return true;
}

void terminate(const string& strErr)
{
  cerr << strErr << endl;
  glfwTerminate();
  exit(EXIT_FAILURE);
}