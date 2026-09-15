/**
 * Alumno: hereda de Persona y agrega el Programa Academico asignado
 * y una lista (arreglo) de hasta 3 notas.
 */
public class Alumno extends Persona {

    private ProgramaAcademico programa;
    private double[] notas;

    /** Constructor para un alumno recien registrado (notas en 0). */
    public Alumno(String cedula, String nombreCompleto, String correoElectronico, ProgramaAcademico programa) {
        super(cedula, nombreCompleto, correoElectronico);
        this.programa = programa;
        this.notas = new double[]{0.0, 0.0, 0.0};
    }

    /** Constructor usado al cargar un alumno ya existente desde alumnos.txt. */
    public Alumno(String cedula, String nombreCompleto, String correoElectronico,
                  ProgramaAcademico programa, double[] notas) {
        super(cedula, nombreCompleto, correoElectronico);
        this.programa = programa;
        this.notas = notas;
    }

    public ProgramaAcademico getPrograma() {
        return programa;
    }

    public double[] getNotas() {
        return notas;
    }

    public double promedio() {
        double suma = 0;
        for (double n : notas) {
            suma += n;
        }
        return suma / notas.length;
    }

    /** Delega la regla de aprobacion al objeto ProgramaAcademico (polimorfismo real). */
    public boolean estaAprobado() {
        return programa.evaluarAprobacion(notas);
    }

    /** Devuelve el indice de la primera nota aun no registrada (0.0), o -1 si ya tiene las 3. */
    public int slotLibreParaNota() {
        for (int i = 0; i < notas.length; i++) {
            if (notas[i] == 0.0) {
                return i;
            }
        }
        return -1;
    }

    public void setNota(int indice, double valor) {
        notas[indice] = valor;
    }

    /** Serializa al formato: Cedula,Nombre,Correo,TipoPrograma,Nota1,Nota2,Nota3 */
    public String aLineaTxt() {
        return getCedula() + "," + getNombreCompleto() + "," + getCorreoElectronico() + "," +
                programa.getNombre() + "," +
                formatNota(notas[0]) + "," + formatNota(notas[1]) + "," + formatNota(notas[2]);
    }

    /** Imprime las notas enteras sin decimales (10 en vez de 10.0), igual que la data de ejemplo del enunciado. */
    private String formatNota(double n) {
        if (n == Math.floor(n) && !Double.isInfinite(n)) {
            return String.valueOf((long) n);
        }
        return String.valueOf(n);
    }
}
