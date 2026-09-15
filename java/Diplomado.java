/**
 * Diplomado: se aprueba si el promedio de las 3 notas es >= 14/20.
 */
public class Diplomado extends ProgramaAcademico {

    public Diplomado() {
        super("Diplomado");
    }

    @Override
    public boolean evaluarAprobacion(double[] notas) {
        double suma = 0;
        for (double n : notas) {
            suma += n;
        }
        double promedio = suma / notas.length;
        return promedio >= 14;
    }
}
