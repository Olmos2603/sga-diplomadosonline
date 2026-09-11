import json
import os
from abc import ABC, abstractmethod
from typing import List, Optional


# ==========================================
# SUPERCLASE ABSTRACTA: Persona
# ==========================================
class Persona(ABC):
    def __init__(self, id_persona: str, nombre: str, apellido: str, edad: int, correo: str):
        self._id = id_persona
        self._nombre = nombre
        self._apellido = apellido
        self._edad = edad
        self._correo = correo

    def mostrarDatos(self) -> None:
        print(f"ID: {self._id} | Nombre: {self._nombre} {self._apellido} | Edad: {self._edad} | Correo: {self._correo}")

    @abstractmethod
    def obtenerRol(self) -> str:
        pass


# ==========================================
# SUPERCLASE ABSTRACTA: ProgramaAcademico
# ==========================================
class ProgramaAcademico(ABC):
    def __init__(self, id_programa: str, nombre: str, duracion: int, costo: float):
        self._id = id_programa
        self._nombre = nombre
        self._duracion = duracion
        self._costo = costo

    @abstractmethod
    def evaluarAprobacion(self, notas: List[float]) -> bool:
        pass


# ==========================================
# SUBCLASES DE ProgramaAcademico (Polimorfismo)
# ==========================================
class Curso(ProgramaAcademico):
    def evaluarAprobacion(self, notas: List[float]) -> bool:
        if not notas:
            return False
        promedio = sum(notas) / len(notas)
        return promedio >= 10.0


class Diplomado(ProgramaAcademico):
    def evaluarAprobacion(self, notas: List[float]) -> bool:
        if not notas:
            return False
        promedio = sum(notas) / len(notas)
        return promedio >= 14.0


class Bootcamp(ProgramaAcademico):
    def evaluarAprobacion(self, notas: List[float]) -> bool:
        if not notas:
            return False
        return all(nota >= 14.0 for nota in notas)


# ==========================================
# SUBCLASES DE Persona
# ==========================================
class Alumno(Persona):
    def __init__(self, id_persona: str, nombre: str, apellido: str, edad: int, correo: str, programa: Optional[ProgramaAcademico] = None):
        super().__init__(id_persona, nombre, apellido, edad, correo)
        self._notas: List[float] = []
        self._programaAcademico: Optional[ProgramaAcademico] = programa

    def registrarNota(self, nota: float) -> None:
        self._notas.append(nota)

    def calcularPromedio(self) -> float:
        if not self._notas:
            return 0.0
        return sum(self._notas) / len(self._notas)

    def estaAprobado(self) -> bool:
        if self._programaAcademico is None:
            return False
        return self._programaAcademico.evaluarAprobacion(self._notas)

    def getPrograma(self) -> Optional[ProgramaAcademico]:
        return self._programaAcademico

    def setPrograma(self, programa: ProgramaAcademico) -> None:
        self._programaAcademico = programa

    def obtenerRol(self) -> str:
        return "Alumno"

    def to_dict(self) -> dict:
        return {
            "id": self._id,
            "nombre": self._nombre,
            "apellido": self._apellido,
            "edad": self._edad,
            "correo": self._correo,
            "notas": self._notas,
            "programa": self._programaAcademico._nombre if self._programaAcademico else None
        }


class Profesor(Persona):
    def __init__(self, id_persona: str, nombre: str, apellido: str, edad: int, correo: str, especialidad: str, materia: str):
        super().__init__(id_persona, nombre, apellido, edad, correo)
        self._especialidad = especialidad
        self._materiaAsignada = materia

    def asignarMateria(self, materia: str) -> None:
        self._materiaAsignada = materia

    def actualizarEspecialidad(self, especialidad: str) -> None:
        self._especialidad = especialidad

    def getMateria(self) -> str:
        return self._materiaAsignada

    def getEspecialidad(self) -> str:
        return self._especialidad

    def obtenerRol(self) -> str:
        return "Profesor"

    def to_dict(self) -> dict:
        return {
            "id": self._id,
            "nombre": self._nombre,
            "apellido": self._apellido,
            "edad": self._edad,
            "correo": self._correo,
            "especialidad": self._especialidad,
            "materia": self._materiaAsignada
        }


# ==========================================
# CLASE PRINCIPAL: SistemaGestionAcademica
# ==========================================
class SistemaGestionAcademica:
    def __init__(self, archivo_datos: str = "datos_sga.json"):
        self._alumnos: List[Alumno] = []
        self._profesores: List[Profesor] = []
        self._programas: List[ProgramaAcademico] = [
            Curso("CUR-01", "Programación Básica", 40, 100.0),
            Diplomado("DIP-01", "Diplomado en Python Backend", 120, 250.0),
            Bootcamp("BTC-01", "Bootcamp Web Fullstack", 300, 500.0)
        ]
        self._archivo_datos = archivo_datos
        self.cargar_datos()

    def registrarAlumno(self, alumno: Alumno) -> None:
        self._alumnos.append(alumno)
        self.guardar_datos()

    def registrarProfesor(self, profesor: Profesor) -> None:
        self._profesores.append(profesor)
        self.guardar_datos()

    def buscarAlumno(self, cedula: str) -> Optional[Alumno]:
        for a in self._alumnos:
            if a._id == cedula:
                return a
        return None

    def buscarProfesor(self, cedula: str) -> Optional[Profesor]:
        for p in self._profesores:
            if p._id == cedula:
                return p
        return None

    def generarReporte(self) -> None:
        print("\n=======================================================")
        print("          REPORTE GENERAL DEL SGA-DO")
        print("=======================================================")
        print(f"\n--- ALUMNOS REGISTRADOS ({len(self._alumnos)}) ---")
        if not self._alumnos:
            print("No hay alumnos registrados.")
        for a in self._alumnos:
            prog = a.getPrograma()._nombre if a.getPrograma() else "Sin programa"
            estado = "APROBADO" if a.estaAprobado() else "NO APROBADO / EN CURSO"
            print(f"• ID: {a._id} | {a._nombre} {a._apellido} | Programa: {prog}")
            print(f"  Notas: {a._notas} | Promedio: {a.calcularPromedio():.2f} | Estado: {estado}\n")

        print(f"--- PROFESORES REGISTRADOS ({len(self._profesores)}) ---")
        if not self._profesores:
            print("No hay profesores registrados.")
        for p in self._profesores:
            print(f"• ID: {p._id} | Prof. {p._nombre} {p._apellido} | Especialidad: {p.getEspecialidad()} | Materia: {p.getMateria()}")
        print("=======================================================\n")

    # ----------------------------------------------------
    # Manejo de Archivos Nativos (JSON)
    # ----------------------------------------------------
    def guardar_datos(self) -> None:
        datos = {
            "alumnos": [a.to_dict() for a in self._alumnos],
            "profesores": [p.to_dict() for p in self._profesores]
        }
        with open(self._archivo_datos, "w", encoding="utf-8") as f:
            json.dump(datos, f, indent=4, ensure_ascii=False)

    def cargar_datos(self) -> None:
        if not os.path.exists(self._archivo_datos):
            return

        try:
            with open(self._archivo_datos, "r", encoding="utf-8") as f:
                datos = json.load(f)

                for item in datos.get("alumnos", []):
                    prog_obj = None
                    for p in self._programas:
                        if p._nombre == item.get("programa"):
                            prog_obj = p
                            break
                    alum = Alumno(
                        item["id"], item["nombre"], item["apellido"],
                        item["edad"], item["correo"], prog_obj
                    )
                    alum._notas = item.get("notas", [])
                    self._alumnos.append(alum)

                for item in datos.get("profesores", []):
                    prof = Profesor(
                        item["id"], item["nombre"], item["apellido"],
                        item["edad"], item["correo"],
                        item["especialidad"], item["materia"]
                    )
                    self._profesores.append(prof)
        except Exception as e:
            print(f"Aviso: No se pudieron cargar los datos previos ({e})")


# ==========================================
# MENÚ INTERACTIVO EN CONSOLA
# ==========================================
def menu_principal():
    sga = SistemaGestionAcademica()

    while True:
        print("\n=== SISTEMA DE GESTIÓN ACADÉMICA (SGA-DO) ===")
        print("1. Registrar Alumno")
        print("2. Registrar Profesor")
        print("3. Registrar Nota a Alumno")
        print("4. Generar Reporte General")
        print("5. Salir")

        opcion = input("Seleccione una opción (1-5): ").strip()

        if opcion == "1":
            print("\n--- REGISTRAR ALUMNO ---")
            cedula = input("Cédula / ID: ").strip()
            nombre = input("Nombre: ").strip()
            apellido = input("Apellido: ").strip()
            edad = int(input("Edad: ").strip())
            correo = input("Correo: ").strip()

            print("\nModalidades disponibles:")
            print("1. Curso (Programación Básica)")
            print("2. Diplomado (Diplomado en Python Backend)")
            print("3. Bootcamp (Bootcamp Web Fullstack)")
            m_opc = input("Seleccione modalidad (1-3): ").strip()

            programa = sga._programas[0]
            if m_opc == "2":
                programa = sga._programas[1]
            elif m_opc == "3":
                programa = sga._programas[2]

            nuevo_alumno = Alumno(cedula, nombre, apellido, edad, correo, programa)
            sga.registrarAlumno(nuevo_alumno)
            print(f"\n¡Alumno {nombre} {apellido} registrado con éxito y guardado en archivo!")

        elif opcion == "2":
            print("\n--- REGISTRAR PROFESOR ---")
            cedula = input("Cédula / ID: ").strip()
            nombre = input("Nombre: ").strip()
            apellido = input("Apellido: ").strip()
            edad = int(input("Edad: ").strip())
            correo = input("Correo: ").strip()
            especialidad = input("Especialidad: ").strip()
            materia = input("Materia asignada: ").strip()

            nuevo_profesor = Profesor(cedula, nombre, apellido, edad, correo, especialidad, materia)
            sga.registrarProfesor(nuevo_profesor)
            print(f"\n¡Profesor {nombre} {apellido} registrado con éxito y guardado en archivo!")

        elif opcion == "3":
            print("\n--- REGISTRAR NOTA ---")
            cedula = input("Ingrese la Cédula/ID del alumno: ").strip()
            alumno = sga.buscarAlumno(cedula)
            if alumno:
                nota = float(input(f"Ingrese nota para {alumno._nombre} {alumno._apellido}: "))
                alumno.registrarNota(nota)
                sga.guardar_datos()
                print("¡Nota registrada y datos actualizados!")
            else:
                print("Alumno no encontrado.")

        elif opcion == "4":
            sga.generarReporte()

        elif opcion == "5":
            print("\nSaliendo del SGA-DO. ¡Hasta luego!")
            break
        else:
            print("Opción inválida. Intente de nuevo.")


if __name__ == "__main__":
    menu_principal()