# Práctica 03: cohete y unión de ocho pirámides

Alan Rogelio Barrientos Ramírez — 422022019  
Laboratorio: grupo 3. Teoría: grupo 6. Semestre: 2027-1.  
Fecha de entrega: 12 de septiembre de 2026.

Están implementados el cohete, el rombo tridimensional cerrado y una vista de la pirámide individual para comprobar sus cinco colores. El rombo se forma con ocho pirámides cuadrangulares oblicuas: las cuatro superiores comparten una sola punta y las cuatro inferiores otra. Sus bases cuadradas cubren el plano central sin dejar huecos. Las bases azules quedan dentro de la unión y se pueden revisar en la pirámide individual con la tecla 3. Los giros y escalas usados para inclinar las instancias están explicados en CAMBIOS.md.

## Windows

Abrir `Proyecto/Practica03.sln` en Visual Studio con desarrollo de escritorio en C++. Seleccionar `Release | x64`, compilar y ejecutar con Ctrl + F5. El proyecto utiliza las bibliotecas, clases y shaders de la base del profesor.

Hay un solo main: `Proyecto/practica3.cpp`. Si se lleva a un proyecto existente, se deben copiar también `Mesh.cpp`, `Window.cpp` y `Shader.cpp`, porque contienen los ajustes documentados en `CAMBIOS.md`.

## Archivos de entrega

- `R03-422022019.pdf`: reporte con carátula, explicación de los ejercicios, capturas, modificaciones, conclusión y bibliografía.
- `Proyecto/`: solución de Visual Studio, un solo main, clases auxiliares, bibliotecas y shaders.
- `Shaders-P03-422022019.zip`: los cuatro archivos de shader, en un ZIP independiente.
- `CAMBIOS.md`: explicación de las modificaciones P01–P12.
- `CAMBIOS-respecto-al-profesor.diff`: comparación con el código recibido.

El paquete `P03-422022019.zip` reúne estos archivos. Para compilar se debe usar el proyecto completo, pues los cambios de `Mesh.cpp`, `Window.cpp` y `Shader.cpp` acompañan al main. Los shaders mantienen el contenido original del profesor.

## Controles

- 1: cohete.
- 2: rombo cerrado de ocho pirámides, cuatro arriba y cuatro abajo.
- 3: pirámide individual para comprobar caras.
- E: rotación en X.
- R: rotación en Y.
- T: rotación en Z.
- Esc: cerrar.

Los shaders originales no aplican la matriz de vista; por eso se conserva la rotación con E, R y T, sin presentar WASD o el ratón como controles de cámara activos.
