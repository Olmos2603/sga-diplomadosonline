import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Scanner;
import java.util.Stack;

/**
 * Orquesta la memoria (ArrayList), la persistencia en archivos .txt,
 * la Pila (Stack) para deshacer notas y la Cola (Queue) para generar
 * el reporte de certificados pendientes.
 */
public class SistemaSGA {

    private static final String ARCHIVO_ALUMNOS = "alumnos.txt";
    private static final String ARCHIVO_PROFESORES = "profesores.txt";
    private static final String ARCHIVO_CERTIFICADOS = "certificados_pendientes.txt";

    private List<Alumno> alumnos;
    private List<Profesor> profesores;
    private Stack<AccionNota> pilaDeshacer; // Pila (LIFO) - coleccion oficial de Java

    public SistemaSGA() {
        this.alumnos = new ArrayList<>();
        this.profesores = new ArrayList<>();
        this.pilaDeshacer = new Stack<>();
        cargarDesdeDisco();
    }

    private ProgramaAcademico crearProgramaPorNombre(String nombre) {
        switch (nombre.trim()) {
            case "Curso":
                return new Curso();
            case "Diplomado":
                return new Diplomado();
            case "Bootcamp":
                return new Bootcamp();
            default:
                throw new IllegalArgumentException("Programa academico desconocido: " + nombre);
        }
    }

    // -----------------------------------------------------------------
    // Carga inicial desde disco (persistencia real, ver EVAL-01)
    // -----------------------------------------------------------------
    private void cargarDesdeDisco() {
        File archivoAlumnos = new File(ARCHIVO_ALUMNOS);
        if (archivoAlumnos.exists()) {
            try (BufferedReader br = new BufferedReader(new FileReader(archivoAlumnos))) {
                String linea;
                while ((linea = br.readLine()) != null) {
                    if (linea.trim().isEmpty()) {
                        continue;
                    }
                    String[] partes = linea.split(",");
                    String cedula = partes[0];
                    String nombre = partes[1];
                    String correo = partes[2];
                    String tipo = partes[3];
                    double[] notas = new double[]{
                            Double.parseDouble(partes[4]),
                            Double.parseDouble(partes[5]),
                            Double.parseDouble(partes[6])
                    };
                    ProgramaAcademico programa = crearProgramaPorNombre(tipo);
                    alumnos.add(new Alumno(cedula, nombre, correo, programa, notas));
                }
            } catch (IOException | NumberFormatException e) {
                System.out.println("Aviso: no se pudo leer alumnos.txt correctamente (" + e.getMessage() + ")");
            }
        }

        File archivoProfesores = new File(ARCHIVO_PROFESORES);
        if (archivoProfesores.exists()) {
            try (BufferedReader br = new BufferedReader(new FileReader(archivoProfesores))) {
                String linea;
                while ((linea = br.readLine()) != null) {
                    if (linea.trim().isEmpty()) {
                        continue;
                    }
                    String[] partes = linea.split(",");
                    profesores.add(new Profesor(partes[0], partes[1], partes[2], partes[3], partes[4]));
                }
            } catch (IOException e) {
                System.out.println("Aviso: no se pudo leer profesores.txt correctamente (" + e.getMessage() + ")");
            }
        }
    }

    // -----------------------------------------------------------------
    // Persistencia: se reescriben los archivos completos tras cada cambio
    // -----------------------------------------------------------------
    public void guardarAlumnos() {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(ARCHIVO_ALUMNOS))) {
            for (Alumno a : alumnos) {
                bw.write(a.aLineaTxt());
                bw.newLine();
            }
        } catch (IOException e) {
            System.out.println("Error al guardar alumnos.txt: " + e.getMessage());
        }
    }

    public void guardarProfesores() {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(ARCHIVO_PROFESORES))) {
            for (Profesor p : profesores) {
                bw.write(p.aLineaTxt());
                bw.newLine();
            }
        } catch (IOException e) {
            System.out.println("Error al guardar profesores.txt: " + e.getMessage());
        }
    }

    private Alumno buscarAlumno(String cedula) {
        for (Alumno a : alumnos) {
            if (a.getCedula().equals(cedula)) {
                return a;
            }
        }
        return null;
    }

    private Profesor buscarProfesor(String cedula) {
        for (Profesor p : profesores) {
            if (p.getCedula().equals(cedula)) {
                return p;
            }
        }
        return null;
    }

    private boolean cedulaYaExiste(String cedula) {
        return buscarAlumno(cedula) != null || buscarProfesor(cedula) != null;
    }

    // -----------------------------------------------------------------
    // OPCION 1: Registrar Alumno
    // -----------------------------------------------------------------
    public void registrarAlumno(Scanner sc) {
        System.out.println("\n--- REGISTRAR ALUMNO ---");
        System.out.print("Cedula/ID: ");
        String cedula = sc.nextLine().trim();

        if (cedulaYaExiste(cedula)) {
            System.out.println("Error: ya existe una persona registrada con la cedula '" + cedula + "'.");
            return;
        }

        System.out.print("Nombre completo: ");
        String nombre = sc.nextLine().trim();
        System.out.print("Correo electronico: ");
        String correo = sc.nextLine().trim();

        ProgramaAcademico programa = null;
        while (programa == null) {
            System.out.println("Tipo de programa:  1) Curso   2) Diplomado   3) Bootcamp");
            System.out.print("Seleccione (1-3): ");
            String opcion = sc.nextLine().trim();
            switch (opcion) {
                case "1":
                    programa = new Curso();
                    break;
                case "2":
                    programa = new Diplomado();
                    break;
                case "3":
                    programa = new Bootcamp();
                    break;
                default:
                    System.out.println("Error: Ingrese un valor numerico valido (1, 2 o 3).");
            }
        }

        Alumno alumno = new Alumno(cedula, nombre, correo, programa);
        alumnos.add(alumno);
        guardarAlumnos();
        System.out.println("Alumno '" + nombre + "' registrado y guardado en alumnos.txt");
    }

    // -----------------------------------------------------------------
    // OPCION 2: Registrar Profesor
    // -----------------------------------------------------------------
    public void registrarProfesor(Scanner sc) {
        System.out.println("\n--- REGISTRAR PROFESOR ---");
        System.out.print("Cedula/ID: ");
        String cedula = sc.nextLine().trim();

        if (cedulaYaExiste(cedula)) {
            System.out.println("Error: ya existe una persona registrada con la cedula '" + cedula + "'.");
            return;
        }

        System.out.print("Nombre completo: ");
        String nombre = sc.nextLine().trim();
        System.out.print("Correo electronico: ");
        String correo = sc.nextLine().trim();
        System.out.print("Especialidad academica (ej. Python, Java, C++): ");
        String especialidad = sc.nextLine().trim();
        System.out.print("Materia asignada: ");
        String materia = sc.nextLine().trim();

        Profesor profesor = new Profesor(cedula, nombre, correo, especialidad, materia);
        profesores.add(profesor);
        guardarProfesores();
        System.out.println("Profesor '" + nombre + "' registrado y guardado en profesores.txt");
    }

    // -----------------------------------------------------------------
    // OPCION 3: Registrar Notas a un Alumno
    // -----------------------------------------------------------------
    public void registrarNotas(Scanner sc) {
        System.out.println("\n--- REGISTRAR NOTAS ---");
        System.out.print("Cedula del alumno: ");
        String cedula = sc.nextLine().trim();
        Alumno alumno = buscarAlumno(cedula);

        if (alumno == null) {
            System.out.println("Error: no se encontro ningun alumno con la cedula '" + cedula + "'.");
            return;
        }

        int indice = alumno.slotLibreParaNota();
        if (indice == -1) {
            System.out.println("Aviso: " + alumno.getNombreCompleto() + " ya tiene sus 3 notas registradas.");
            return;
        }

        double nota = 0;
        boolean valido = false;
        while (!valido) {
            System.out.print("Ingrese nota #" + (indice + 1) + " para " + alumno.getNombreCompleto() + ": ");
            String entrada = sc.nextLine().trim();
            try {
                nota = Double.parseDouble(entrada);
                valido = true;
            } catch (NumberFormatException e) {
                // EVAL-03: no debe romper el programa ante entradas invalidas
                System.out.println("Error: Ingrese un valor numerico valido");
            }
        }

        alumno.setNota(indice, nota);
        // Se apila la accion para poder deshacerla luego (LIFO)
        pilaDeshacer.push(new AccionNota(alumno.getCedula(), indice));
        guardarAlumnos();
        System.out.println("Nota " + nota + " registrada correctamente para " + alumno.getNombreCompleto() + ".");
    }

    // -----------------------------------------------------------------
    // OPCION 4: Deshacer Ultimo Registro de Nota (Pila / LIFO)
    // -----------------------------------------------------------------
    public void deshacerUltimaNota() {
        System.out.println("\n--- DESHACER ULTIMO REGISTRO DE NOTA ---");
        if (pilaDeshacer.isEmpty()) {
            System.out.println("No hay ninguna accion pendiente para deshacer.");
            return;
        }

        AccionNota accion = pilaDeshacer.pop(); // LIFO: se saca el ultimo que entro
        Alumno alumno = buscarAlumno(accion.getCedulaAlumno());
        if (alumno == null) {
            System.out.println("Aviso: el alumno de esa accion ya no existe en el sistema.");
            return;
        }

        double valorAnterior = alumno.getNotas()[accion.getIndiceNota()];
        alumno.setNota(accion.getIndiceNota(), 0.0);
        guardarAlumnos();
        System.out.println("Se deshizo la nota " + valorAnterior + " (posicion " + (accion.getIndiceNota() + 1) +
                ") de " + alumno.getNombreCompleto() + ".");
    }

    // -----------------------------------------------------------------
    // OPCION 5: Generar Cola de Certificados (Cola / FIFO)
    // -----------------------------------------------------------------
    public void generarColaCertificados() {
        System.out.println("\n--- GENERAR COLA DE CERTIFICADOS ---");
        Queue<Alumno> cola = new LinkedList<>(); // Cola (FIFO) - coleccion oficial de Java

        for (Alumno a : alumnos) {
            if (a.estaAprobado()) {
                cola.offer(a);
            }
        }

        int total = cola.size();
        StringBuilder sb = new StringBuilder();
        sb.append("=".repeat(41)).append("\n");
        sb.append("REPORTE DE CERTIFICADOS PENDIENTES").append("\n");
        sb.append("=".repeat(41)).append("\n");
        sb.append("Total de graduandos en cola: ").append(total).append("\n\n");

        int contador = 1;
        while (!cola.isEmpty()) { // Procesamiento estrictamente FIFO
            Alumno a = cola.poll();
            String estatus = "APROBADO";
            if (a.getPrograma() instanceof Bootcamp) {
                estatus += " (Cumple regla de ninguna nota < 14)";
            }
            sb.append(contador).append(". [").append(a.getCedula()).append("] ").append(a.getNombreCompleto()).append("\n");
            sb.append("   - Programa: ").append(a.getPrograma().getNombre()).append("\n");
            sb.append("   - Promedio Final: ").append(Math.round(a.promedio() * 10.0) / 10.0).append("\n");
            sb.append("   - Estatus: ").append(estatus).append("\n\n");
            contador++;
        }

        sb.append("=".repeat(41)).append("\n");
        sb.append("* Fin del reporte - Generado por SGA-DO *").append("\n");

        try (BufferedWriter bw = new BufferedWriter(new FileWriter(ARCHIVO_CERTIFICADOS))) {
            bw.write(sb.toString());
        } catch (IOException e) {
            System.out.println("Error al escribir certificados_pendientes.txt: " + e.getMessage());
            return;
        }

        System.out.println("Cola procesada. Se exportaron " + total + " graduandos a certificados_pendientes.txt");
    }

    // -----------------------------------------------------------------
    // OPCION 6: Mostrar Reporte General
    // -----------------------------------------------------------------
    public void mostrarReporteGeneral() {
        System.out.println("\n" + "=".repeat(50));
        System.out.println("REPORTE GENERAL - SGA-DO");
        System.out.println("=".repeat(50));

        System.out.println("\nPROFESORES ACTIVOS (" + profesores.size() + "):");
        if (profesores.isEmpty()) {
            System.out.println("  (No hay profesores registrados)");
        }
        for (Profesor p : profesores) {
            System.out.println("  [" + p.getCedula() + "] " + p.getNombreCompleto() + " - " + p.getEspecialidad() +
                    " - " + p.getMateria() + " - " + p.getCorreoElectronico());
        }

        System.out.println("\nALUMNOS REGISTRADOS (" + alumnos.size() + "):");
        if (alumnos.isEmpty()) {
            System.out.println("  (No hay alumnos registrados)");
        }
        for (Alumno a : alumnos) {
            String estatus = a.estaAprobado() ? "APROBADO" : "REPROBADO";
            System.out.println("  [" + a.getCedula() + "] " + a.getNombreCompleto() + " - " +
                    a.getPrograma().getNombre() + " - Notas: " + Arrays.toString(a.getNotas()) +
                    " - Promedio: " + (Math.round(a.promedio() * 10.0) / 10.0) + " - Estatus: " + estatus);
        }
        System.out.println("=".repeat(50));
    }
}
