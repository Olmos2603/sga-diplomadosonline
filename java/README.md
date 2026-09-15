# SGA-DO — Implementación en Java (Entregable 5)

Sistema de Gestión Académica de DiplomadosOnline.com, motor core por consola.
Misma lógica de negocio que la versión en Python, adaptada a las reglas
estrictas de Java: encapsulamiento `private` + getters, colecciones
oficiales (`Stack`, `Queue`) y manejo de excepciones (`try/catch`).

## Compilar y ejecutar

```bash
javac *.java
java Main
```

Al iniciar, si existen `alumnos.txt` y `profesores.txt` en la misma carpeta,
el sistema los carga automáticamente (persistencia real en disco, no solo
en memoria).

## Estructura de clases

- `Persona` (abstracta) → `Alumno`, `Profesor` — herencia.
- `ProgramaAcademico` (abstracta) → `Curso`, `Diplomado`, `Bootcamp`, cada
  una sobreescribe `evaluarAprobacion(double[] notas)` con `@Override`
  (polimorfismo real, sin `if (tipo.equals("Bootcamp"))` en la lógica principal).
- `AccionNota`: clase auxiliar para poder apilar/deshacer una nota.
- `SistemaSGA`: contiene la lógica de negocio, la persistencia y las
  estructuras de datos exigidas.
- `Main`: contiene el menú de consola en bucle.

## Estructuras de datos exigidas

- **Pila (LIFO)**: `java.util.Stack<AccionNota>` — cada nota agregada
  (Opción 3) se apila con `push()`; la Opción 4 la revierte con `pop()`.
- **Cola (FIFO)**: `java.util.Queue<Alumno>` implementada con `LinkedList`
  — en la Opción 5 se encolan los alumnos aprobados con `offer()` y se
  procesan en orden con `poll()` hacia `certificados_pendientes.txt`.

## Archivos generados

- `alumnos.txt` → `Cedula,Nombre,Correo,TipoPrograma,Nota1,Nota2,Nota3`
- `profesores.txt` → `Cedula,Nombre,Correo,Especialidad,Materia`
- `certificados_pendientes.txt` → reporte de salida (se sobrescribe en cada Opción 5)

## Casos de prueba (Anexo QA — PARTE 1)

### CP-01 — Cálculo polimórfico correcto
Registra a los 4 alumnos de la data semilla (V-101 Ana, V-202 Carlos,
V-303 María, V-404 Luis) con sus notas, y ve a la Opción 6. Ana y Luis
deben salir **APROBADO**; Carlos y María **REPROBADO** (María pese a tener
promedio 17.6, por la regla estricta del Bootcamp: tiene una nota de 13).

### CP-02 — Funcionalidad LIFO (Deshacer)
Registra una nota errónea (ej. 45) a un alumno con espacio libre, ve a la
Opción 4 inmediatamente, y vuelve a consultar al alumno: la nota debe
haber desaparecido de su historial y del promedio.

### CP-03 — Funcionalidad FIFO (Cola de certificados)
Ejecuta la Opción 5. El archivo `certificados_pendientes.txt` debe listar
**solo** a los aprobados, en el mismo orden en que fueron registrados
(Ana primero, Luis segundo, con la data semilla). Carlos y María no deben
aparecer.

## Notas de robustez

- El menú y el ingreso de notas están protegidos con `try/catch`
  (`NumberFormatException`) para no romperse ante letras o texto vacío
  (EVAL-03).
- Se valida que no existan cédulas duplicadas entre alumnos y profesores
  (integridad de datos).
- Después de cada opción que no requiere teclear nada (4, 5, 6) se pide
  presionar ENTER antes de volver a mostrar el menú, para que el
  resultado no quede tapado por el menú siguiente.

## Nota de verificación

Este código fue escrito y revisado cuidadosamente (incluyendo balance de
llaves/paréntesis línea por línea), pero no pude compilarlo dentro de este
entorno porque solo cuenta con el JRE (no el JDK) y no tiene acceso a
internet para instalarlo. Por favor compílalo y corre los 3 casos de
prueba de arriba en tu máquina; si algo no compila o falla, pégame el
error exacto y lo corrijo de inmediato.
