#version 330
layout (location =0) in vec3 pos;
out vec4 vColor;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 color;
uniform mat4 view;
void main()
{
	// Paso de las coordenadas de la pieza a la escena, después a la cámara y a la proyección.
	gl_Position=projection*view*model*vec4(pos,1.0f);
	// Envío al fragment shader el color RGB de la pieza con opacidad completa.
	vColor=vec4(color,1.0f);

}
