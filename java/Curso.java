/**
 * Curso: se aprueba si el promedio de las 3 notas es >= 10/20.
 */
public class Curso extends ProgramaAcademico {

    public Curso() {
        super("Curso");
    }

    @Override
    public boolean evaluarAprobacion(double[] notas) {
        double suma = 0;
        for (double n : notas) {
            suma += n;
        }
        double promedio = suma / notas.length;
        return promedio >= 10;
    }
}
