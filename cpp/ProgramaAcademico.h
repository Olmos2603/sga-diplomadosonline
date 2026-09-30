#ifndef PROGRAMA_ACADEMICO_H
#define PROGRAMA_ACADEMICO_H

#include <string>

/**
 * Clase base de la jerarquia de programas academicos.
 * Cada subclase (Curso, Diplomado, Bootcamp) SOBREESCRIBE
 * evaluarAprobacion() con su propia regla de negocio (polimorfismo real).
 *
 * El resto del sistema nunca pregunta "if (tipo == Bootcamp)"; delega
 * la decision al objeto ProgramaAcademico correspondiente.
 */
class ProgramaAcademico {
private:
    std::string nombre;

public:
    explicit ProgramaAcademico(const std::string& nombre);
    virtual ~ProgramaAcademico();

    std::string getNombre() const;

    // Metodo virtual puro: obliga a cada subclase a implementarlo.
    virtual bool evaluarAprobacion(const double notas[3]) const = 0;
};

#endif
