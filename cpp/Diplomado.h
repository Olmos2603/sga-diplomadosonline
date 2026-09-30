#ifndef DIPLOMADO_H
#define DIPLOMADO_H

#include "ProgramaAcademico.h"

/** Diplomado: se aprueba si el promedio de las 3 notas es >= 14/20. */
class Diplomado : public ProgramaAcademico {
public:
    Diplomado();
    bool evaluarAprobacion(const double notas[3]) const override;
};

#endif
