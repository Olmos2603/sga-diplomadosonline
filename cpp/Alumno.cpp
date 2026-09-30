#include "Alumno.h"
#include <cmath>
#include <sstream>

Alumno::Alumno(const std::string& cedula, const std::string& nombreCompleto,
               const std::string& correoElectronico, ProgramaAcademico* programa)
    : Persona(cedula, nombreCompleto, correoElectronico), programa(programa) {
    notas[0] = 0.0;
    notas[1] = 0.0;
    notas[2] = 0.0;
}

Alumno::Alumno(const std::string& cedula, const std::string& nombreCompleto,
               const std::string& correoElectronico, ProgramaAcademico* programa,
               const double notasIniciales[3])
    : Persona(cedula, nombreCompleto, correoElectronico), programa(programa) {
    notas[0] = notasIniciales[0];
    notas[1] = notasIniciales[1];
    notas[2] = notasIniciales[2];
}

Alumno::~Alumno() {
    // Liberacion manual de memoria: "programa" fue creado con "new"
    // (ver SistemaSGA::crearProgramaPorNombre) y es propiedad exclusiva
    // de este Alumno, por lo que se libera aqui con "delete".
    delete programa;
    programa = nullptr;
}

ProgramaAcademico* Alumno::getPrograma() const {
    return programa;
}

const double* Alumno::getNotas() const {
    return notas;
}

double Alumno::promedio() const {
    return (notas[0] + notas[1] + notas[2]) / 3.0;
}

bool Alumno::estaAprobado() const {
    return programa->evaluarAprobacion(notas);
}

int Alumno::slotLibreParaNota() const {
    for (int i = 0; i < 3; i++) {
        if (notas[i] == 0.0) {
            return i;
        }
    }
    return -1;
}

void Alumno::setNota(int indice, double valor) {
    notas[indice] = valor;
}

std::string Alumno::formatNota(double n) {
    if (n == std::floor(n)) {
        std::ostringstream oss;
        oss << static_cast<long>(n);
        return oss.str();
    }
    std::ostringstream oss;
    oss << n;
    return oss.str();
}

std::string Alumno::aLineaTxt() const {
    return getCedula() + "," + getNombreCompleto() + "," + getCorreoElectronico() + "," +
           programa->getNombre() + "," +
           formatNota(notas[0]) + "," + formatNota(notas[1]) + "," + formatNota(notas[2]);
}
