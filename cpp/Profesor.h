#ifndef PROFESOR_H
#define PROFESOR_H

#include "Persona.h"
#include <string>

/** Profesor: hereda de Persona y agrega Especialidad Academica y Materia Asignada. */
class Profesor : public Persona {
private:
    std::string especialidad;
    std::string materia;

public:
    Profesor(const std::string& cedula, const std::string& nombreCompleto,
             const std::string& correoElectronico, const std::string& especialidad,
             const std::string& materia);
    ~Profesor() override;

    std::string getEspecialidad() const;
    std::string getMateria() const;

    // Serializa al formato: Cedula,Nombre,Correo,Especialidad,Materia
    std::string aLineaTxt() const;
};

#endif
