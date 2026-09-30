#ifndef ALUMNO_H
#define ALUMNO_H

#include "Persona.h"
#include "ProgramaAcademico.h"
#include <string>

/**
 * Alumno: hereda de Persona y agrega el Programa Academico asignado
 * y un arreglo de hasta 3 notas.
 *
 * IMPORTANTE (gestion manual de memoria, ver Anexo QA EVAL-04):
 * el puntero "programa" se crea con "new" (Curso/Diplomado/Bootcamp) y
 * es PROPIEDAD de este Alumno. El destructor de Alumno libera esa
 * memoria con "delete" para evitar fugas de memoria (memory leaks).
 */
class Alumno : public Persona {
private:
    ProgramaAcademico* programa; // puntero propio, creado con "new"
    double notas[3];

    // Se prohibe copiar el objeto para no duplicar la propiedad del
    // puntero "programa" (evita un doble "delete" sobre la misma direccion).
    Alumno(const Alumno&) = delete;
    Alumno& operator=(const Alumno&) = delete;

    // Imprime notas enteras sin decimales (10 en vez de 10.0).
    static std::string formatNota(double n);

public:
    // Constructor para un alumno recien registrado (notas en 0).
    Alumno(const std::string& cedula, const std::string& nombreCompleto,
           const std::string& correoElectronico, ProgramaAcademico* programa);

    // Constructor usado al cargar un alumno ya existente desde alumnos.txt.
    Alumno(const std::string& cedula, const std::string& nombreCompleto,
           const std::string& correoElectronico, ProgramaAcademico* programa,
           const double notasIniciales[3]);

    ~Alumno() override; // libera "programa" con delete

    ProgramaAcademico* getPrograma() const;
    const double* getNotas() const;

    double promedio() const;
    bool estaAprobado() const; // delega en programa->evaluarAprobacion() (polimorfismo)

    // Devuelve el indice de la primera nota aun no registrada (0.0), o -1 si ya tiene las 3.
    int slotLibreParaNota() const;
    void setNota(int indice, double valor);

    // Serializa al formato: Cedula,Nombre,Correo,TipoPrograma,Nota1,Nota2,Nota3
    std::string aLineaTxt() const;
};

#endif
