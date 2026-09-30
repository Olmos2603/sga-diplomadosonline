#include "Bootcamp.h"

Bootcamp::Bootcamp() : ProgramaAcademico("Bootcamp") {
}

bool Bootcamp::evaluarAprobacion(const double notas[3]) const {
    for (int i = 0; i < 3; i++) {
        if (notas[i] < 14.0) {
            return false;
        }
    }
    return true;
}
