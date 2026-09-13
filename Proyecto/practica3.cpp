// Practica 03 - Cohete espacial y union de ocho piramides.
// Alan Rogelio Barrientos Ramirez - 422022019

//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
// Uso MeshColor para asignar un color distinto a cada cara de la piramide.
MeshColor piramideColor;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(piramidet);

}


//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,//frontal
		3,2,4,//izquierda
		2,1,4,//trasera
		1,0,4,//derecha
		0,1,2,//abajo1
		0,2,3//abajo2

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramidec = new Mesh();
	piramidec->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramidec);

	// Repito los vertices por cara para asignarles un color uniforme.
	// Conservo las posiciones de la piramide y doy el mismo RGB a cada triangulo.
	// Orden: frontal roja, izquierda verde, trasera amarilla,
	// derecha magenta y los dos triangulos de la base azul.
	GLfloat verticesPiramideColor[] = {
		0.50000f, -0.50000f, 0.50000f, 1.00000f, 0.00000f, 0.00000f,
		-0.50000f, -0.50000f, 0.50000f, 1.00000f, 0.00000f, 0.00000f,
		0.00000f, 0.50000f, 0.00000f, 1.00000f, 0.00000f, 0.00000f,
		-0.50000f, -0.50000f, 0.50000f, 0.00000f, 1.00000f, 0.00000f,
		-0.50000f, -0.50000f, -0.50000f, 0.00000f, 1.00000f, 0.00000f,
		0.00000f, 0.50000f, 0.00000f, 0.00000f, 1.00000f, 0.00000f,
		-0.50000f, -0.50000f, -0.50000f, 1.00000f, 1.00000f, 0.00000f,
		0.50000f, -0.50000f, -0.50000f, 1.00000f, 1.00000f, 0.00000f,
		0.00000f, 0.50000f, 0.00000f, 1.00000f, 1.00000f, 0.00000f,
		0.50000f, -0.50000f, -0.50000f, 1.00000f, 0.00000f, 1.00000f,
		0.50000f, -0.50000f, 0.50000f, 1.00000f, 0.00000f, 1.00000f,
		0.00000f, 0.50000f, 0.00000f, 1.00000f, 0.00000f, 1.00000f,
		0.50000f, -0.50000f, 0.50000f, 0.00000f, 0.00000f, 1.00000f,
		0.50000f, -0.50000f, -0.50000f, 0.00000f, 0.00000f, 1.00000f,
		-0.50000f, -0.50000f, -0.50000f, 0.00000f, 0.00000f, 1.00000f,
		0.50000f, -0.50000f, 0.50000f, 0.00000f, 0.00000f, 1.00000f,
		-0.50000f, -0.50000f, -0.50000f, 0.00000f, 0.00000f, 1.00000f,
		-0.50000f, -0.50000f, 0.50000f, 0.00000f, 0.00000f, 1.00000f,
	};
	piramideColor.CreateMeshColor(verticesPiramideColor, 108);
}



/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	// Corrijo los indices para recorrer vertices, no componentes del arreglo.
	// Antes: for (i = 0; i < vertices.size(); i++) indices.push_back(i);
	// Formo dos triangulos por sector para dibujar la pared con RenderMesh.
	for (i = 0; i < res; i++) {
		indices.push_back(2 * i); indices.push_back(2 * i + 1); indices.push_back(2 * i + 2);
		indices.push_back(2 * i + 1); indices.push_back(2 * i + 3); indices.push_back(2 * i + 2);
	}
	// Tapas: cada circunferencia ya esta en el arreglo; no hacen falta vertices nuevos.
	for (i = 1; i < res - 1; i++) {
		indices.push_back(2 * (res + 1)); indices.push_back(2 * (res + 1) + i); indices.push_back(2 * (res + 1) + i + 1);
		indices.push_back(3 * (res + 1)); indices.push_back(3 * (res + 1) + i + 1); indices.push_back(3 * (res + 1) + i);
	}

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	// Quito este punto extra porque el ciclo ya cierra la circunferencia.
	// vertices.push_back(R * cos(0) * dt);
	// vertices.push_back(-0.5);
	// vertices.push_back(R * sin(0) * dt);


	// Formo los triangulos laterales y agrego la tapa para cerrar el cono.
	// Antes: for (i = 0; i < res+2; i++) indices.push_back(i);
	for (i = 0; i < res; i++) {
		indices.push_back(0); indices.push_back(i + 2); indices.push_back(i + 1);
	}
	for (i = 1; i < res - 1; i++) {
		indices.push_back(1); indices.push_back(i + 1); indices.push_back(i + 2);
	}

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	// Uso el total de indices de las caras y la tapa.
	// Antes: cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cono);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	// Detengo el programa si falla la inicializacion de la ventana.
	// Antes: mainWindow.Initialise();
	if (mainWindow.Initialise() != 0) return 1;
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	// Aumento a 48 sectores para suavizar el contorno del cilindro.
	// Antes: CrearCilindro(5, 1.0f);
	CrearCilindro(48, 1.0f);//índice 2 en MeshList
	// Uso 48 sectores y radio 1 para ajustar el cono con la escala de cada pieza.
	// Antes: CrearCono(25, 2.0f);
	CrearCono(48, 1.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	// Cambio de escena con 1, 2 y 3 para mostrar ambos ejercicios en el mismo main.
	int escena = 1;
	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		if (mainWindow.getsKeys()[GLFW_KEY_1]) escena = 1;
		if (mainWindow.getsKeys()[GLFW_KEY_2]) escena = 2;
		if (mainWindow.getsKeys()[GLFW_KEY_3]) escena = 3;
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		// Uso un fondo azul oscuro para distinguir las piezas claras.
		// Antes: glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(0.055f, 0.070f, 0.105f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		// Repito el bloque de transformaciones y dibujo para cada pieza.
		// Ajusto el tamano con model y conservo la perspectiva y relacion de aspecto.
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		if (escena == 1) {
			// 20 instancias: 5 cilindros, 7 conos, 2 cubos, 2 esferas y 4 piramides.
			// Todas las mallas parten del origen. Las transformaciones posicionan las piezas.

			// CILINDRO 01 - fuselaje central
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, 0.15000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.48000f, 1.95000f, 0.48000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.84000f, 0.88000f, 0.93000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();

			// CILINDRO 02 - banda superior
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, 1.05000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.50000f, 0.13000f, 0.50000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.93000f, 0.28000f, 0.08000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();

			// CILINDRO 03 - banda inferior
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, -0.83000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.51000f, 0.15000f, 0.51000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.28000f, 0.34000f, 0.42000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();

			// CONO 01 - punta del cohete
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, 1.57500f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.48000f, 0.90000f, 0.48000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.94000f, 0.32000f, 0.08000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[3]->RenderMesh();

			// CONO 02 - tobera central
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, -1.05500f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.31000f, 0.35000f, 0.31000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.18000f, 0.23000f, 0.29000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[3]->RenderMesh();

			// CILINDRO 04 - propulsor izquierdo
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.77000f, -0.47000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.20000f, 1.35000f, 0.20000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.76000f, 0.82000f, 0.90000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();

			// CILINDRO 05 - propulsor derecho
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.77000f, -0.47000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.20000f, 1.35000f, 0.20000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.76000f, 0.82000f, 0.90000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();

			// CONO 03 - punta del propulsor izquierdo
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.77000f, 0.47000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.20000f, 0.53000f, 0.20000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.93000f, 0.28000f, 0.08000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[3]->RenderMesh();

			// CONO 04 - punta del propulsor derecho
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.77000f, 0.47000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.20000f, 0.53000f, 0.20000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.93000f, 0.28000f, 0.08000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[3]->RenderMesh();

			// CUBO 01 - soporte del propulsor izquierdo
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.53000f, -0.30000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.45000f, 0.16000f, 0.22000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.28000f, 0.34000f, 0.42000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMesh();

			// CUBO 02 - soporte del propulsor derecho
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.53000f, -0.30000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.45000f, 0.16000f, 0.22000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.28000f, 0.34000f, 0.42000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMesh();

			// ESFERA 01 - marco de la ventanilla
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, 0.45000f, 0.47800f));
			model = glm::scale(model, glm::vec3(0.24000f, 0.24000f, 0.11000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.23000f, 0.30000f, 0.40000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();

			// ESFERA 02 - cristal de la ventanilla
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, 0.45000f, 0.56500f));
			model = glm::scale(model, glm::vec3(0.17000f, 0.17000f, 0.04500f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.13000f, 0.72000f, 0.96000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();

			// PIRAMIDE 01 - aleta delantera
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, -0.60000f, 0.47000f));
			model = glm::scale(model, glm::vec3(0.63000f, 1.15000f, 0.29000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.93000f, 0.28000f, 0.08000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[4]->RenderMesh();

			// PIRAMIDE 02 - aleta posterior
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, -0.60000f, -0.47000f));
			model = glm::rotate(model, glm::radians(180.00000f), glm::vec3(0.00000f, 1.00000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.63000f, 1.15000f, 0.29000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.93000f, 0.28000f, 0.08000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[4]->RenderMesh();

			// PIRAMIDE 03 - aleta izquierda
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.47000f, -0.60000f, 0.00000f));
			model = glm::rotate(model, glm::radians(90.00000f), glm::vec3(0.00000f, 1.00000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.63000f, 1.15000f, 0.29000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.93000f, 0.28000f, 0.08000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[4]->RenderMesh();

			// PIRAMIDE 04 - aleta derecha
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.47000f, -0.60000f, 0.00000f));
			model = glm::rotate(model, glm::radians(-90.00000f), glm::vec3(0.00000f, 1.00000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.63000f, 1.15000f, 0.29000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.93000f, 0.28000f, 0.08000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[4]->RenderMesh();

			// CONO 05 - llama central
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.00000f, -1.63500f, 0.00000f));
			model = glm::rotate(model, glm::radians(180.00000f), glm::vec3(1.00000f, 0.00000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.28000f, 0.81000f, 0.28000f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.00000f, 0.58000f, 0.05000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[3]->RenderMesh();

			// CONO 06 - llama izquierda
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.77000f, -1.47000f, 0.00000f));
			model = glm::rotate(model, glm::radians(180.00000f), glm::vec3(1.00000f, 0.00000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.18500f, 0.65000f, 0.18500f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.00000f, 0.72000f, 0.12000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[3]->RenderMesh();

			// CONO 07 - llama derecha
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.80000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.77000f, -1.47000f, 0.00000f));
			model = glm::rotate(model, glm::radians(180.00000f), glm::vec3(1.00000f, 0.00000f, 0.00000f));
			model = glm::scale(model, glm::vec3(0.18500f, 0.65000f, 0.18500f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.00000f, 0.72000f, 0.12000f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[3]->RenderMesh();
		}

		if (escena == 2) {
			// Acomodo cuatro piramides arriba y cuatro abajo para cerrar el rombo.
			// Inclino cada piramide: la base sigue cuadrada y la punta queda sobre una esquina.
			// Para conseguirlo uso giro, escala y giro, que equivalen a un cizallamiento:
			// x' = x - y/sqrt(2), y' = y, z' = z.
			// Los valores 1.41421356 y 0.70710678 son sqrt(2) y 1/sqrt(2).
			// El angulo 35.26438968 es atan(1/sqrt(2)); 54.73561032 es su complementario.
			// La matriz local combina Rz(-35.26438968), S(sqrt(2),1/sqrt(2),1)
			// y Rz(54.73561032). Los giros en Y orientan la inclinacion al cuadrante.
			// Traslado las superiores con (+/-0.25, 0.5, +/-0.25).
			// Sus bases de lado 1 cubren Y = 0 y comparten la punta (0, 1, 0).
			// Giro las otras cuatro 180 grados en Z para compartir la punta (0, -1, 0).
			// Las bases azules quedan dentro; uso la escena 3 para revisar todos los colores.
			shaderList[1].useShader();
			uniformModel = shaderList[1].getModelLocation();
			uniformProjection = shaderList[1].getProjectLocation();
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

			// PIRAMIDE 1/8: ARRIBA, base en el cuadrante X-, Z+.
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax() + 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay() - 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.25000000f, 0.5f, 0.25000000f));
			model = glm::rotate(model, glm::radians(-135.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Inclinar la punta sin alterar los vertices originales de la malla.
			model = glm::rotate(model, glm::radians(-35.26438968f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.41421356f, 0.70710678f, 1.0f));
			model = glm::rotate(model, glm::radians(54.73561032f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(135.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Orientacion de los colores de esta instancia.
			model = glm::rotate(model, glm::radians(0.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();

			// PIRAMIDE 2/8: ARRIBA, base en el cuadrante X+, Z+.
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax() + 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay() - 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.25000000f, 0.5f, 0.25000000f));
			model = glm::rotate(model, glm::radians(-45.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Inclinar la punta sin alterar los vertices originales de la malla.
			model = glm::rotate(model, glm::radians(-35.26438968f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.41421356f, 0.70710678f, 1.0f));
			model = glm::rotate(model, glm::radians(54.73561032f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(45.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Orientacion de los colores de esta instancia.
			model = glm::rotate(model, glm::radians(90.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();

			// PIRAMIDE 3/8: ARRIBA, base en el cuadrante X-, Z-.
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax() + 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay() - 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.25000000f, 0.5f, -0.25000000f));
			model = glm::rotate(model, glm::radians(135.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Inclinar la punta sin alterar los vertices originales de la malla.
			model = glm::rotate(model, glm::radians(-35.26438968f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.41421356f, 0.70710678f, 1.0f));
			model = glm::rotate(model, glm::radians(54.73561032f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(-135.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Orientacion de los colores de esta instancia.
			model = glm::rotate(model, glm::radians(270.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();

			// PIRAMIDE 4/8: ARRIBA, base en el cuadrante X+, Z-.
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax() + 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay() - 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.25000000f, 0.5f, -0.25000000f));
			model = glm::rotate(model, glm::radians(45.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Inclinar la punta sin alterar los vertices originales de la malla.
			model = glm::rotate(model, glm::radians(-35.26438968f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.41421356f, 0.70710678f, 1.0f));
			model = glm::rotate(model, glm::radians(54.73561032f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(-45.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Orientacion de los colores de esta instancia.
			model = glm::rotate(model, glm::radians(180.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();

			// PIRAMIDE 5/8: ABAJO, base en el cuadrante X+, Z+.
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax() + 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay() - 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.25000000f, 0.5f, 0.25000000f));
			model = glm::rotate(model, glm::radians(-135.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Inclinar la punta sin alterar los vertices originales de la malla.
			model = glm::rotate(model, glm::radians(-35.26438968f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.41421356f, 0.70710678f, 1.0f));
			model = glm::rotate(model, glm::radians(54.73561032f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(135.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Orientacion de los colores de esta instancia.
			model = glm::rotate(model, glm::radians(180.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();

			// PIRAMIDE 6/8: ABAJO, base en el cuadrante X-, Z+.
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax() + 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay() - 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.25000000f, 0.5f, 0.25000000f));
			model = glm::rotate(model, glm::radians(-45.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Inclinar la punta sin alterar los vertices originales de la malla.
			model = glm::rotate(model, glm::radians(-35.26438968f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.41421356f, 0.70710678f, 1.0f));
			model = glm::rotate(model, glm::radians(54.73561032f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(45.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Orientacion de los colores de esta instancia.
			model = glm::rotate(model, glm::radians(270.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();

			// PIRAMIDE 7/8: ABAJO, base en el cuadrante X+, Z-.
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax() + 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay() - 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.25000000f, 0.5f, -0.25000000f));
			model = glm::rotate(model, glm::radians(135.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Inclinar la punta sin alterar los vertices originales de la malla.
			model = glm::rotate(model, glm::radians(-35.26438968f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.41421356f, 0.70710678f, 1.0f));
			model = glm::rotate(model, glm::radians(54.73561032f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(-135.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Orientacion de los colores de esta instancia.
			model = glm::rotate(model, glm::radians(90.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();

			// PIRAMIDE 8/8: ABAJO, base en el cuadrante X-, Z-.
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax() + 15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay() - 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.25000000f, 0.5f, -0.25000000f));
			model = glm::rotate(model, glm::radians(45.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Inclinar la punta sin alterar los vertices originales de la malla.
			model = glm::rotate(model, glm::radians(-35.26438968f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.41421356f, 0.70710678f, 1.0f));
			model = glm::rotate(model, glm::radians(54.73561032f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(-45.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			// Orientacion de los colores de esta instancia.
			model = glm::rotate(model, glm::radians(0.00000000f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();
		}


		if (escena == 3) {
			// Muestro una piramide por separado para revisar sus cuatro caras y la base azul.
			shaderList[1].useShader();
			uniformModel = shaderList[1].getModelLocation();
			uniformProjection = shaderList[1].getProjectLocation();
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.00000f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			piramideColor.RenderMeshColor();
		}

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		
