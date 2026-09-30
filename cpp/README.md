<<<<<<< HEAD
# SGA-DO — Implementación en C++ (Entregable 6)

Sistema de Gestión Académica de DiplomadosOnline.com, motor core por consola.
Misma lógica de negocio que las versiones en Python y Java, adaptada al
entorno compilado: persistencia con `fstream` y **gestión manual de
memoria con `new`/`delete`**.

## Compilar y ejecutar

```bash
g++ -std=c++17 -Wall -Wextra -o sga_do *.cpp
./sga_do
```

Compilado y probado en este entorno con `g++ 13.3.0` sin errores ni
advertencias (`-Wall -Wextra`).

Al iniciar, si existen `alumnos.txt` y `profesores.txt` en la misma
carpeta, el sistema los carga automáticamente (persistencia real en
disco, no solo en memoria).

## Estructura de clases

- `Persona` (con destructor virtual) → `Alumno`, `Profesor` — herencia.
- `ProgramaAcademico` (con método virtual puro) → `Curso`, `Diplomado`,
  `Bootcamp`, cada una sobreescribe `evaluarAprobacion()` — polimorfismo
  real, sin `if (tipo == "Bootcamp")` en la lógica principal.
- `AccionNota`: struct auxiliar para la pila de deshacer.
- `SistemaSGA`: lógica de negocio, persistencia y estructuras de datos.
- `main.cpp`: menú de consola en bucle.

## Gestión manual de memoria (ver Anexo QA EVAL-04)

- Cada `Alumno` y `Profesor` se crea con `new` en `registrarAlumno()`,
  `registrarProfesor()` y `cargarDesdeDisco()`, y se guarda como puntero
  (`Alumno*`, `Profesor*`) en `std::vector`.
- Cada `ProgramaAcademico` (`Curso`/`Diplomado`/`Bootcamp`) también se
  crea con `new` y es **propiedad** del `Alumno` que lo recibe.
- El destructor de `Alumno` libera su `ProgramaAcademico*` con `delete`.
- El destructor de `SistemaSGA` recorre los vectores y libera con
  `delete` cada `Alumno*`/`Profesor*`, evitando fugas de memoria.
- Se deshabilitó el constructor de copia (`= delete`) en `Alumno` y
  `SistemaSGA` para no duplicar la propiedad de un puntero y provocar
  un doble `delete`.

## Estructuras de datos exigidas

- **Pila (LIFO)**: `std::stack<AccionNota>` — cada nota agregada
  (Opción 3) se apila con `push()`; la Opción 4 la revierte con
  `top()`/`pop()`.
- **Cola (FIFO)**: `std::queue<Alumno*>` — en la Opción 5 se encolan los
  alumnos aprobados con `push()` y se procesan en orden con
  `front()`/`pop()` hacia `certificados_pendientes.txt`.

## Archivos generados

- `alumnos.txt` → `Cedula,Nombre,Correo,TipoPrograma,Nota1,Nota2,Nota3`
- `profesores.txt` → `Cedula,Nombre,Correo,Especialidad,Materia`
- `certificados_pendientes.txt` → reporte de salida (se sobrescribe en cada Opción 5)

## Casos de prueba (verificados en este entorno)

### CP-01 — Cálculo polimórfico correcto
Con la data semilla (V-101 Ana, V-202 Carlos, V-303 María, V-404 Luis),
la Opción 6 mostró: Ana y Luis **APROBADO**; Carlos y María
**REPROBADO** (María pese a promedio 17.7, por la regla estricta del
Bootcamp: tiene una nota de 13).

### CP-02 — Funcionalidad LIFO (Deshacer)
Se registró una nota errónea (45) a un alumno de prueba, se deshizo con
la Opción 4, y el sistema confirmó "Se deshizo la nota 45..."; la nota
volvió a 0 y el slot quedó libre para una nueva nota correcta.

### CP-03 — Funcionalidad FIFO (Cola de certificados)
La Opción 5 exportó a `certificados_pendientes.txt` **solo** a Ana Silva
(1º) y Luis Rojas (2º), en ese orden. Carlos y María no aparecen.

## Notas de robustez

- El menú y el ingreso de notas usan `try/catch` sobre
  `std::invalid_argument`/`std::out_of_range` (lanzadas por `std::stoi`/
  `std::stod`) para no romperse ante letras o texto vacío (EVAL-03).
- Se valida que no existan cédulas duplicadas entre alumnos y profesores.
- Después de cada opción que no requiere teclear nada (4, 5, 6) se pide
  presionar ENTER antes de volver a mostrar el menú.
=======
# Sistema de Gestión Académica (SGA-DO)
>>>>>>> 47e758852b52bc795b8d8d78c86cf1f23c1d80d7
