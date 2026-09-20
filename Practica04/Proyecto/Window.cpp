#include "Window.h"

Window::Window()
{
	mainWindow = nullptr;
	xChange = yChange = lastX = lastY = 0.0f;
	mouseFirstMoved = true;
	width = 800;
	height = 600;
	rotax = rotay = rotaz = 0.0f;
	articulacion1 = articulacion2 = articulacion3 = 0.0f;
	articulacion4 = articulacion5 = articulacion6 = 0.0f;
	panel1 = panel2 = antena = 0.0f;
	for (int i = 0; i < 6; i++) ruedas[i] = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	// Inicializo la ventana y el ratón antes de recibir el primer evento.
	mainWindow = nullptr;
	xChange = yChange = lastX = lastY = 0.0f;
	mouseFirstMoved = true;
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	// Inicio las seis ruedas y los elementos móviles de la sonda en su posición de reposo.
	for (int i = 0; i < 6; i++) ruedas[i] = 0.0f;
	panel1 = panel2 = antena = 0.0f;
	
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 04 | F1 Rover | F2 Sonda | 1-6 ruedas | Shift: invertir", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); // Uso la profundidad para ocultar las superficies que quedan detrás.
	// El área de dibujo ocupa el tamaño real del framebuffer.
	glViewport(0, 0, bufferWidth, bufferHeight);
	// Asocio la ventana con este objeto para consultarlo desde los eventos del teclado y el ratón.
	glfwSetWindowUserPointer(mainWindow, this);
	return 0; // Indico que la ventana se creó correctamente.
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	
	// paso vale 5 grados; con Shift vale -5 para girar en sentido contrario.
	// Aplico el giro al presionar o mantener la tecla, no al soltarla.
	GLfloat paso = (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->rotax += paso;
	}
	if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->rotay += paso;
	}
	if (key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->rotaz += paso;
	}
	// Limito cada articulación para que el brazo y la suspensión no giren sin tope.
	if (key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion1 += paso;
		if (theWindow->articulacion1 > 80.0f) theWindow->articulacion1 = 80.0f;
		if (theWindow->articulacion1 < -80.0f) theWindow->articulacion1 = -80.0f;
	}

	if (key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion2 += paso;
		if (theWindow->articulacion2 > 35.0f) theWindow->articulacion2 = 35.0f;
		if (theWindow->articulacion2 < -25.0f) theWindow->articulacion2 = -25.0f;
	}
	if (key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion3 += paso;
		if (theWindow->articulacion3 > 35.0f) theWindow->articulacion3 = 35.0f;
		if (theWindow->articulacion3 < -40.0f) theWindow->articulacion3 = -40.0f;
	}
	if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion4 += paso;
		if (theWindow->articulacion4 > 45.0f) theWindow->articulacion4 = 45.0f;
		if (theWindow->articulacion4 < -45.0f) theWindow->articulacion4 = -45.0f;
	}
	if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion5 += paso;
		if (theWindow->articulacion5 > 35.0f) theWindow->articulacion5 = 35.0f;
		if (theWindow->articulacion5 < 0.0f) theWindow->articulacion5 = 0.0f;
	}
	if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion6 += paso;
		if (theWindow->articulacion6 > 10.0f) theWindow->articulacion6 = 10.0f;
		if (theWindow->articulacion6 < -10.0f) theWindow->articulacion6 = -10.0f;
	}
	// Un ángulo por rueda. 1, 2 y 3 corresponden a Z negativo; 4, 5 y 6, a Z positivo.
	if (key >= GLFW_KEY_1 && key <= GLFW_KEY_6 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		int rueda = key - GLFW_KEY_1;
		theWindow->ruedas[rueda] += paso;
		if (theWindow->ruedas[rueda] >= 360.0f) theWindow->ruedas[rueda] -= 360.0f;
		if (theWindow->ruedas[rueda] <= -360.0f) theWindow->ruedas[rueda] += 360.0f;
	}
	// Cada panel gira desde su bisagra y la antena desde su soporte.
	if (key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->panel1 += paso;
		if (theWindow->panel1 > 70.0f) theWindow->panel1 = 70.0f;
		if (theWindow->panel1 < -20.0f) theWindow->panel1 = -20.0f;
	}
	if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->panel2 += paso;
		if (theWindow->panel2 > 70.0f) theWindow->panel2 = 70.0f;
		if (theWindow->panel2 < -20.0f) theWindow->panel2 = -20.0f;
	}
	if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->antena += paso;
		if (theWindow->antena > 45.0f) theWindow->antena = 45.0f;
		if (theWindow->antena < -45.0f) theWindow->antena = -45.0f;
	}


	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
