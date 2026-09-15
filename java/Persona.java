/**
 * Clase base de la jerarquia de personas. Contiene los atributos comunes
 * a Alumno y Profesor: Cedula/ID, Nombre Completo y Correo Electronico.
 *
 * Los atributos son privados (encapsulamiento) y se exponen unicamente
 * a traves de metodos getter publicos.
 */
public abstract class Persona {

    private String cedula;
    private String nombreCompleto;
    private String correoElectronico;

    public Persona(String cedula, String nombreCompleto, String correoElectronico) {
        this.cedula = cedula;
        this.nombreCompleto = nombreCompleto;
        this.correoElectronico = correoElectronico;
    }

    public String getCedula() {
        return cedula;
    }

    public String getNombreCompleto() {
        return nombreCompleto;
    }

    public String getCorreoElectronico() {
        return correoElectronico;
    }
}
