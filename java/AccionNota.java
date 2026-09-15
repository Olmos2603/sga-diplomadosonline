/**
 * Representa una accion "se agrego una nota" para poder apilarla y
 * deshacerla despues (Opcion 4 del menu, logica LIFO).
 */
public class AccionNota {

    private String cedulaAlumno;
    private int indiceNota;

    public AccionNota(String cedulaAlumno, int indiceNota) {
        this.cedulaAlumno = cedulaAlumno;
        this.indiceNota = indiceNota;
    }

    public String getCedulaAlumno() {
        return cedulaAlumno;
    }

    public int getIndiceNota() {
        return indiceNota;
    }
}
