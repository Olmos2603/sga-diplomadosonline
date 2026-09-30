#ifndef ACCION_NOTA_H
#define ACCION_NOTA_H

#include <string>

/**
 * Representa una accion "se agrego una nota" para poder apilarla y
 * deshacerla despues (Opcion 4 del menu, logica LIFO).
 */
struct AccionNota {
    std::string cedulaAlumno;
    int indiceNota;

    AccionNota(const std::string& cedulaAlumno, int indiceNota)
        : cedulaAlumno(cedulaAlumno), indiceNota(indiceNota) {
    }
};

#endif
