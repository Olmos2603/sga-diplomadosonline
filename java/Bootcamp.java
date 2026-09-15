/**
 * Bootcamp: NO se aprueba por promedio. Exige estrictamente que ninguna
 * nota individual sea menor a 14/20.
 */
public class Bootcamp extends ProgramaAcademico {

    public Bootcamp() {
        super("Bootcamp");
    }

    @Override
    public boolean evaluarAprobacion(double[] notas) {
        for (double n : notas) {
            if (n < 14) {
                return false;
            }
        }
        return true;
    }
}
