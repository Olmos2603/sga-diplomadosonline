#include "SistemaSGA.h"
#include "Curso.h"
#include "Diplomado.h"
#include "Bootcamp.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <cctype>

const std::string SistemaSGA::ARCHIVO_ALUMNOS = "alumnos.txt";
const std::string SistemaSGA::ARCHIVO_PROFESORES = "profesores.txt";
const std::string SistemaSGA::ARCHIVO_CERTIFICADOS = "certificados_pendientes.txt";

// ---------------------------------------------------------------------
// Utilidades internas
// ---------------------------------------------------------------------
static std::string trim(const std::string& s) {
    size_t inicio = s.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) {
        return "";
    }
    size_t fin = s.find_last_not_of(" \t\r\n");
    return s.substr(inicio, fin - inicio + 1);
}

std::vector<std::string> SistemaSGA::splitCSV(const std::string& linea) {
    std::vector<std::string> partes;
    std::stringstream ss(linea);
    std::string campo;
    while (std::getline(ss, campo, ',')) {
        partes.push_back(campo);
    }
    return partes;
}

// ---------------------------------------------------------------------
// Construccion / destruccion
// ---------------------------------------------------------------------
SistemaSGA::SistemaSGA() {
    cargarDesdeDisco();
}

SistemaSGA::~SistemaSGA() {
    // Liberacion manual de memoria (EVAL-04): cada Alumno/Profesor fue
    // creado con "new"; aqui se libera con "delete" para no dejar fugas.
    for (Alumno* a : alumnos) {
        delete a; // el destructor de Alumno libera a su vez su ProgramaAcademico*
    }
    alumnos.clear();

    for (Profesor* p : profesores) {
        delete p;
    }
    profesores.clear();
}

ProgramaAcademico* SistemaSGA::crearProgramaPorNombre(const std::string& nombre) const {
    std::string n = trim(nombre);
    if (n == "Curso") {
        return new Curso();
    }
    if (n == "Diplomado") {
        return new Diplomado();
    }
    if (n == "Bootcamp") {
        return new Bootcamp();
    }
    throw std::invalid_argument("Programa academico desconocido: " + nombre);
}

// ---------------------------------------------------------------------
// Carga inicial desde disco (persistencia real, ver EVAL-01)
// ---------------------------------------------------------------------
void SistemaSGA::cargarDesdeDisco() {
    std::ifstream archivoAlumnos(ARCHIVO_ALUMNOS);
    if (archivoAlumnos.is_open()) {
        std::string linea;
        while (std::getline(archivoAlumnos, linea)) {
            if (trim(linea).empty()) {
                continue;
            }
            std::vector<std::string> partes = splitCSV(linea);
            if (partes.size() < 7) {
                continue;
            }
            try {
                std::string cedula = partes[0];
                std::string nombre = partes[1];
                std::string correo = partes[2];
                std::string tipo = partes[3];
                double notasIniciales[3] = {
                    std::stod(partes[4]),
                    std::stod(partes[5]),
                    std::stod(partes[6])
                };
                ProgramaAcademico* programa = crearProgramaPorNombre(tipo); // new
                Alumno* alumno = new Alumno(cedula, nombre, correo, programa, notasIniciales);
                alumnos.push_back(alumno);
            } catch (const std::exception& e) {
                std::cout << "Aviso: se omitio una linea invalida de alumnos.txt (" << e.what() << ")\n";
            }
        }
        archivoAlumnos.close();
    }

    std::ifstream archivoProfesores(ARCHIVO_PROFESORES);
    if (archivoProfesores.is_open()) {
        std::string linea;
        while (std::getline(archivoProfesores, linea)) {
            if (trim(linea).empty()) {
                continue;
            }
            std::vector<std::string> partes = splitCSV(linea);
            if (partes.size() < 5) {
                continue;
            }
            Profesor* profesor = new Profesor(partes[0], partes[1], partes[2], partes[3], partes[4]);
            profesores.push_back(profesor);
        }
        archivoProfesores.close();
    }
}

// ---------------------------------------------------------------------
// Persistencia: se reescriben los archivos completos tras cada cambio
// ---------------------------------------------------------------------
void SistemaSGA::guardarAlumnos() const {
    std::ofstream archivo(ARCHIVO_ALUMNOS, std::ios::trunc);
    if (!archivo.is_open()) {
        std::cout << "Error: no se pudo abrir alumnos.txt para escritura.\n";
        return;
    }
    for (const Alumno* a : alumnos) {
        archivo << a->aLineaTxt() << "\n";
    }
    archivo.close();
}

void SistemaSGA::guardarProfesores() const {
    std::ofstream archivo(ARCHIVO_PROFESORES, std::ios::trunc);
    if (!archivo.is_open()) {
        std::cout << "Error: no se pudo abrir profesores.txt para escritura.\n";
        return;
    }
    for (const Profesor* p : profesores) {
        archivo << p->aLineaTxt() << "\n";
    }
    archivo.close();
}

Alumno* SistemaSGA::buscarAlumno(const std::string& cedula) const {
    for (Alumno* a : alumnos) {
        if (a->getCedula() == cedula) {
            return a;
        }
    }
    return nullptr;
}

Profesor* SistemaSGA::buscarProfesor(const std::string& cedula) const {
    for (Profesor* p : profesores) {
        if (p->getCedula() == cedula) {
            return p;
        }
    }
    return nullptr;
}

bool SistemaSGA::cedulaYaExiste(const std::string& cedula) const {
    return buscarAlumno(cedula) != nullptr || buscarProfesor(cedula) != nullptr;
}

// ---------------------------------------------------------------------
// OPCION 1: Registrar Alumno
// ---------------------------------------------------------------------
void SistemaSGA::registrarAlumno() {
    std::cout << "\n--- REGISTRAR ALUMNO ---\n";
    std::cout << "Cedula/ID: ";
    std::string cedula;
    std::getline(std::cin, cedula);
    cedula = trim(cedula);

    if (cedulaYaExiste(cedula)) {
        std::cout << "Error: ya existe una persona registrada con la cedula '" << cedula << "'.\n";
        return;
    }

    std::cout << "Nombre completo: ";
    std::string nombre;
    std::getline(std::cin, nombre);
    nombre = trim(nombre);

    std::cout << "Correo electronico: ";
    std::string correo;
    std::getline(std::cin, correo);
    correo = trim(correo);

    ProgramaAcademico* programa = nullptr;
    while (programa == nullptr) {
        std::cout << "Tipo de programa:  1) Curso   2) Diplomado   3) Bootcamp\n";
        std::cout << "Seleccione (1-3): ";
        std::string opcion;
        std::getline(std::cin, opcion);
        opcion = trim(opcion);

        if (opcion == "1") {
            programa = new Curso();
        } else if (opcion == "2") {
            programa = new Diplomado();
        } else if (opcion == "3") {
            programa = new Bootcamp();
        } else {
            std::cout << "Error: Ingrese un valor numerico valido (1, 2 o 3).\n";
        }
    }

    Alumno* alumno = new Alumno(cedula, nombre, correo, programa); // memoria dinamica (new)
    alumnos.push_back(alumno);
    guardarAlumnos();
    std::cout << "Alumno '" << nombre << "' registrado y guardado en alumnos.txt\n";
}

// ---------------------------------------------------------------------
// OPCION 2: Registrar Profesor
// ---------------------------------------------------------------------
void SistemaSGA::registrarProfesor() {
    std::cout << "\n--- REGISTRAR PROFESOR ---\n";
    std::cout << "Cedula/ID: ";
    std::string cedula;
    std::getline(std::cin, cedula);
    cedula = trim(cedula);

    if (cedulaYaExiste(cedula)) {
        std::cout << "Error: ya existe una persona registrada con la cedula '" << cedula << "'.\n";
        return;
    }

    std::cout << "Nombre completo: ";
    std::string nombre;
    std::getline(std::cin, nombre);
    nombre = trim(nombre);

    std::cout << "Correo electronico: ";
    std::string correo;
    std::getline(std::cin, correo);
    correo = trim(correo);

    std::cout << "Especialidad academica (ej. Python, Java, C++): ";
    std::string especialidad;
    std::getline(std::cin, especialidad);
    especialidad = trim(especialidad);

    std::cout << "Materia asignada: ";
    std::string materia;
    std::getline(std::cin, materia);
    materia = trim(materia);

    Profesor* profesor = new Profesor(cedula, nombre, correo, especialidad, materia); // new
    profesores.push_back(profesor);
    guardarProfesores();
    std::cout << "Profesor '" << nombre << "' registrado y guardado en profesores.txt\n";
}

// ---------------------------------------------------------------------
// OPCION 3: Registrar Notas a un Alumno
// ---------------------------------------------------------------------
void SistemaSGA::registrarNotas() {
    std::cout << "\n--- REGISTRAR NOTAS ---\n";
    std::cout << "Cedula del alumno: ";
    std::string cedula;
    std::getline(std::cin, cedula);
    cedula = trim(cedula);

    Alumno* alumno = buscarAlumno(cedula);
    if (alumno == nullptr) {
        std::cout << "Error: no se encontro ningun alumno con la cedula '" << cedula << "'.\n";
        return;
    }

    int indice = alumno->slotLibreParaNota();
    if (indice == -1) {
        std::cout << "Aviso: " << alumno->getNombreCompleto() << " ya tiene sus 3 notas registradas.\n";
        return;
    }

    double nota = 0.0;
    bool valido = false;
    while (!valido) {
        std::cout << "Ingrese nota #" << (indice + 1) << " para " << alumno->getNombreCompleto() << ": ";
        std::string entrada;
        std::getline(std::cin, entrada);
        entrada = trim(entrada);
        try {
            size_t procesados = 0;
            nota = std::stod(entrada, &procesados);
            if (procesados != entrada.size()) {
                throw std::invalid_argument("texto sobrante");
            }
            valido = true;
        } catch (const std::exception&) {
            // EVAL-03: no debe romper el programa ante entradas invalidas
            std::cout << "Error: Ingrese un valor numerico valido\n";
        }
    }

    alumno->setNota(indice, nota);
    // Se apila la accion para poder deshacerla luego (LIFO)
    pilaDeshacer.push(AccionNota(alumno->getCedula(), indice));
    guardarAlumnos();
    std::cout << "Nota " << nota << " registrada correctamente para " << alumno->getNombreCompleto() << ".\n";
}

// ---------------------------------------------------------------------
// OPCION 4: Deshacer Ultimo Registro de Nota (Pila / LIFO)
// ---------------------------------------------------------------------
void SistemaSGA::deshacerUltimaNota() {
    std::cout << "\n--- DESHACER ULTIMO REGISTRO DE NOTA ---\n";
    if (pilaDeshacer.empty()) {
        std::cout << "No hay ninguna accion pendiente para deshacer.\n";
        return;
    }

    AccionNota accion = pilaDeshacer.top();
    pilaDeshacer.pop(); // LIFO: se saca el ultimo que entro

    Alumno* alumno = buscarAlumno(accion.cedulaAlumno);
    if (alumno == nullptr) {
        std::cout << "Aviso: el alumno de esa accion ya no existe en el sistema.\n";
        return;
    }

    double valorAnterior = alumno->getNotas()[accion.indiceNota];
    alumno->setNota(accion.indiceNota, 0.0);
    guardarAlumnos();
    std::cout << "Se deshizo la nota " << valorAnterior << " (posicion " << (accion.indiceNota + 1)
               << ") de " << alumno->getNombreCompleto() << ".\n";
}

// ---------------------------------------------------------------------
// OPCION 5: Generar Cola de Certificados (Cola / FIFO)
// ---------------------------------------------------------------------
void SistemaSGA::generarColaCertificados() {
    std::cout << "\n--- GENERAR COLA DE CERTIFICADOS ---\n";
    std::queue<Alumno*> cola; // Cola (FIFO) - contenedor oficial de la STL

    for (Alumno* a : alumnos) {
        if (a->estaAprobado()) {
            cola.push(a);
        }
    }

    int total = static_cast<int>(cola.size());
    std::ostringstream out;
    out << std::string(41, '=') << "\n";
    out << "REPORTE DE CERTIFICADOS PENDIENTES\n";
    out << std::string(41, '=') << "\n";
    out << "Total de graduandos en cola: " << total << "\n\n";

    int contador = 1;
    while (!cola.empty()) { // Procesamiento estrictamente FIFO
        Alumno* a = cola.front();
        cola.pop();

        std::string estatus = "APROBADO";
        if (dynamic_cast<Bootcamp*>(a->getPrograma()) != nullptr) {
            estatus += " (Cumple regla de ninguna nota < 14)";
        }

        double promedioRedondeado = std::round(a->promedio() * 10.0) / 10.0;

        out << contador << ". [" << a->getCedula() << "] " << a->getNombreCompleto() << "\n";
        out << "   - Programa: " << a->getPrograma()->getNombre() << "\n";
        out << "   - Promedio Final: " << promedioRedondeado << "\n";
        out << "   - Estatus: " << estatus << "\n\n";
        contador++;
    }

    out << std::string(41, '=') << "\n";
    out << "* Fin del reporte - Generado por SGA-DO *\n";

    std::ofstream archivo(ARCHIVO_CERTIFICADOS, std::ios::trunc);
    if (!archivo.is_open()) {
        std::cout << "Error: no se pudo escribir certificados_pendientes.txt\n";
        return;
    }
    archivo << out.str();
    archivo.close();

    std::cout << "Cola procesada. Se exportaron " << total << " graduandos a certificados_pendientes.txt\n";
}

// ---------------------------------------------------------------------
// OPCION 6: Mostrar Reporte General
// ---------------------------------------------------------------------
void SistemaSGA::mostrarReporteGeneral() const {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "REPORTE GENERAL - SGA-DO\n";
    std::cout << std::string(50, '=') << "\n";

    std::cout << "\nPROFESORES ACTIVOS (" << profesores.size() << "):\n";
    if (profesores.empty()) {
        std::cout << "  (No hay profesores registrados)\n";
    }
    for (const Profesor* p : profesores) {
        std::cout << "  [" << p->getCedula() << "] " << p->getNombreCompleto() << " - "
                   << p->getEspecialidad() << " - " << p->getMateria() << " - "
                   << p->getCorreoElectronico() << "\n";
    }

    std::cout << "\nALUMNOS REGISTRADOS (" << alumnos.size() << "):\n";
    if (alumnos.empty()) {
        std::cout << "  (No hay alumnos registrados)\n";
    }
    for (const Alumno* a : alumnos) {
        std::string estatus = a->estaAprobado() ? "APROBADO" : "REPROBADO";
        const double* notas = a->getNotas();
        double promedioRedondeado = std::round(a->promedio() * 10.0) / 10.0;

        std::cout << "  [" << a->getCedula() << "] " << a->getNombreCompleto() << " - "
                   << a->getPrograma()->getNombre() << " - Notas: ["
                   << notas[0] << ", " << notas[1] << ", " << notas[2] << "] - Promedio: "
                   << promedioRedondeado << " - Estatus: " << estatus << "\n";
    }
    std::cout << std::string(50, '=') << "\n";
}
