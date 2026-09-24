#version 330
layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;
out vec4 vColor;
out vec3 normalMundo;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 color;
uniform mat4 view;
void main()
{
    // Incluyo la vista para poder recorrer el rover con la cámara.
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    vColor = vec4(color, 1.0f);
    // Model entrega normales invertidas; recupero su sentido y la orientación de la pieza.
    normalMundo = mat3(transpose(inverse(model))) * (-normal);
}
