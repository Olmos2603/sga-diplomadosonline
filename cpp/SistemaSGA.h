#ifndef SISTEMA_SGA_H
#define SISTEMA_SGA_H

#include "Alumno.h"
#include "Profesor.h"
#include "AccionNota.h"
#include "ProgramaAcademico.h"
#include <vector>
#include <stack>
#include <string>

/**
 * Orquesta la memoria dinamica (vector de punteros), la persistencia en
 * archivos .txt via fstream, la Pila (std::stack) para deshacer notas y
 * la Cola (std::queue) para generar el reporte de certificados pendientes.
 *
 * Gestion manual de memoria (EVAL-04): todo Alumno* y Profesor* se crea
 * con "new" en registrarAlumno()/registrarProfesor()/cargarDesdeDisco(),
 * y se libera con "delete" en el destructor de SistemaSGA.
 */
class SistemaSGA {
private:
    std::vector<Alumno*> alumnos;
    std::vector<Profesor*> profesores;
    std::stack<AccionNota> pilaDeshacer; // Pila (LIFO) - contenedor oficial de la STL

    static const std::string ARCHIVO_ALUMNOS;
    static const std::string ARCHIVO_PROFESORES;
    static const std::string ARCHIVO_CERTIFICADOS;

    ProgramaAcademico* crearProgramaPorNombre(const std::string& nombre) const;
    void cargarDesdeDisco();

    Alumno* buscarAlumno(const std::string& cedula) const;
    Profesor* buscarProfesor(const std::string& cedula) const;
    bool cedulaYaExiste(const std::string& cedula) const;

    static std::vector<std::string> splitCSV(const std::string& linea);

public:
    SistemaSGA();
    ~SistemaSGA(); // libera con "delete" todos los Alumno*/Profesor* creados con "new"

    // Se prohibe copiar el sistema completo (evita doble liberacion de los punteros).
    SistemaSGA(const SistemaSGA&) = delete;
    SistemaSGA& operator=(const SistemaSGA&) = delete;

    void guardarAlumnos() const;
    void guardarProfesores() const;

    void registrarAlumno();          // Opcion 1
    void registrarProfesor();        // Opcion 2
    void registrarNotas();           // Opcion 3
    void deshacerUltimaNota();       // Opcion 4 (Pila / LIFO)
    void generarColaCertificados();  // Opcion 5 (Cola / FIFO)
    void mostrarReporteGeneral() const; // Opcion 6
};

#endif
