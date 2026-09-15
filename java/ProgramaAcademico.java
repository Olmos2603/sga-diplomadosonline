/**
 * Clase base de la jerarquia de programas academicos.
 * Cada subclase (Curso, Diplomado, Bootcamp) SOBREESCRIBE
 * evaluarAprobacion() con su propia regla de negocio.
 *
 * Este es el punto clave de polimorfismo real: el resto del sistema
 * nunca pregunta "if (tipo == Bootcamp)"; simplemente delega la decision
 * al objeto ProgramaAcademico correspondiente.
 */
public abstract class ProgramaAcademico {

    private String nombre;

    public ProgramaAcademico(String nombre) {
        this.nombre = nombre;
    }

    public String getNombre() {
        return nombre;
    }

    /**
     * Evalua si un conjunto de 3 notas aprueba este programa academico.
     * Cada subclase debe implementar su propia regla.
     */
    public abstract boolean evaluarAprobacion(double[] notas);
}
