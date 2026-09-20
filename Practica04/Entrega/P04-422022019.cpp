/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Práctica de laboratorio: Rover y sonda espacial.
F1: rover. F2: sonda. Ambos ejercicios se dibujan desde este mismo main.
F: giro de la base del brazo. G: hombro. H: codo. J: muñeca.
K: abrir la pinza. L: articulación de la suspensión frontal.
1, 2, 3: ruedas frontal, central y trasera del lado Z negativo.
4, 5, 6: ruedas frontal, central y trasera del lado Z positivo.
U e I: paneles de la sonda. O: antena. E, R y T: giro completo de la sonda.
Con Shift y la misma tecla invierto el sentido. WASD y ratón controlan la cámara.
*/
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
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
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
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
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

	// Conservo los vértices y conecto las paredes y las tapas con triángulos.
	// Antes: for (i = 0; i < vertices.size(); i++) indices.push_back(i);
	// vertices.size() cuenta coordenadas, no vértices; así evito índices fuera del arreglo.
	for (i = 0; i < res; i++) {
		indices.push_back(2 * i); indices.push_back(2 * i + 1); indices.push_back(2 * i + 2);
		indices.push_back(2 * i + 1); indices.push_back(2 * i + 3); indices.push_back(2 * i + 2);
	}
	for (i = 1; i < res - 1; i++) {
		indices.push_back(2 * (res + 1));
		indices.push_back(2 * (res + 1) + i + 1);
		indices.push_back(2 * (res + 1) + i);
		indices.push_back(3 * (res + 1));
		indices.push_back(3 * (res + 1) + i);
		indices.push_back(3 * (res + 1) + i + 1);
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
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(1000, 800); // Amplío la ventana para ver el rover completo.
	if (mainWindow.Initialise() != 0) return 1;
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(32, 1.0f);// Uso más segmentos para que ruedas y brazos se vean redondos.
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	// Dejo un VAO activo durante la validación del programa en el perfil core.
	GLuint vaoValidacion;
	glGenVertexArrays(1, &vaoValidacion);
	glBindVertexArray(vaoValidacion);
	CreateShaders();
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoValidacion);
	

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

	// Alejo la cámara y la dirijo al centro del rover; no cambio sus proporciones.
	camera = Camera(glm::vec3(-12.0f, 13.0f, 18.0f), glm::vec3(0.0f, 1.0f, 0.0f), -63.435f, -15.885f, 0.3f, 0.3f);

	
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
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux2(1.0);

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;
	bool mostrarSonda = false;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		// Cambio de ejercicio sin abrir otra ventana ni usar un segundo main.
		if (mainWindow.getsKeys()[GLFW_KEY_F1]) {
			mostrarSonda = false;
			camera = Camera(glm::vec3(-12.0f, 13.0f, 18.0f), glm::vec3(0, 1, 0), -63.435f, -15.885f, 0.3f, 0.3f);
		}
		if (mainWindow.getsKeys()[GLFW_KEY_F2]) {
			mostrarSonda = true;
			camera = Camera(glm::vec3(-7.0f, 11.0f, 9.0f), glm::vec3(0, 1, 0), -61.70f, -18.70f, 0.3f, 0.3f);
		}
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.09f, 0.12f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		// La misma proyección conserva la relación de aspecto en los dos ejercicios.
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		if (!mostrarSonda) {
		// Marco las uniones con esferas. Guardo cada unión antes de escalar la pieza.
		model = glm::mat4(1.0);
	
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -4.0f)); //NOS POSICIONAMOS EN EL CENTRO DEL OBJETO X AMARILLA 
		modelaux = model; //guardamos la matriz de modelo para que la base se mueva con el objeto
		// Cabina de 8 por 4 por 6 unidades, conectada a la base.
		model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f));//PARA LLEGAR AL CENTRO DE LA CABINA a PARTIR DEL ORIGEN
		model = glm::scale(model, glm::vec3(8.0f, 4.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.76f, 0.79f, 0.82f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera


		// BASE
		// model = glm::mat4(1.0); // Recupero modelaux para conservar el origen del rover.
		/*En su lugar usamos la matriz auxiliar modelaux para que la base se mueva con el objeto.
		Lo que debemos de saber es: de las transformaciones geométricas que se aplican a la cabina y 
		cuales queremos que se apliquen a la base. En este caso, sólo queremos que se aplique la traslación del origen, no la rotación ni el escalado de la cabina. 
		Por lo tanto, debemos de guardar en modelaux sólo la traslación del origen y luego aplicarla a la base.
		*/
		model = modelaux;
		//NOS POSICIONAMOS EN EL CENTRO DE LA BASE
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));
		modelaux = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		model = glm::scale(model, glm::vec3(10.0f, 1.5f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.29f, 0.35f, 0.41f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();
		
		model = modelaux;
		modelaux2 = model; //guardamos la matriz de modelo para conectar los siguientes objetos a la base
		// modelaux2 conserva la base sin su escala. Regreso aquí al iniciar cada rama.
		// modelaux guarda la articulación actual y model se usa para dibujar cada pieza.
		// No heredo la escala de un cilindro a la esfera o a la siguiente articulación.

		// Panel superior de la cabina: otra instancia del mismo cubo.
		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.0f, 4.80f, 0.0f));
		model = glm::scale(model, glm::vec3(7.4f, 0.10f, 5.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.08f, 0.25f, 0.40f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		for (int panel = 0; panel < 5; panel++) {
			model = modelaux2;
			model = glm::translate(model, glm::vec3(-1.8f + panel * 1.4f, 4.86f, 0.0f));
			model = glm::scale(model, glm::vec3(0.04f, 0.02f, 5.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.28f, 0.57f, 0.71f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMesh();
		}

		// Los ejes atraviesan la base y llegan a las uniones de ambos lados.
		for (int eje = 0; eje < 3; eje++) {
			model = modelaux2;
			model = glm::translate(model, glm::vec3(eje == 0 ? -4.2f : (eje == 1 ? 0.3f : 1.55f), eje == 0 ? -0.05f : -0.2f, 0.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.16f, 9.0f, 0.16f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.62f, 0.68f, 0.72f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();
		}

		// Repito las tres ruedas en ambos lados: lado -1 y lado 1 sobre Z.
		for (int lado = -1; lado <= 1; lado += 2) {
			// Panel lateral; su escala no afecta la suspensión.
			model = modelaux2;
			model = glm::translate(model, glm::vec3(1.0f, 2.75f, lado * 3.03f));
			model = glm::scale(model, glm::vec3(6.6f, 1.80f, 0.06f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.16f, 0.33f, 0.44f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMesh();

			for (int rueda = 0; rueda < 3; rueda++) {
				model = modelaux2;
				if (rueda == 0) {
					// Eje frontal -> barra -> unión -> los dos tramos de la pata en L -> llanta.
					model = glm::translate(model, glm::vec3(-4.2f, -0.05f, lado * 4.5f));
					model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
					modelaux = model;
					model = glm::scale(model, glm::vec3(0.28f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.97f, 0.52f, 0.12f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					sp.render();

					model = modelaux;
					// Dejo la unión por encima de la llanta para que se distingan ambos tramos de la L.
					model = glm::translate(model, glm::vec3(-1.45f, -0.25f, 0.0f));
					model = glm::rotate(model, glm::radians(99.78241f), glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::scale(model, glm::vec3(0.15f, 2.94279f, 0.15f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.62f, 0.68f, 0.72f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					meshList[2]->RenderMesh(); // El cilindro ahora usa índices de triángulos.

					model = modelaux;
					model = glm::translate(model, glm::vec3(-2.9f, -0.5f, 0.0f));
					modelaux = model;
					model = glm::scale(model, glm::vec3(0.24f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.97f, 0.52f, 0.12f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					sp.render();

					model = modelaux;
					// Primer tramo de la L frontal: desde la unión hasta el doblez.
					model = glm::translate(model, glm::vec3(-0.3f, 0.0f, 0.0f));
					model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::scale(model, glm::vec3(0.17f, 0.6f, 0.17f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.62f, 0.68f, 0.72f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					meshList[2]->RenderMesh();

					// Segundo tramo perpendicular: baja desde el doblez al centro de la llanta.
					model = glm::translate(modelaux, glm::vec3(-0.6f, -0.75f, 0.0f));
					model = glm::scale(model, glm::vec3(0.17f, 1.5f, 0.17f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.62f, 0.68f, 0.72f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					meshList[2]->RenderMesh();
					model = glm::translate(modelaux, glm::vec3(-0.6f, -1.5f, 0.0f));
					modelaux = model; // Centro de la rueda frontal, unido a toda su rama.
				}
				else {
					// Eje central o trasero -> tramo corto -> doblez -> tramo largo -> llanta.
					model = glm::translate(model, glm::vec3(rueda == 1 ? 0.3f : 1.55f, -0.2f, lado * 4.5f));
					modelaux = model;
					model = glm::scale(model, glm::vec3(0.28f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.97f, 0.52f, 0.12f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					sp.render();

					model = modelaux;
					// Formo una L inclinada con dos cilindros perpendiculares, sin nuevas mallas.
					// Las longitudes conservan el centro anterior de la rueda: (-1.9,-1.85) o (2.35,-1.85).
					model = glm::translate(model, glm::vec3(rueda == 1 ? -0.31196325f : 0.48071325f, rueda == 1 ? 0.18011207f : 0.27753992f, 0.0f));
					model = glm::rotate(model, glm::radians(rueda == 1 ? 60.0f : -60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::scale(model, glm::vec3(0.18f, rueda == 1 ? 0.72044827f : 1.11015970f, 0.18f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.62f, 0.68f, 0.72f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					meshList[2]->RenderMesh();

					// Recupero la matriz sin escala y dibujo el segundo tramo desde el mismo doblez.
					model = glm::translate(modelaux, glm::vec3(rueda == 1 ? -1.26196325f : 1.65571325f, rueda == 1 ? -0.74488793f : -0.64746008f, 0.0f));
					model = glm::rotate(model, glm::radians(rueda == 1 ? 150.0f : -150.0f), glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::scale(model, glm::vec3(0.18f, rueda == 1 ? 2.55214700f : 2.77714700f, 0.18f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.62f, 0.68f, 0.72f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					meshList[2]->RenderMesh();
					model = glm::translate(modelaux, glm::vec3(rueda == 1 ? -1.9f : 2.35f, -1.85f, 0.0f));
					modelaux = model;
				}

				// Giro alrededor del centro de esta llanta, antes de escalarla.
				// Rin, radios y centro heredan el mismo giro; la suspensión no lo hereda.
				int numeroRueda = (lado == -1 ? 0 : 3) + rueda;
				modelaux = glm::rotate(modelaux, glm::radians(mainWindow.getrueda(numeroRueda)), glm::vec3(0.0f, 0.0f, 1.0f));
				// Llanta, rin y centro: tres instancias del cilindro, orientadas sobre Z.
				model = glm::rotate(modelaux, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1.2f, 0.82f, 1.2f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(0.07f, 0.08f, 0.09f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				meshList[2]->RenderMesh();

				model = glm::rotate(modelaux, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.84f, 0.88f, 0.84f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(0.68f, 0.73f, 0.76f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				meshList[2]->RenderMesh();

				for (int radio = 0; radio < 6; radio++) {
					model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, lado * 0.46f));
					model = glm::rotate(model, glm::radians(radio * 60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
					model = glm::translate(model, glm::vec3(0.0f, 0.46f, 0.0f));
					model = glm::scale(model, glm::vec3(0.09f, 0.62f, 0.035f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					// Un radio naranja permite distinguir el giro completo de cada rueda.
					color = radio == 0 ? glm::vec3(0.97f, 0.52f, 0.12f) : glm::vec3(0.22f, 0.29f, 0.35f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					meshList[0]->RenderMesh();
				}
				model = glm::rotate(modelaux, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.23f, 0.98f, 0.23f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(0.97f, 0.52f, 0.12f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				meshList[2]->RenderMesh();
			}
		}

		// BASE DEL BRAZO: inicio otra rama desde la base del rover, sin tomar la de las ruedas.
		model = glm::translate(modelaux2, glm::vec3(-4.15f, 0.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.45f, 0.0f));
		model = glm::scale(model, glm::vec3(0.47f, 0.9f, 0.47f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.31f, 0.39f, 0.45f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		// HOMBRO: el giro se hereda al brazo 1, al codo, al brazo 2 y a la pinza.
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.9f, 0.0f));
		model = glm::rotate(model, glm::radians(43.0f + mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.97f, 0.52f, 0.12f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		model = glm::translate(modelaux, glm::vec3(0.0f, 1.725f, 0.0f));
		model = glm::scale(model, glm::vec3(0.21f, 3.45f, 0.21f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.82f, 0.85f, 0.87f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		// CODO: avanzo exactamente la longitud del brazo 1 para conservar la unión.
		model = glm::translate(modelaux, glm::vec3(0.0f, 3.45f, 0.0f));
		model = glm::rotate(model, glm::radians(-88.0f + mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.97f, 0.52f, 0.12f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		model = glm::translate(modelaux, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 4.0f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.82f, 0.85f, 0.87f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		// MUÑECA: la pinza parte del extremo del segundo brazo.
		model = glm::translate(modelaux, glm::vec3(0.0f, 4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-12.0f + mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.27f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.97f, 0.52f, 0.12f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.5f, 0.17f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.31f, 0.39f, 0.45f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.57f, 0.0f));
		model = glm::scale(model, glm::vec3(0.95f, 0.18f, 0.45f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Las dos mordazas parten de la misma muñeca y abren en sentidos opuestos.
		for (int mordaza = -1; mordaza <= 1; mordaza += 2) {
			model = glm::translate(modelaux, glm::vec3(mordaza * 0.38f, 0.66f, 0.0f));
			model = glm::rotate(model, glm::radians(-mordaza * mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.38f, 0.0f));
			model = glm::scale(model, glm::vec3(0.14f, 0.76f, 0.28f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.97f, 0.52f, 0.12f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMesh();
			model = glm::translate(modelaux, glm::vec3(mordaza * 0.38f, 0.66f, 0.0f));
			model = glm::rotate(model, glm::radians(-mordaza * mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-mordaza * 0.10f, 0.76f, 0.0f));
			model = glm::scale(model, glm::vec3(0.34f, 0.14f, 0.28f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
		}

		} else {
			// SONDA: guardo el cuerpo central sin escala como raíz de todas las ramas.
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 6.0f, -4.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1, 0, 0));
			model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0, 1, 0));
			model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0, 0, 1));
			modelaux2 = model;
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.83f, 0.61f, 0.20f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMesh();

			// Bandas del cuerpo y propulsores: regresan siempre a la raíz.
			for (int nivel = -1; nivel <= 1; nivel += 2) {
				model = glm::translate(modelaux2, glm::vec3(0, nivel * 1.3f, 0));
				model = glm::scale(model, glm::vec3(3.08f, 0.16f, 3.08f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(0.27f, 0.31f, 0.35f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				meshList[0]->RenderMesh();
			}
			for (int x = -1; x <= 1; x += 2) {
				for (int z = -1; z <= 1; z += 2) {
					modelaux = glm::translate(modelaux2, glm::vec3(x * 0.9f, -1.85f, z * 0.9f));
					model = glm::scale(modelaux, glm::vec3(0.38f, 0.70f, 0.38f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.26f, 0.30f, 0.34f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					meshList[2]->RenderMesh();
					model = glm::translate(modelaux, glm::vec3(0, -0.36f, 0));
					model = glm::scale(model, glm::vec3(0.27f, 0.03f, 0.27f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					color = glm::vec3(0.08f, 0.10f, 0.12f);
					glUniform3fv(uniformColor, 1, glm::value_ptr(color));
					meshList[2]->RenderMesh();
				}
			}

			// Cuerpo -> bisagra -> soporte -> panel y celdas. Un panel no afecta al otro.
			for (int lado = -1; lado <= 1; lado += 2) {
				model = glm::translate(modelaux2, glm::vec3(lado * 1.5f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(lado * (lado == -1 ? mainWindow.getpanel1() : mainWindow.getpanel2())), glm::vec3(0, 0, 1));
				modelaux = model;
				model = glm::scale(model, glm::vec3(0.26f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(0.88f, 0.90f, 0.92f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				sp.render();
				model = glm::translate(modelaux, glm::vec3(lado * 0.55f, 0, 0));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
				model = glm::scale(model, glm::vec3(0.10f, 1.1f, 0.10f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[2]->RenderMesh();
				modelaux = glm::translate(modelaux, glm::vec3(lado * 3.60f, 0, 0));
				model = glm::scale(modelaux, glm::vec3(5.2f, 0.16f, 3.4f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(0.52f, 0.59f, 0.66f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				meshList[0]->RenderMesh();
				// Repito cubos planos para las celdas, todos sujetos al mismo panel.
				for (int fila = 0; fila < 3; fila++) {
					for (int columna = 0; columna < 6; columna++) {
						model = glm::translate(modelaux, glm::vec3(-2.125f + columna * 0.85f, 0.10f, -1.05f + fila * 1.05f));
						model = glm::scale(model, glm::vec3(0.79f, 0.06f, 0.97f));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
						color = glm::vec3(0.08f, 0.27f + fila * 0.04f, 0.56f);
						glUniform3fv(uniformColor, 1, glm::value_ptr(color));
						meshList[0]->RenderMesh();
					}
				}
			}

			// Cuerpo -> soporte articulado -> plato -> receptor de la antena.
			model = glm::translate(modelaux2, glm::vec3(0, 1.5f, 0));
			model = glm::rotate(model, glm::radians(mainWindow.getantena()), glm::vec3(0, 0, 1));
			modelaux = model;
			model = glm::scale(model, glm::vec3(0.28f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.85f, 0.88f, 0.90f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();
			model = glm::translate(modelaux, glm::vec3(0, 0.65f, 0));
			model = glm::scale(model, glm::vec3(0.16f, 1.3f, 0.16f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[2]->RenderMesh();
			modelaux = glm::translate(modelaux, glm::vec3(0, 1.3f, 0));
			model = glm::scale(modelaux, glm::vec3(1.9f, 0.22f, 1.9f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			sp.render();
			model = glm::translate(modelaux, glm::vec3(0, 0.70f, 0));
			model = glm::scale(model, glm::vec3(0.06f, 1.4f, 0.06f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.35f, 0.40f, 0.44f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();
			model = glm::translate(modelaux, glm::vec3(0, 1.40f, 0));
			model = glm::scale(model, glm::vec3(0.20f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.96f, 0.57f, 0.16f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();

			// Cuerpo -> brazo de instrumentos -> sensor, en una rama independiente.
			modelaux = glm::translate(modelaux2, glm::vec3(0, 0, 1.5f));
			model = glm::translate(modelaux, glm::vec3(0, 0, 1.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
			model = glm::scale(model, glm::vec3(0.10f, 2.0f, 0.10f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.70f, 0.75f, 0.79f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();
			modelaux = glm::translate(modelaux, glm::vec3(0, 0, 2.3f));
			model = glm::scale(modelaux, glm::vec3(0.90f, 0.75f, 0.60f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
			model = glm::translate(modelaux, glm::vec3(0, 0, 0.34f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
			model = glm::scale(model, glm::vec3(0.22f, 0.08f, 0.22f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.04f, 0.12f, 0.20f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMesh();
		}

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		
