#include "Window.h"

// Comparto los ángulos y desplazamientos definidos en el main con el control del teclado.
extern GLfloat giroEsquina1, giroEsquina2, giroEsquina3, giroEsquina4;
extern GLfloat giroEsquina5, giroEsquina6, giroEsquina7, giroEsquina8;
extern GLfloat sateliteX, sateliteY, sateliteZ;
extern GLfloat giroPanel1, giroPanel2, giroPanel3, giroPanel4;

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	// Inicio los desplazamientos del mouse en cero para evitar movimientos antes de recibir su posición.
	lastX = lastY = xChange = yChange = 0.0f;
	// Tomo la primera posición como referencia para que la cámara no salte al empezar.
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
	mainWindow = glfwCreateWindow(width, height, "P05 - Rover, Holocron y satelite - Alan Barrientos", NULL, NULL);

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

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
	// Devuelvo cero cuando la ventana y OpenGL se inicializaron sin errores.
	return 0;
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

	
	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	// F mueve solo esta pata; Shift invierte el sentido. No cuento la liberación de la tecla.
	if (key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion1 +=
			(mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		// Recorto el ángulo después de cada cambio para no rebasar los límites.
		if (theWindow->articulacion1 > 45.0f)
			theWindow->articulacion1 = 45.0f;
		if (theWindow->articulacion1 < -45.0f)
			theWindow->articulacion1 = -45.0f;
	}

	// G mueve solo esta pata; Shift invierte el sentido. No cuento la liberación de la tecla.
	if (key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion2 +=
			(mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		// Recorto el ángulo después de cada cambio para no rebasar los límites.
		if (theWindow->articulacion2 > 45.0f)
			theWindow->articulacion2 = 45.0f;
		if (theWindow->articulacion2 < -45.0f)
			theWindow->articulacion2 = -45.0f;
	}
	// H mueve solo esta pata; Shift invierte el sentido. No cuento la liberación de la tecla.
	if (key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion3 +=
			(mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		// Recorto el ángulo después de cada cambio para no rebasar los límites.
		if (theWindow->articulacion3 > 45.0f)
			theWindow->articulacion3 = 45.0f;
		if (theWindow->articulacion3 < -45.0f)
			theWindow->articulacion3 = -45.0f;
	}
	// J mueve solo esta pata; Shift invierte el sentido. No cuento la liberación de la tecla.
	if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion4 +=
			(mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		// Recorto el ángulo después de cada cambio para no rebasar los límites.
		if (theWindow->articulacion4 > 45.0f)
			theWindow->articulacion4 = 45.0f;
		if (theWindow->articulacion4 < -45.0f)
			theWindow->articulacion4 = -45.0f;
	}
	// K mueve solo esta pata; Shift invierte el sentido. No cuento la liberación de la tecla.
	if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion5 +=
			(mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		// Recorto el ángulo después de cada cambio para no rebasar los límites.
		if (theWindow->articulacion5 > 45.0f)
			theWindow->articulacion5 = 45.0f;
		if (theWindow->articulacion5 < -45.0f)
			theWindow->articulacion5 = -45.0f;
	}
	// L mueve solo esta pata; Shift invierte el sentido. No cuento la liberación de la tecla.
	if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		theWindow->articulacion6 +=
			(mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		// Recorto el ángulo después de cada cambio para no rebasar los límites.
		if (theWindow->articulacion6 > 45.0f)
			theWindow->articulacion6 = 45.0f;
		if (theWindow->articulacion6 < -45.0f)
			theWindow->articulacion6 = -45.0f;
	}


	// Solo cambio el movimiento al pulsar o mantener una tecla; soltarla no suma otro paso.
	if (key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroEsquina1 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	}
	if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroEsquina2 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	}
	if (key == GLFW_KEY_3 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroEsquina3 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	}
	if (key == GLFW_KEY_4 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroEsquina4 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	}
	if (key == GLFW_KEY_5 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroEsquina5 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	}
	if (key == GLFW_KEY_6 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroEsquina6 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	}
	if (key == GLFW_KEY_7 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroEsquina7 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	}
	if (key == GLFW_KEY_8 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroEsquina8 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
	}

	// La traslación está en unidades de la escena y se hereda a todos los paneles.
	if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		sateliteX += (mode & GLFW_MOD_SHIFT) ? -0.2f : 0.2f;
	}
	if (key == GLFW_KEY_Y && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		sateliteY += (mode & GLFW_MOD_SHIFT) ? -0.2f : 0.2f;
	}
	if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		sateliteZ += (mode & GLFW_MOD_SHIFT) ? -0.2f : 0.2f;
	}

	// Limito los paneles a 70 grados por sentido para mantener un movimiento de bisagra.
	if (key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroPanel1 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		if (giroPanel1 > 70.0f) giroPanel1 = 70.0f;
		if (giroPanel1 < -70.0f) giroPanel1 = -70.0f;
	}
	if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroPanel2 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		if (giroPanel2 > 70.0f) giroPanel2 = 70.0f;
		if (giroPanel2 < -70.0f) giroPanel2 = -70.0f;
	}
	if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroPanel3 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		if (giroPanel3 > 70.0f) giroPanel3 = 70.0f;
		if (giroPanel3 < -70.0f) giroPanel3 = -70.0f;
	}
	if (key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		giroPanel4 += (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		if (giroPanel4 > 70.0f) giroPanel4 = 70.0f;
		if (giroPanel4 < -70.0f) giroPanel4 = -70.0f;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
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
