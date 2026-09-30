#include "Curso.h"

Curso::Curso() : ProgramaAcademico("Curso") {
}

bool Curso::evaluarAprobacion(const double notas[3]) const {
    double suma = notas[0] + notas[1] + notas[2];
    double promedio = suma / 3.0;
    return promedio >= 10.0;
}
