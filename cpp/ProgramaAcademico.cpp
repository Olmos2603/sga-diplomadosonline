#include "ProgramaAcademico.h"

ProgramaAcademico::ProgramaAcademico(const std::string& nombre) : nombre(nombre) {
}

ProgramaAcademico::~ProgramaAcademico() {
}

std::string ProgramaAcademico::getNombre() const {
    return nombre;
}
