#include "Profesor.h"

Profesor::Profesor(const std::string& cedula, const std::string& nombreCompleto,
                    const std::string& correoElectronico, const std::string& especialidad,
                    const std::string& materia)
    : Persona(cedula, nombreCompleto, correoElectronico),
      especialidad(especialidad), materia(materia) {
}

Profesor::~Profesor() {
}

std::string Profesor::getEspecialidad() const {
    return especialidad;
}

std::string Profesor::getMateria() const {
    return materia;
}

std::string Profesor::aLineaTxt() const {
    return getCedula() + "," + getNombreCompleto() + "," + getCorreoElectronico() + "," +
           especialidad + "," + materia;
}
