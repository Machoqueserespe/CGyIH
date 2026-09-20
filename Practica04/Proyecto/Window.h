#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	// Consulto estos ángulos desde el main para girar cada articulación del rover.
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	// numero va de 0 a 5 y me permite consultar el ángulo de una sola llanta.
	GLfloat getrueda(int numero) { return ruedas[numero]; }
	// Cada panel y la antena tienen su propio ángulo, independiente del giro de la sonda.
	GLfloat getpanel1() { return panel1; }
	GLfloat getpanel2() { return panel2; }
	GLfloat getantena() { return antena; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	// rotax, rotay y rotaz giran la sonda completa.
	// articulacion1 a articulacion6 controlan base del brazo, hombro, codo, muñeca, pinza y suspensión.
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6;
	GLfloat ruedas[6]; // Guardo un ángulo en grados para cada llanta.
	GLfloat panel1, panel2, antena; // Ángulos de las dos bisagras y del soporte de la antena.
	bool keys[1024]; // Registro cuáles teclas están presionadas.
	GLint bufferWidth, bufferHeight;
	// Comparo la posición actual del ratón con la anterior para obtener su desplazamiento.
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved; // Uso la primera lectura como referencia para evitar un salto de la cámara.
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};
