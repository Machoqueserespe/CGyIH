# Práctica 03 - Cohete y pirámides

## Contenido de la práctica

La práctica incluye el cohete, el rombo tridimensional cerrado y la pirámide individual de cinco colores. El rombo contiene ocho pirámides de base cuadrada: cuatro se unen en la punta superior y cuatro en la inferior. Las bases cubren el plano central y las caras interiores coinciden, por lo que no quedan huecos entre las piezas. El reporte R03-422022019.pdf documenta ambos ejercicios. Fecha de entrega: 12 de septiembre de 2026.

## Base utilizada

Se utilizó `practica3.cpp`, sus clases y los shaders proporcionados por el profesor. Los archivos recibidos se conservan por separado; no se incluye otro main en la entrega. `CAMBIOS-respecto-al-profesor.diff` permite comparar los cambios. El proyecto de Windows contiene un solo main, `Proyecto/practica3.cpp`.

No se añadieron clases ni funciones de modelado o dibujo. Se reutilizan `Mesh`, `MeshColor`, `Sphere`, las funciones `CrearCubo`, `CrearPiramideTriangular`, `CrearPiramideCuadrangular`, `CrearCilindro`, `CrearCono` y los dos programas de shader del ejemplo. Los bloques de transformación se repiten para cada instancia.

## Cambios y justificación

| Marca | Archivo | Cambio y motivo |
|---|---|---|
| P01 | practica3.cpp | Se utiliza la clase existente `MeshColor`. La pirámide cuadrangular conserva sus cinco posiciones originales, pero se repiten los vértices por cara para asignar un RGB constante a cada triángulo. Las cuatro caras laterales son roja, verde, amarilla y magenta; los dos triángulos de la base son azules. No se usa una malla distinta para cada instancia. |
| P02 | practica3.cpp | Se corrigen los índices del cilindro: el ejemplo generaba un índice por componente del arreglo, aunque cada vértice ocupa tres componentes. Se forman dos triángulos por sector y las tapas, reutilizando las circunferencias existentes. Se dibuja con `RenderMesh`, que ya utiliza `GL_TRIANGLES`. |
| P03 | practica3.cpp | Se elimina del cono un punto de cierre adicional calculado incorrectamente; el ciclo ya cierra el contorno. Se agregan los índices de la base para obtener un sólido cerrado. Se reutiliza `RenderMesh`. |
| P04 | practica3.cpp | Se comprueba el resultado de `Initialise` antes de utilizar OpenGL. |
| P05 | practica3.cpp | Cilindro y cono usan 48 sectores y radio 1, de modo que las escalas de sus instancias sean fáciles de identificar. |
| P06 | practica3.cpp | Se consultan las teclas del arreglo existente: 1 muestra el cohete, 2 la unión de ocho pirámides y 3 la pirámide individual para comprobar sus caras. No se modifica la clase de entrada. |
| P07 | practica3.cpp | Se utiliza un fondo azul oscuro para distinguir las piezas claras. |
| P08 | practica3.cpp | Se repite el bloque original de modelo, traslación, rotación, escala, color y dibujo. El cohete se desplaza a Z = -4.8 para que quepa completo; la pirámide de comprobación permanece en Z = -4. La proyección perspectiva original de 60 grados y su relación de aspecto se conservan. |
| P09 | Mesh.cpp | `CreateMeshColor` recibe el número de componentes para cargar el VBO, pero `glDrawArrays` necesita el número de vértices. Se divide entre 6, correspondientes a XYZ y RGB, para evitar leer fuera del arreglo. |
| P10 | Window.cpp | Se inicializan los miembros que estaban sin valor y se devuelve 0 cuando la ventana se crea correctamente. También se cambia su título para indicar los controles. |
| P11 | Shader.cpp | Se comenta la validación prematura, que ocurre antes de enlazar un VAO y puede devolver un error en un contexto core. Se conservan las verificaciones de compilación y enlace y la consulta de uniformes. |
| P12 | practica3.cpp | Se dibujan ocho instancias de la misma pirámide cuadrangular. Mediante giros y escala se inclina cada instancia, conservando su base cuadrada, para que las cuatro superiores compartan la punta (0, 1, 0) y las inferiores (0, -1, 0). Las bases ocupan los cuatro cuadrantes del plano Y = 0. Esta inclinación permite cerrar el rombo sin separar sus puntas ni superponer todo el volumen de las instancias. No se modifican los vértices de la malla ni se añaden funciones o clases. |

Los fragmentos sustituidos se conservan comentados cuando el cambio es local. El diff completo permite comparar las adiciones con el original. Los archivos de trabajo se guardaron en UTF-8 y el proyecto usa `/utf-8`; este cambio no altera las instrucciones de C++.

## Instancias del cohete

- 5 cilindros: fuselaje, dos bandas y dos propulsores laterales.
- 7 conos: punta central, tobera central, dos puntas de propulsores y tres llamas.
- 2 cubos: soportes de los propulsores laterales.
- 2 esferas: marco y cristal de la ventanilla, escalados para ajustar su profundidad.
- 4 pirámides cuadrangulares: aletas alrededor del fuselaje.

Total: 20 instancias. Todas las mallas se crean en sus coordenadas locales alrededor del origen y se acomodan con las transformaciones del ejemplo. No se deformó la proyección para ajustar el dibujo a la ventana.

## Unión de las ocho pirámides

Cada pirámide mantiene su base cuadrada de lado 1, pero se transforma en una pirámide oblicua: su punta queda sobre una esquina de la base en lugar de quedar sobre su centro. Las cuatro bases superiores ocupan los cuadrantes del cuadrado central de lado 2. Al compartir la punta (0, 1, 0), sus caras interiores encajan y forman una sola mitad superior. La mitad inferior se obtiene girando las otras cuatro instancias 180° alrededor de Z.

Se reutilizan únicamente las operaciones de modelo que ya usa el ejemplo: traslación, rotación y escala. La combinación de giro, escala y giro que aparece en cada bloque equivale a la inclinación x' = x - y/√2, y' = y, z' = z. En el orden de multiplicación de GLM:

Rz(-35.26438968°) × S(√2, 1/√2, 1) × Rz(54.73561032°).

Los giros previos y posteriores alrededor de Y orientan esa inclinación hacia el centro de cada cuadrante. La traslación (±0.25, 0.5, ±0.25) coloca cada pieza superior; la inferior utiliza además el giro de 180° ya descrito. Los ángulos proceden de atan(1/√2) = 35.26438968° y su complementario, no de un cambio en la proyección.

Cada base sigue siendo un cuadrado de lado 1 en Y = 0. Las cuatro bases de cada mitad cubren el cuadrado central completo y no se solapan en su interior. El volumen de cada pieza es 1/3; las ocho suman 8/3, igual que las dos mitades de base 4 y altura 1. Las bases azules y algunas caras laterales quedan dentro del sólido; todos los colores se comprueban con la vista individual.

## Comprobación geométrica

Se comprobaron las matrices del main: las ocho instancias conservan bases cuadradas, se unen en las dos puntas comunes y suman un volumen de 8/3. Las capturas del reporte muestran el cohete, el rombo cerrado y la pirámide individual. El cohete, la proyección, las mallas y la escena individual no cambiaron en la corrección final de la unión.
