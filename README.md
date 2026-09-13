# Computación Gráfica e Interacción Humano Computadora

**Alumno:** Alan Rogelio Barrientos Ramírez  
**Número de cuenta:** 422022019  
**Grupo de teoría:** 6  
**Grupo de laboratorio:** 3  
**Semestre:** 2027-1

## Práctica 03 — Cohete y rombo de ocho pirámides

**Fecha de entrega:** 12 de septiembre de 2026.

El cohete está construido con instancias de cilindros, conos, esferas, cubos y pirámides. El segundo ejercicio une ocho pirámides de base cuadrada: cuatro arriba y cuatro abajo. Cada pirámide tiene las caras triangulares roja, verde, amarilla y magenta, y la base azul.

### Entrega

- [Reporte en PDF](Reporte/R03-422022019.pdf).
- [Paquete completo: reporte, proyecto y shaders](Entrega/P03-422022019.zip).
- [Main de los dos ejercicios](Proyecto/practica3.cpp).
- [ZIP independiente de shaders](Entrega/Shaders-P03-422022019.zip).
- [Cambios y justificación](CAMBIOS.md), incluida la inclinación P12 que cierra el rombo.
- [Comparación con el código del profesor](CAMBIOS-respecto-al-profesor.diff).

### Ejecución

Abrir `Proyecto/Practica03.sln` en Visual Studio con desarrollo de escritorio en C++, seleccionar `Release | x64` y ejecutar con Ctrl + F5. El proyecto contiene un solo main. Si se utiliza otro proyecto, copiar también los archivos auxiliares modificados: `Mesh.cpp`, `Window.cpp` y `Shader.cpp`.

| Tecla | Acción |
|---|---|
| 1 | Mostrar el cohete |
| 2 | Mostrar el rombo de ocho pirámides |
| 3 | Revisar una pirámide y sus cinco caras |
| E / R / T | Girar en X / Y / Z |
| Esc | Cerrar |

Se conservan las clases y funciones de dibujo del ejemplo. Las modificaciones están marcadas en el código y explicadas en el reporte; los cuatro shaders mantienen su contenido original.

### Capturas

![Cohete](Capturas/cohete-3d.png)

![Giro del rombo cerrado](Capturas/rombo-giro.gif)
