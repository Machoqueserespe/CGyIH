#version 330
in vec4 vColor;
in vec3 normalMundo;
out vec4 color;
void main()
{
    // La normal permite distinguir el volumen sin cambiar la geometría ni cargar texturas.
    float luz = 0.40 + 0.60 * max(dot(normalize(normalMundo), normalize(vec3(0.4, 0.8, 0.6))), 0.0);
    color = vec4(vColor.rgb * luz, vColor.a);
}
