#include "Diplomado.h"

Diplomado::Diplomado() : ProgramaAcademico("Diplomado") {
}

bool Diplomado::evaluarAprobacion(const double notas[3]) const {
    double suma = notas[0] + notas[1] + notas[2];
    double promedio = suma / 3.0;
    return promedio >= 14.0;
}
