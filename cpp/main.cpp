/*
 * SGA-DO: SISTEMA DE GESTION ACADEMICA - DIPLOMADOSONLINE.COM
 * Entregable 6 - Implementacion en C++
 *
 * Punto de entrada del programa: muestra el menu de consola en un bucle
 * continuo hasta que el usuario elige la Opcion 7 (Salir).
 */
#include "SistemaSGA.h"
#include <iostream>
#include <string>

static void mostrarMenu() {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "SGA-DO: SISTEMA DIPLOMADOSONLINE\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "1. Registrar Alumno\n";
    std::cout << "2. Registrar Profesor\n";
    std::cout << "3. Registrar Notas a un Alumno\n";
    std::cout << "4. Deshacer Ultimo Registro de Nota\n";
    std::cout << "5. Generar Cola de Certificados\n";
    std::cout << "6. Mostrar Reporte General\n";
    std::cout << "7. Salir\n";
    std::cout << std::string(50, '=') << "\n";
}

static std::string trimLocal(const std::string& s) {
    size_t inicio = s.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) {
        return "";
    }
    size_t fin = s.find_last_not_of(" \t\r\n");
    return s.substr(inicio, fin - inicio + 1);
}

int main() {
    SistemaSGA sistema; // construida en la pila; internamente usa new/delete para sus datos

    bool salir = false;
    while (!salir) {
        mostrarMenu();
        std::cout << "Seleccione una opcion (1-7): ";
        std::string entrada;
        std::getline(std::cin, entrada);
        entrada = trimLocal(entrada);

        int opcion;
        try {
            size_t procesados = 0;
            opcion = std::stoi(entrada, &procesados);
            if (procesados != entrada.size()) {
                throw std::invalid_argument("texto sobrante");
            }
        } catch (const std::exception&) {
            // EVAL-03: entrada no numerica -> no debe romper el programa
            std::cout << "Error: Ingrese un valor numerico valido\n";
            continue;
        }

        switch (opcion) {
            case 1:
                sistema.registrarAlumno();
                break;
            case 2:
                sistema.registrarProfesor();
                break;
            case 3:
                sistema.registrarNotas();
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
                std::cout << "\nGuardando cambios pendientes y cerrando SGA-DO de forma segura...\n";
                sistema.guardarAlumnos();
                sistema.guardarProfesores();
                std::cout << "Hasta luego.\n";
                salir = true;
                break;
            default:
                std::cout << "Error: Opcion fuera de rango. Seleccione un numero entre 1 y 7.\n";
        }

        if (!salir) {
            // Pausa para que el resultado impreso arriba no desaparezca
            // empujado por el menu antes de que el usuario lo lea.
            std::cout << "\nPresione ENTER para volver al menu...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    }

    // Al salir del scope, el destructor de "sistema" libera con "delete"
    // todos los Alumno*/Profesor* creados con "new" durante la ejecucion.
    return 0;
}
