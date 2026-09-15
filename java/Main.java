import java.util.Scanner;

/**
 * SGA-DO: SISTEMA DE GESTION ACADEMICA - DIPLOMADOSONLINE.COM
 * Entregable 5 - Implementacion en Java
 *
 * Punto de entrada del programa: muestra el menu de consola en un bucle
 * continuo hasta que el usuario elige la Opcion 7 (Salir).
 */
public class Main {

    private static void mostrarMenu() {
        System.out.println("\n" + "=".repeat(50));
        System.out.println("SGA-DO: SISTEMA DIPLOMADOSONLINE");
        System.out.println("=".repeat(50));
        System.out.println("1. Registrar Alumno");
        System.out.println("2. Registrar Profesor");
        System.out.println("3. Registrar Notas a un Alumno");
        System.out.println("4. Deshacer Ultimo Registro de Nota");
        System.out.println("5. Generar Cola de Certificados");
        System.out.println("6. Mostrar Reporte General");
        System.out.println("7. Salir");
        System.out.println("=".repeat(50));
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        SistemaSGA sistema = new SistemaSGA();

        boolean salir = false;
        while (!salir) {
            mostrarMenu();
            System.out.print("Seleccione una opcion (1-7): ");
            String entrada = sc.nextLine().trim();
            int opcion;

            try {
                opcion = Integer.parseInt(entrada);
            } catch (NumberFormatException e) {
                // EVAL-03: entrada no numerica -> no debe romper el programa
                System.out.println("Error: Ingrese un valor numerico valido");
                continue;
            }

            switch (opcion) {
                case 1:
                    sistema.registrarAlumno(sc);
                    break;
                case 2:
                    sistema.registrarProfesor(sc);
                    break;
                case 3:
                    sistema.registrarNotas(sc);
                    break;
                case 4:
                    sistema.deshacerUltimaNota();
                    break;
                case 5:
                    sistema.generarColaCertificados();
                    break;
                case 6:
                    sistema.mostrarReporteGeneral();
                    break;
                case 7:
                    System.out.println("\nGuardando cambios pendientes y cerrando SGA-DO de forma segura...");
                    sistema.guardarAlumnos();
                    sistema.guardarProfesores();
                    System.out.println("Hasta luego.");
                    salir = true;
                    break;
                default:
                    System.out.println("Error: Opcion fuera de rango. Seleccione un numero entre 1 y 7.");
            }

            if (!salir) {
                // Pausa para que el resultado impreso arriba no desaparezca
                // empujado por el menu antes de que el usuario lo lea.
                System.out.print("\nPresione ENTER para volver al menu...");
                sc.nextLine();
            }
        }

        sc.close();
    }
}
