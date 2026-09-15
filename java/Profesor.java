/**
 * Profesor: hereda de Persona y agrega Especialidad Academica y Materia Asignada.
 */
public class Profesor extends Persona {

    private String especialidad;
    private String materia;

    public Profesor(String cedula, String nombreCompleto, String correoElectronico,
                     String especialidad, String materia) {
        super(cedula, nombreCompleto, correoElectronico);
        this.especialidad = especialidad;
        this.materia = materia;
    }

    public String getEspecialidad() {
        return especialidad;
    }

    public String getMateria() {
        return materia;
    }

    /** Serializa al formato: Cedula,Nombre,Correo,Especialidad,Materia */
    public String aLineaTxt() {
        return getCedula() + "," + getNombreCompleto() + "," + getCorreoElectronico() + "," +
                especialidad + "," + materia;
    }
}
