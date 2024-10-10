#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include"Color.h"
#include<iostream>
#include <fstream>
#include <sstream>
void posfunct(GLFWwindow* window, double xpos, double ypos) {
	CColor& clearcolor = *(reinterpret_cast<CColor*>(glfwGetWindowUserPointer(window)));

	int nWidth, nHeight;
	glfwGetWindowSize(window, &nWidth, &nHeight);

	clearcolor.SetHSV(clearcolor.GetH(), (xpos/1.0f / nWidth), static_cast<float>((nHeight - ypos) / nHeight));
}
void keyfunct(GLFWwindow* window, int key, int scancode, int action, int mods) {
	CColor& clearcolor = *(reinterpret_cast<CColor*>(glfwGetWindowUserPointer(window)));
	if (action == GLFW_PRESS ) {
		switch (key) {
		case GLFW_KEY_UP:
			clearcolor.SetHSV(clearcolor.GetH()+1, clearcolor.GetS(), clearcolor.GetV());
				break;
		case GLFW_KEY_DOWN:
			clearcolor.SetHSV(clearcolor.GetH() - 1, clearcolor.GetS(), clearcolor.GetV());
			break;
		}
	}
}


int main(int argc, char* argv[])
{
	glfwInit();
   //Préparer la fenêtre(taille, attributs, version d’OpenGL)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Créer la fenêtre
	GLFWwindow* pWnd = glfwCreateWindow(800, 600, "En mode beau gosse la team", nullptr, nullptr);
	//Activer le contexte de la fenêtre
	glfwMakeContextCurrent(pWnd);
	//Initialiser la bibliothèque
	glewExperimental = GL_TRUE;
	glewInit();
	//Ecrire une boucle d’événements
	CColor couleur = { 0,100,100 };
	glfwSetKeyCallback(pWnd, keyfunct);
	glfwSetWindowUserPointer(pWnd, &couleur);
	glfwSetCursorPosCallback(pWnd, posfunct);

	GLfloat vertices[] = {
		//Rectangle
	   -0.5f,  0.25f, //0
		0.5f,  0.25f, //1
		0.5f, -0.25f, //2
	   -0.5f, -0.25f, //3
	   -0.5f,  0.75f, //4
		0.5f,  0.75f, //5

	};
	GLuint indices[] = {
		0, 2, 3, //Triangle 1 (ACD)
		0, 1, 2,  //Triangle 2 (ABC)
		 4,0,1,
		 4,1,5,
	};


	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	GLint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::ifstream vertexShaderFile{"vertex.vert"};
	if (!vertexShaderFile.is_open())
		return false;
	std::stringstream vertexShaderStream;
	vertexShaderStream << vertexShaderFile.rdbuf();
	std::string vertexShaderString{ vertexShaderStream.str() };
	const GLchar* vertexShaderSource = vertexShaderString.c_str(); //vertexShaderSource est correct tant que vertexShaderString n'est pas modifié
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);


	GLint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream fragmentShaderFile{ "fragment.frag" };
	if (!fragmentShaderFile.is_open())
		return false;
	std::stringstream fragmentShaderStream;
	fragmentShaderStream << fragmentShaderFile.rdbuf();
	std::string fragmentShaderString{ fragmentShaderStream.str() };
	const GLchar* fragmentShaderSource = fragmentShaderString.c_str(); //vertexShaderSource est correct tant que vertexShaderString n'est pas modifié

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	GLint fragmentblueShader;
	fragmentblueShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream fragmentblueShaderFile{ "fragmentbleu.frag" };
	if (!fragmentblueShaderFile.is_open())
		return false;
	std::stringstream fragmentblueShaderStream;
	fragmentblueShaderStream << fragmentblueShaderFile.rdbuf();
	std::string fragmentblueShaderString{ fragmentblueShaderStream.str() };
	const GLchar* fragmentblueShaderSource = fragmentblueShaderString.c_str(); //vertexShaderSource est correct tant que vertexShaderString n'est pas modifié

	glShaderSource(fragmentblueShader, 1, &fragmentblueShaderSource, nullptr);
	glCompileShader(fragmentblueShader);

	//Création d’un « objet » programme OpenGL
	GLint shaderProgram;
	shaderProgram = glCreateProgram();
	//Attacher les shaders compilés à ce programme
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Demander l’édition des liens
	glLinkProgram(shaderProgram);
	//Nettoyer les éléments temporaires de compilation pour récupérer les ressources

	GLint shaderBlueProgram;
	shaderBlueProgram = glCreateProgram();
	//Attacher les shaders compilés à ce programme
	glAttachShader(shaderBlueProgram, vertexShader);
	glAttachShader(shaderBlueProgram, fragmentblueShader);
	//Demander l’édition des liens
	glLinkProgram(shaderBlueProgram);
	//Nettoyer les éléments temporaires de compilation pour récupérer les ressources

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentblueShader);

	

	


	while (!glfwWindowShouldClose(pWnd))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(couleur.GetR(), couleur.GetG(), couleur.GetB(),255);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUseProgram(shaderBlueProgram);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUseProgram(shaderProgram);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(6*sizeof(GLuint)));
		
		glfwSwapBuffers(pWnd);
		

	}

	
	glfwTerminate();


	return 0;
}
//void keyfunct(GLFWwindow* pWnd, int key, int scancode, int action, int mods);
