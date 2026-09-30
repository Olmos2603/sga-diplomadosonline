#ifndef BOOTCAMP_H
#define BOOTCAMP_H

#include "ProgramaAcademico.h"

/** Bootcamp: NO se aprueba por promedio. Ninguna nota puede ser < 14/20. */
class Bootcamp : public ProgramaAcademico {
public:
    Bootcamp();
    bool evaluarAprobacion(const double notas[3]) const override;
};

#endif
