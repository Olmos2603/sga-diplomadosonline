#ifndef CURSO_H
#define CURSO_H

#include "ProgramaAcademico.h"

/** Curso: se aprueba si el promedio de las 3 notas es >= 10/20. */
class Curso : public ProgramaAcademico {
public:
    Curso();
    bool evaluarAprobacion(const double notas[3]) const override;
};

#endif
