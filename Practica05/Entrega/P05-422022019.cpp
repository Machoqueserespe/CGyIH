/*
Práctica 5: Optimización y Carga de Modelos
Reporte 05 - Alan Rogelio Barrientos Ramírez - 422022019
Entrega: 26/09/2026
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
// El rover se carga desde OBJ; no genero esferas para este ejercicio.
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//Lista de Modelos a importar
Model Cuerpo_M, BaseBrazo_M, Brazo1_M, Brazo2_M, Pinza_M;
Model PataDD_M, PataDI_M, PataMD_M, PataMI_M, PataTD_M, PataTI_M;
Model RuedaDD_M, RuedaDI_M, RuedaMD_M, RuedaMI_M, RuedaTD_M, RuedaTI_M;
 
Model HolocronCuerpo_M;
Model HolocronInterior_M;
Model HolocronCristal_M;
Model HolocronEsquina1_M;
Model HolocronEsquina2_M;
Model HolocronEsquina3_M;
Model HolocronEsquina4_M;
Model HolocronEsquina5_M;
Model HolocronEsquina6_M;
Model HolocronEsquina7_M;
Model HolocronEsquina8_M;
Model SateliteCuerpoOro_M;
Model SateliteCuerpoGris_M;
Model SateliteCuerpoAzul_M;
Model SatelitePanel1Soporte_M;
Model SatelitePanel1CeldasA_M;
Model SatelitePanel1CeldasB_M;
Model SatelitePanel2Soporte_M;
Model SatelitePanel2CeldasA_M;
Model SatelitePanel2CeldasB_M;
Model SatelitePanel3Soporte_M;
Model SatelitePanel3CeldasA_M;
Model SatelitePanel3CeldasB_M;
Model SatelitePanel4Soporte_M;
Model SatelitePanel4CeldasA_M;
Model SatelitePanel4CeldasB_M;

// Conservo el Skybox y sus seis imágenes para dibujar el fondo de la escena.
Skybox skybox;
// Guardo un giro por articulación; los tramos siguientes heredan ese movimiento.
GLfloat giroBaseBrazo = 0.0f;
GLfloat giroBrazo1 = 0.0f;
GLfloat giroBrazo2 = 0.0f;
GLfloat giroPinza = 0.0f;

// Guardo un ángulo por esquina; cada una gira alrededor del centro del Holocrón.
GLfloat giroEsquina1 = 0.0f;
GLfloat giroEsquina2 = 0.0f;
GLfloat giroEsquina3 = 0.0f;
GLfloat giroEsquina4 = 0.0f;
GLfloat giroEsquina5 = 0.0f;
GLfloat giroEsquina6 = 0.0f;
GLfloat giroEsquina7 = 0.0f;
GLfloat giroEsquina8 = 0.0f;
// Estos desplazamientos se aplican a todo el satélite, incluidos sus paneles.
GLfloat sateliteX = 0.0f, sateliteY = 0.0f, sateliteZ = 0.0f;
// Los cuatro paneles tienen ángulos independientes alrededor de sus bisagras.
GLfloat giroPanel1 = 0.0f;
GLfloat giroPanel2 = 0.0f;
GLfloat giroPanel3 = 0.0f;
GLfloat giroPanel4 = 0.0f;

// Cada modelo conserva su forma; su origen está en la unión que le corresponde.

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	// Activo un VAO para validar el shader al inicio; después cada modelo usa el suyo.
	GLuint vaoValidacion = 0;
	glGenVertexArrays(1, &vaoValidacion);
	glBindVertexArray(vaoValidacion);
	CreateShaders();
	glBindVertexArray(0);

	// Alejo la cámara para mostrar los tres modelos al mismo tiempo, sin deformarlos.
	camera = Camera(glm::vec3(4.5f, 7.0f, 18.0f), glm::vec3(0.0f, 1.0f, 0.0f), -104.0f, -20.7f, 0.3f, 0.3f);
	// Cargo el cuerpo, los cuatro segmentos del brazo y las seis ruedas por separado.
	// Inicializo cada modelo con Model() antes de cargar su archivo.
	// Las patas también tienen su archivo para que cada rueda herede el giro de su soporte.
	Cuerpo_M = Model();
	Cuerpo_M.LoadModel("Models/Cuerpo.obj");
	BaseBrazo_M = Model();
	BaseBrazo_M.LoadModel("Models/BaseBrazo.obj");
	Brazo1_M = Model();
	Brazo1_M.LoadModel("Models/Brazo1.obj");
	Brazo2_M = Model();
	Brazo2_M.LoadModel("Models/Brazo2.obj");
	Pinza_M = Model();
	Pinza_M.LoadModel("Models/Pinza.obj");
	PataDD_M = Model();
	PataDD_M.LoadModel("Models/PataDD.obj");
	RuedaDD_M = Model();
	RuedaDD_M.LoadModel("Models/RuedaDD.obj");
	PataDI_M = Model();
	PataDI_M.LoadModel("Models/PataDI.obj");
	RuedaDI_M = Model();
	RuedaDI_M.LoadModel("Models/RuedaDI.obj");
	PataMD_M = Model();
	PataMD_M.LoadModel("Models/PataMD.obj");
	RuedaMD_M = Model();
	RuedaMD_M.LoadModel("Models/RuedaMD.obj");
	PataMI_M = Model();
	PataMI_M.LoadModel("Models/PataMI.obj");
	RuedaMI_M = Model();
	RuedaMI_M.LoadModel("Models/RuedaMI.obj");
	PataTD_M = Model();
	PataTD_M.LoadModel("Models/PataTD.obj");
	RuedaTD_M = Model();
	RuedaTD_M.LoadModel("Models/RuedaTD.obj");
	PataTI_M = Model();
	PataTI_M.LoadModel("Models/PataTI.obj");
	RuedaTI_M = Model();
	RuedaTI_M.LoadModel("Models/RuedaTI.obj");

	// Importo las piezas separadas y conservo la jerarquía mediante sus matrices.
	HolocronCuerpo_M = Model();
	HolocronCuerpo_M.LoadModel("Models/HolocronCuerpo.obj");
	HolocronInterior_M = Model();
	HolocronInterior_M.LoadModel("Models/HolocronInterior.obj");
	HolocronCristal_M = Model();
	HolocronCristal_M.LoadModel("Models/HolocronCristal.obj");
	HolocronEsquina1_M = Model();
	HolocronEsquina1_M.LoadModel("Models/HolocronEsquina1.obj");
	HolocronEsquina2_M = Model();
	HolocronEsquina2_M.LoadModel("Models/HolocronEsquina2.obj");
	HolocronEsquina3_M = Model();
	HolocronEsquina3_M.LoadModel("Models/HolocronEsquina3.obj");
	HolocronEsquina4_M = Model();
	HolocronEsquina4_M.LoadModel("Models/HolocronEsquina4.obj");
	HolocronEsquina5_M = Model();
	HolocronEsquina5_M.LoadModel("Models/HolocronEsquina5.obj");
	HolocronEsquina6_M = Model();
	HolocronEsquina6_M.LoadModel("Models/HolocronEsquina6.obj");
	HolocronEsquina7_M = Model();
	HolocronEsquina7_M.LoadModel("Models/HolocronEsquina7.obj");
	HolocronEsquina8_M = Model();
	HolocronEsquina8_M.LoadModel("Models/HolocronEsquina8.obj");
	SateliteCuerpoOro_M = Model();
	SateliteCuerpoOro_M.LoadModel("Models/SateliteCuerpoOro.obj");
	SateliteCuerpoGris_M = Model();
	SateliteCuerpoGris_M.LoadModel("Models/SateliteCuerpoGris.obj");
	SateliteCuerpoAzul_M = Model();
	SateliteCuerpoAzul_M.LoadModel("Models/SateliteCuerpoAzul.obj");
	SatelitePanel1Soporte_M = Model();
	SatelitePanel1Soporte_M.LoadModel("Models/SatelitePanel1Soporte.obj");
	SatelitePanel1CeldasA_M = Model();
	SatelitePanel1CeldasA_M.LoadModel("Models/SatelitePanel1CeldasA.obj");
	SatelitePanel1CeldasB_M = Model();
	SatelitePanel1CeldasB_M.LoadModel("Models/SatelitePanel1CeldasB.obj");
	SatelitePanel2Soporte_M = Model();
	SatelitePanel2Soporte_M.LoadModel("Models/SatelitePanel2Soporte.obj");
	SatelitePanel2CeldasA_M = Model();
	SatelitePanel2CeldasA_M.LoadModel("Models/SatelitePanel2CeldasA.obj");
	SatelitePanel2CeldasB_M = Model();
	SatelitePanel2CeldasB_M.LoadModel("Models/SatelitePanel2CeldasB.obj");
	SatelitePanel3Soporte_M = Model();
	SatelitePanel3Soporte_M.LoadModel("Models/SatelitePanel3Soporte.obj");
	SatelitePanel3CeldasA_M = Model();
	SatelitePanel3CeldasA_M.LoadModel("Models/SatelitePanel3CeldasA.obj");
	SatelitePanel3CeldasB_M = Model();
	SatelitePanel3CeldasB_M.LoadModel("Models/SatelitePanel3CeldasB.obj");
	SatelitePanel4Soporte_M = Model();
	SatelitePanel4Soporte_M.LoadModel("Models/SatelitePanel4Soporte.obj");
	SatelitePanel4CeldasA_M = Model();
	SatelitePanel4CeldasA_M.LoadModel("Models/SatelitePanel4CeldasA.obj");
	SatelitePanel4CeldasB_M = Model();
	SatelitePanel4CeldasB_M.LoadModel("Models/SatelitePanel4CeldasB.obj");

	// Creo el Skybox con las seis texturas del ejemplo, en el mismo orden.
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	// Aplico la misma orientación desde la primera imagen del cubo.
	stbi_set_flip_vertically_on_load(true);
	// Mantengo un VAO activo también durante la validación del shader del Skybox.
	glBindVertexArray(vaoValidacion);
	skybox = Skybox(skyboxFaces);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoValidacion);

	printf("F/G/H/J/K/L: patas DD/DI/MD/MI/TD/TI. Shift invierte el giro. Limites: -45 y 45 grados.\n");
	printf("F1/F2/F3/F4: base del brazo, primer tramo, segundo tramo y pinza. Shift invierte el giro.\n");
	printf("W/A/S/D y mouse: camara. Esc: salir.\n");
	printf("1-8: esquinas del Holocron. X/Y/Z: traslacion del satelite.\n");
	printf("U/I/O/P: paneles +X/-X/+Z/-Z. Shift invierte todos los movimientos.\n");

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	// Convierto el campo de visión a radianes y mantengo la proporción de la ventana.
	glm::mat4 projection = glm::perspective(45.0f * toRadians, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.12f, 0.16f, 0.22f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Dibujo el fondo antes de los modelos; la cámara cambia su orientación.
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.25f, 0.29f, 0.34f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		// Uso el centro del cuerpo como origen local de toda la jerarquía del rover.
		model = glm::mat4(1.0);
		// Compenso el cambio de origen para conservar la posición del rover en la escena.
		model = glm::translate(model, glm::vec3(-7.162216f, 0.309026f, -1.5f));
		// La escala es uniforme para conservar las proporciones del modelo.
		model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));
		color = glm::vec3(0.78f, 0.84f, 0.90f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_M.RenderModel();

		// Llevo la base del brazo a su unión, medida desde el centro del cuerpo.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(3.172745f, -0.337018f, -1.100001f));
		modelaux = glm::rotate(modelaux, giroBaseBrazo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.65f, 0.72f, 0.80f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		BaseBrazo_M.RenderModel();

		// Conservo la matriz de la base y avanzo al eje del primer tramo.
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 1.187502f, 0.0f));
		modelaux = glm::rotate(modelaux, giroBrazo1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.92f, 0.64f, 0.20f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		Brazo1_M.RenderModel();

		// El segundo tramo hereda los giros anteriores y gira desde el codo.
		modelaux = glm::translate(modelaux, glm::vec3(2.534587f, 2.556534f, 0.000001f));
		modelaux = glm::rotate(modelaux, giroBrazo2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.82f, 0.87f, 0.93f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		Brazo2_M.RenderModel();

		// Coloco la pinza en la muñeca; su giro no modifica los tramos anteriores.
		modelaux = glm::translate(modelaux, glm::vec3(-2.647493f, 2.945179f, 0.189120f));
		modelaux = glm::rotate(modelaux, giroPinza * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.92f, 0.64f, 0.20f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		Pinza_M.RenderModel();

		// Cada pata empieza de nuevo desde model, así su giro no afecta a las demás.
		// Mido cada pivote desde el nuevo centro del cuerpo y después giro en Z.
		// La rueda se coloca desde esa matriz: acompaña a la pata sin separarse.

		// Pata y rueda delantera derecha.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(3.571591f, -0.923169f, -3.0660f));
		modelaux = glm::rotate(modelaux, mainWindow.getarticulacion1() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.42f, 0.60f, 0.70f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PataDD_M.RenderModel();

		modelaux = glm::translate(modelaux, glm::vec3(3.6105f, -2.5025f, -0.0040f));
		color = glm::vec3(0.12f, 0.14f, 0.17f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RuedaDD_M.RenderModel();

		// Pata y rueda delantera izquierda.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(3.571591f, -0.923169f, 3.0540f));
		modelaux = glm::rotate(modelaux, mainWindow.getarticulacion2() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.42f, 0.60f, 0.70f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PataDI_M.RenderModel();

		modelaux = glm::translate(modelaux, glm::vec3(3.6115f, -2.5025f, 0.0160f));
		color = glm::vec3(0.12f, 0.14f, 0.17f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RuedaDI_M.RenderModel();

		// Pata y rueda media derecha.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-1.064409f, -1.554169f, -2.8590f));
		modelaux = glm::rotate(modelaux, mainWindow.getarticulacion3() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.42f, 0.60f, 0.70f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PataMD_M.RenderModel();

		modelaux = glm::translate(modelaux, glm::vec3(1.5900f, -2.1645f, -1.3000f));
		color = glm::vec3(0.12f, 0.14f, 0.17f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RuedaMD_M.RenderModel();

		// Pata y rueda media izquierda.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-1.064409f, -1.554169f, 2.8590f));
		modelaux = glm::rotate(modelaux, mainWindow.getarticulacion4() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.42f, 0.60f, 0.70f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PataMI_M.RenderModel();

		modelaux = glm::translate(modelaux, glm::vec3(1.5905f, -2.1645f, 1.3000f));
		color = glm::vec3(0.12f, 0.14f, 0.17f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RuedaMI_M.RenderModel();

		// Pata y rueda trasera derecha.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-2.064409f, -1.554169f, -2.8590f));
		modelaux = glm::rotate(modelaux, mainWindow.getarticulacion5() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.42f, 0.60f, 0.70f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PataTD_M.RenderModel();

		modelaux = glm::translate(modelaux, glm::vec3(-1.5780f, -2.1910f, -1.3000f));
		color = glm::vec3(0.12f, 0.14f, 0.17f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RuedaTD_M.RenderModel();

		// Pata y rueda trasera izquierda.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-2.064409f, -1.554169f, 2.8590f));
		modelaux = glm::rotate(modelaux, mainWindow.getarticulacion6() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.42f, 0.60f, 0.70f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PataTI_M.RenderModel();

		modelaux = glm::translate(modelaux, glm::vec3(-1.5955f, -2.1645f, 1.3000f));
		color = glm::vec3(0.12f, 0.14f, 0.17f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RuedaTI_M.RenderModel();

		// HOLOCRÓN: todos sus archivos comparten el mismo centro, colocado en el origen.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));
		color = glm::vec3(0.55f, 0.34f, 0.10f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HolocronCuerpo_M.RenderModel();
		color = glm::vec3(0.18f, 0.24f, 0.30f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HolocronInterior_M.RenderModel();
		color = glm::vec3(0.10f, 0.85f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HolocronCristal_M.RenderModel();

		// Esquina 1: recupero el centro para no heredar el giro de otra esquina.
		modelaux = model;
		modelaux = glm::rotate(modelaux, giroEsquina1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.92f, 0.68f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		HolocronEsquina1_M.RenderModel();

		// Esquina 2: recupero el centro para no heredar el giro de otra esquina.
		modelaux = model;
		modelaux = glm::rotate(modelaux, giroEsquina2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.92f, 0.68f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		HolocronEsquina2_M.RenderModel();

		// Esquina 3: recupero el centro para no heredar el giro de otra esquina.
		modelaux = model;
		modelaux = glm::rotate(modelaux, giroEsquina3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.92f, 0.68f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		HolocronEsquina3_M.RenderModel();

		// Esquina 4: recupero el centro para no heredar el giro de otra esquina.
		modelaux = model;
		modelaux = glm::rotate(modelaux, giroEsquina4 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.92f, 0.68f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		HolocronEsquina4_M.RenderModel();

		// Esquina 5: recupero el centro para no heredar el giro de otra esquina.
		modelaux = model;
		modelaux = glm::rotate(modelaux, giroEsquina5 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.92f, 0.68f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		HolocronEsquina5_M.RenderModel();

		// Esquina 6: recupero el centro para no heredar el giro de otra esquina.
		modelaux = model;
		modelaux = glm::rotate(modelaux, giroEsquina6 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.92f, 0.68f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		HolocronEsquina6_M.RenderModel();

		// Esquina 7: recupero el centro para no heredar el giro de otra esquina.
		modelaux = model;
		modelaux = glm::rotate(modelaux, giroEsquina7 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.92f, 0.68f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		HolocronEsquina7_M.RenderModel();

		// Esquina 8: recupero el centro para no heredar el giro de otra esquina.
		modelaux = model;
		modelaux = glm::rotate(modelaux, giroEsquina8 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.92f, 0.68f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		HolocronEsquina8_M.RenderModel();

		// SATÉLITE: esta matriz mueve el cuerpo y todas las ramas de su jerarquía.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f + sateliteX, 0.7f + sateliteY, sateliteZ));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
		color = glm::vec3(0.88f, 0.66f, 0.20f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SateliteCuerpoOro_M.RenderModel();
		color = glm::vec3(0.76f, 0.79f, 0.83f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SateliteCuerpoGris_M.RenderModel();
		color = glm::vec3(0.15f, 0.28f, 0.45f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SateliteCuerpoAzul_M.RenderModel();

		// Panel 1: traslado a su bisagra antes de rotar; el cuerpo no recibe este giro.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(149.83f, 30.01f, -1.42f));
		modelaux = glm::rotate(modelaux, giroPanel1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		// El soporte y ambas caras de las celdas conservan exactamente la misma matriz.
		color = glm::vec3(0.78f, 0.80f, 0.83f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel1Soporte_M.RenderModel();
		// Las celdas coinciden con la superficie del panel; evito el parpadeo de profundidad.
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0f, -1.0f);
		color = glm::vec3(0.12f, 0.28f, 0.60f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel1CeldasA_M.RenderModel();
		color = glm::vec3(0.07f, 0.14f, 0.29f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel1CeldasB_M.RenderModel();
		glDisable(GL_POLYGON_OFFSET_FILL);

		// Panel 2: traslado a su bisagra antes de rotar; el cuerpo no recibe este giro.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-149.83f, 30.01f, -1.42f));
		modelaux = glm::rotate(modelaux, giroPanel2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		// El soporte y ambas caras de las celdas conservan exactamente la misma matriz.
		color = glm::vec3(0.78f, 0.80f, 0.83f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel2Soporte_M.RenderModel();
		// Las celdas coinciden con la superficie del panel; evito el parpadeo de profundidad.
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0f, -1.0f);
		color = glm::vec3(0.12f, 0.28f, 0.60f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel2CeldasA_M.RenderModel();
		color = glm::vec3(0.07f, 0.14f, 0.29f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel2CeldasB_M.RenderModel();
		glDisable(GL_POLYGON_OFFSET_FILL);

		// Panel 3: traslado a su bisagra antes de rotar; el cuerpo no recibe este giro.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-2.84f, 30.01f, 146.28f));
		modelaux = glm::rotate(modelaux, giroPanel3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		// El soporte y ambas caras de las celdas conservan exactamente la misma matriz.
		color = glm::vec3(0.78f, 0.80f, 0.83f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel3Soporte_M.RenderModel();
		// Las celdas coinciden con la superficie del panel; evito el parpadeo de profundidad.
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0f, -1.0f);
		color = glm::vec3(0.12f, 0.28f, 0.60f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel3CeldasA_M.RenderModel();
		color = glm::vec3(0.07f, 0.14f, 0.29f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel3CeldasB_M.RenderModel();
		glDisable(GL_POLYGON_OFFSET_FILL);

		// Panel 4: traslado a su bisagra antes de rotar; el cuerpo no recibe este giro.
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-2.84f, 30.01f, -146.28f));
		modelaux = glm::rotate(modelaux, giroPanel4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		// El soporte y ambas caras de las celdas conservan exactamente la misma matriz.
		color = glm::vec3(0.78f, 0.80f, 0.83f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel4Soporte_M.RenderModel();
		// Las celdas coinciden con la superficie del panel; evito el parpadeo de profundidad.
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0f, -1.0f);
		color = glm::vec3(0.12f, 0.28f, 0.60f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel4CeldasA_M.RenderModel();
		color = glm::vec3(0.07f, 0.14f, 0.29f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		SatelitePanel4CeldasB_M.RenderModel();
		glDisable(GL_POLYGON_OFFSET_FILL);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
