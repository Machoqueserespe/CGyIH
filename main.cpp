#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glew.h>
#include <glfw3.h>

const int WIDTH = 1000, HEIGHT = 700;
GLuint VAO, VBO, shader;

static const char* vShader = "                         \n\
#version 330 core                                  \n\
layout (location = 0) in vec3 pos;                 \n\
void main()                                        \n\
{                                                  \n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f); \n\
}";

static const char* fShader = "                     \n\
#version 330 core                                  \n\
out vec4 color;                                    \n\
void main()                                        \n\
{                                                  \n\
    color = vec4(1.0f, 0.85f, 0.10f, 1.0f);       \n\
}";

void CrearLetras()
{
    GLfloat vertices[] = {
        -0.90f,-0.55f,0.0f, -0.78f,-0.55f,0.0f, -0.61f,0.55f,0.0f,
        -0.90f,-0.55f,0.0f, -0.61f,0.55f,0.0f, -0.67f,0.55f,0.0f,
        -0.61f,0.55f,0.0f, -0.44f,-0.55f,0.0f, -0.32f,-0.55f,0.0f,
        -0.61f,0.55f,0.0f, -0.32f,-0.55f,0.0f, -0.55f,0.55f,0.0f,
        -0.77f,-0.08f,0.0f, -0.46f,-0.08f,0.0f, -0.44f,0.04f,0.0f,
        -0.77f,-0.08f,0.0f, -0.44f,0.04f,0.0f, -0.73f,0.04f,0.0f,

        -0.22f,-0.55f,0.0f, -0.08f,-0.55f,0.0f, -0.08f,0.55f,0.0f,
        -0.22f,-0.55f,0.0f, -0.08f,0.55f,0.0f, -0.22f,0.55f,0.0f,
        -0.08f,0.43f,0.0f,  0.18f,0.43f,0.0f,  0.18f,0.55f,0.0f,
        -0.08f,0.43f,0.0f,  0.18f,0.55f,0.0f, -0.08f,0.55f,0.0f,
        -0.08f,0.06f,0.0f,  0.18f,0.06f,0.0f,  0.18f,0.18f,0.0f,
        -0.08f,0.06f,0.0f,  0.18f,0.18f,0.0f, -0.08f,0.18f,0.0f,
         0.18f,0.06f,0.0f,  0.31f,-0.55f,0.0f,  0.16f,-0.55f,0.0f,
         0.18f,0.06f,0.0f,  0.16f,-0.55f,0.0f,  0.05f,0.06f,0.0f,
         0.18f,0.55f,0.0f,  0.30f,0.43f,0.0f,  0.30f,0.18f,0.0f,
         0.18f,0.55f,0.0f,  0.30f,0.18f,0.0f,  0.18f,0.06f,0.0f,

         0.40f,-0.55f,0.0f,  0.54f,-0.55f,0.0f,  0.54f,0.55f,0.0f,
         0.40f,-0.55f,0.0f,  0.54f,0.55f,0.0f,  0.40f,0.55f,0.0f,
         0.54f,0.43f,0.0f,  0.78f,0.43f,0.0f,  0.78f,0.55f,0.0f,
         0.54f,0.43f,0.0f,  0.78f,0.55f,0.0f,  0.54f,0.55f,0.0f,
         0.54f,0.06f,0.0f,  0.78f,0.06f,0.0f,  0.78f,0.18f,0.0f,
         0.54f,0.06f,0.0f,  0.78f,0.18f,0.0f,  0.54f,0.18f,0.0f,
         0.54f,-0.55f,0.0f, 0.78f,-0.55f,0.0f, 0.78f,-0.43f,0.0f,
         0.54f,-0.55f,0.0f, 0.78f,-0.43f,0.0f, 0.54f,-0.43f,0.0f,
         0.78f,0.55f,0.0f,  0.90f,0.43f,0.0f,  0.90f,0.18f,0.0f,
         0.78f,0.55f,0.0f,  0.90f,0.18f,0.0f,  0.78f,0.06f,0.0f,
         0.78f,0.06f,0.0f,  0.90f,-0.06f,0.0f, 0.90f,-0.43f,0.0f,
         0.78f,0.06f,0.0f,  0.90f,-0.43f,0.0f, 0.78f,-0.55f,0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = (GLint)strlen(shaderCode);
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("El error al compilar el shader %d es: %s\n", shaderType, eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();
    if (!shader)
    {
        printf("Error creando el shader");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("El error al linkear es: %s\n", eLog);
        return;
    }
}

int main()
{
    if (!glfwInit())
    {
        printf("Fallo inicializar GLFW");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1", NULL, NULL);
    if (!mainWindow)
    {
        printf("Fallo en crearse la ventana con GLFW");
        glfwTerminate();
        return 1;
    }

    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("Fallo inicializacion de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glGetError();
    glViewport(0, 0, BufferWidth, BufferHeight);
    CrearLetras();
    CompileShaders();

    srand((unsigned int)time(NULL) ^ (unsigned int)glfwGetTimerValue());
    GLfloat rojo = (GLfloat)rand() / (GLfloat)RAND_MAX;
    GLfloat verde = (GLfloat)rand() / (GLfloat)RAND_MAX;
    GLfloat azul = (GLfloat)rand() / (GLfloat)RAND_MAX;
    double ultimoCambio = glfwGetTime();

    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();
        if (glfwGetTime() - ultimoCambio >= 2.0)
        {
            rojo = (GLfloat)rand() / (GLfloat)RAND_MAX;
            verde = (GLfloat)rand() / (GLfloat)RAND_MAX;
            azul = (GLfloat)rand() / (GLfloat)RAND_MAX;
            ultimoCambio = glfwGetTime();
        }

        glClearColor(rojo, verde, azul, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 84);
        glBindVertexArray(0);
        glUseProgram(0);
        glfwSwapBuffers(mainWindow);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shader);
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
