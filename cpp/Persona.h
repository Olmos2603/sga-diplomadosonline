#ifndef PERSONA_H
#define PERSONA_H

#include <string>

/**
 * Clase base de la jerarquia de personas. Contiene los atributos comunes
 * a Alumno y Profesor: Cedula/ID, Nombre Completo y Correo Electronico.
 *
 * Se declara con destructor virtual porque se manejaran punteros
 * polimorficos (Alumno*, Profesor*) que deben liberarse correctamente
 * con "delete" a traves de un puntero base si llegara a usarse asi.
 */
class Persona {
private:
    std::string cedula;
    std::string nombreCompleto;
    std::string correoElectronico;

public:
    Persona(const std::string& cedula, const std::string& nombreCompleto,
            const std::string& correoElectronico);
    virtual ~Persona();

    std::string getCedula() const;
    std::string getNombreCompleto() const;
    std::string getCorreoElectronico() const;
};

#endif
