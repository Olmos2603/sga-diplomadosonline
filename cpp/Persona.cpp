#include "Persona.h"

Persona::Persona(const std::string& cedula, const std::string& nombreCompleto,
                  const std::string& correoElectronico)
    : cedula(cedula), nombreCompleto(nombreCompleto), correoElectronico(correoElectronico) {
}

Persona::~Persona() {
    // No hay memoria dinamica propia que liberar aqui.
}

std::string Persona::getCedula() const {
    return cedula;
}

std::string Persona::getNombreCompleto() const {
    return nombreCompleto;
}

std::string Persona::getCorreoElectronico() const {
    return correoElectronico;
}
