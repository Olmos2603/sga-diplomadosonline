"""
================================================================================
 SGA-DO: SISTEMA DE GESTION ACADEMICA - DIPLOMADOSONLINE.COM
 Entregable 4 - Implementacion en Python
 Diplomado en Programacion - Proyecto: Sistema de Gestion Academica
================================================================================

"""

import os
from collections import deque

# ---------------------------------------------------------------------------
# Rutas de los archivos de persistencia (se crean junto al script)
# ---------------------------------------------------------------------------
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
ARCHIVO_ALUMNOS = os.path.join(BASE_DIR, "alumnos.txt")
ARCHIVO_PROFESORES = os.path.join(BASE_DIR, "profesores.txt")
ARCHIVO_CERTIFICADOS = os.path.join(BASE_DIR, "certificados_pendientes.txt")


# ===========================================================================
# PARTE II.1 - JERARQUIA DE PERSONAS (Herencia)
# ===========================================================================
class Persona:
    """Clase base con los atributos comunes a Alumno y Profesor."""

    def __init__(self, cedula, nombre_completo, correo_electronico):
        self.cedula = cedula
        self.nombre_completo = nombre_completo
        self.correo_electronico = correo_electronico


class Alumno(Persona):
    """Un alumno tiene hasta 3 notas y un Programa Academico asignado."""

    def __init__(self, cedula, nombre_completo, correo_electronico, programa, notas=None):
        super().__init__(cedula, nombre_completo, correo_electronico)
        self.programa = programa  # instancia de ProgramaAcademico (polimorfismo)
        self.notas = notas if notas is not None else [0, 0, 0]

    def promedio(self):
        return sum(self.notas) / len(self.notas)

    def esta_aprobado(self):
        """Delega la regla de aprobacion al objeto ProgramaAcademico (polimorfismo real)."""
        return self.programa.evaluar_aprobacion(self.notas)

    def slot_libre_para_nota(self):
        """Devuelve el indice de la primera nota en 0 (aun no registrada), o None si esta llena."""
        for i, n in enumerate(self.notas):
            if n == 0:
                return i
        return None

    def a_linea_txt(self):
        # Cedula,Nombre,Correo,TipoPrograma,Nota1,Nota2,Nota3
        return "{},{},{},{},{},{},{}".format(
            self.cedula, self.nombre_completo, self.correo_electronico,
            self.programa.nombre, self.notas[0], self.notas[1], self.notas[2]
        )


class Profesor(Persona):
    """Un profesor tiene Especialidad Academica y Materia Asignada."""

    def __init__(self, cedula, nombre_completo, correo_electronico, especialidad, materia):
        super().__init__(cedula, nombre_completo, correo_electronico)
        self.especialidad = especialidad
        self.materia = materia

    def a_linea_txt(self):
        # Cedula,Nombre,Correo,Especialidad,Materia
        return "{},{},{},{},{}".format(
            self.cedula, self.nombre_completo, self.correo_electronico,
            self.especialidad, self.materia
        )


# ===========================================================================
# PARTE II.2 - JERARQUIA DE PROGRAMAS ACADEMICOS (Polimorfismo)
# ===========================================================================
class ProgramaAcademico:
    """Clase base. Cada subclase sobreescribe evaluar_aprobacion()."""

    nombre = "ProgramaAcademico"

    def evaluar_aprobacion(self, notas):
        raise NotImplementedError("Las subclases deben implementar evaluar_aprobacion().")


class Curso(ProgramaAcademico):
    nombre = "Curso"

    def evaluar_aprobacion(self, notas):
        # Se aprueba si el promedio de las 3 notas es >= 10/20
        return (sum(notas) / len(notas)) >= 10


class Diplomado(ProgramaAcademico):
    nombre = "Diplomado"

    def evaluar_aprobacion(self, notas):
        # Se aprueba si el promedio de las 3 notas es >= 14/20
        return (sum(notas) / len(notas)) >= 14


class Bootcamp(ProgramaAcademico):
    nombre = "Bootcamp"

    def evaluar_aprobacion(self, notas):
        # No se aprueba por promedio: ninguna nota individual puede ser < 14
        return all(n >= 14 for n in notas)


PROGRAMAS_DISPONIBLES = {
    "1": Curso,
    "2": Diplomado,
    "3": Bootcamp,
}


def crear_programa_por_nombre(nombre_programa):
    """Factory: reconstruye el objeto ProgramaAcademico correcto a partir del texto guardado en el .txt."""
    nombre_programa = nombre_programa.strip()
    if nombre_programa == "Curso":
        return Curso()
    if nombre_programa == "Diplomado":
        return Diplomado()
    if nombre_programa == "Bootcamp":
        return Bootcamp()
    raise ValueError(f"Programa academico desconocido: {nombre_programa}")


# ===========================================================================
# SISTEMA PRINCIPAL (orquesta memoria + persistencia + Pila + Cola)
# ===========================================================================
class SistemaSGA:
    def __init__(self):
        self.alumnos = []       # lista en memoria de objetos Alumno
        self.profesores = []    # lista en memoria de objetos Profesor
        self.pila_deshacer = [] # Pila (LIFO): cada item = (cedula_alumno, indice_nota)
        self._cargar_desde_disco()

    # -----------------------------------------------------------------
    # Carga inicial desde disco (garantiza persistencia real, EVAL-01)
    # -----------------------------------------------------------------
    def _cargar_desde_disco(self):
        if os.path.exists(ARCHIVO_ALUMNOS):
            with open(ARCHIVO_ALUMNOS, "r", encoding="utf-8") as f:
                for linea in f:
                    linea = linea.strip()
                    if not linea:
                        continue
                    partes = linea.split(",")
                    cedula, nombre, correo, tipo, n1, n2, n3 = partes
                    programa = crear_programa_por_nombre(tipo)
                    notas = [int(float(n1)), int(float(n2)), int(float(n3))]
                    self.alumnos.append(Alumno(cedula, nombre, correo, programa, notas))

        if os.path.exists(ARCHIVO_PROFESORES):
            with open(ARCHIVO_PROFESORES, "r", encoding="utf-8") as f:
                for linea in f:
                    linea = linea.strip()
                    if not linea:
                        continue
                    cedula, nombre, correo, especialidad, materia = linea.split(",")
                    self.profesores.append(Profesor(cedula, nombre, correo, especialidad, materia))

    # -----------------------------------------------------------------
    # Persistencia: se reescriben los archivos completos tras cada cambio
    # -----------------------------------------------------------------
    def _guardar_alumnos(self):
        with open(ARCHIVO_ALUMNOS, "w", encoding="utf-8") as f:
            for a in self.alumnos:
                f.write(a.a_linea_txt() + "\n")

    def _guardar_profesores(self):
        with open(ARCHIVO_PROFESORES, "w", encoding="utf-8") as f:
            for p in self.profesores:
                f.write(p.a_linea_txt() + "\n")

    def _buscar_alumno(self, cedula):
        for a in self.alumnos:
            if a.cedula == cedula:
                return a
        return None

    def _buscar_profesor(self, cedula):
        for p in self.profesores:
            if p.cedula == cedula:
                return p
        return None

    # -----------------------------------------------------------------
    # OPCION 1: Registrar Alumno
    # -----------------------------------------------------------------
    def registrar_alumno(self):
        print("\n--- REGISTRAR ALUMNO ---")
        cedula = input("Cedula/ID: ").strip()

        # Integridad de datos: evitar cedulas duplicadas
        if self._buscar_alumno(cedula) or self._buscar_profesor(cedula):
            print(f"Error: ya existe una persona registrada con la cedula '{cedula}'.")
            return

        nombre = input("Nombre completo: ").strip()
        correo = input("Correo electronico: ").strip()

        print("Tipo de programa:  1) Curso   2) Diplomado   3) Bootcamp")
        while True:
            try:
                opcion_prog = input("Seleccione (1-3): ").strip()
                clase_programa = PROGRAMAS_DISPONIBLES[opcion_prog]
                break
            except KeyError:
                print("Error: Ingrese un valor numerico valido (1, 2 o 3).")

        alumno = Alumno(cedula, nombre, correo, clase_programa())
        self.alumnos.append(alumno)
        self._guardar_alumnos()
        print(f"Alumno '{nombre}' registrado y guardado en alumnos.txt")

    # -----------------------------------------------------------------
    # OPCION 2: Registrar Profesor
    # -----------------------------------------------------------------
    def registrar_profesor(self):
        print("\n--- REGISTRAR PROFESOR ---")
        cedula = input("Cedula/ID: ").strip()

        if self._buscar_alumno(cedula) or self._buscar_profesor(cedula):
            print(f"Error: ya existe una persona registrada con la cedula '{cedula}'.")
            return

        nombre = input("Nombre completo: ").strip()
        correo = input("Correo electronico: ").strip()
        especialidad = input("Especialidad academica (ej. Python, Java, C++): ").strip()
        materia = input("Materia asignada: ").strip()

        profesor = Profesor(cedula, nombre, correo, especialidad, materia)
        self.profesores.append(profesor)
        self._guardar_profesores()
        print(f"Profesor '{nombre}' registrado y guardado en profesores.txt")

    # -----------------------------------------------------------------
    # OPCION 3: Registrar Notas a un Alumno
    # -----------------------------------------------------------------
    def registrar_notas(self):
        print("\n--- REGISTRAR NOTAS ---")
        cedula = input("Cedula del alumno: ").strip()
        alumno = self._buscar_alumno(cedula)

        if alumno is None:
            print(f"Error: no se encontro ningun alumno con la cedula '{cedula}'.")
            return

        indice = alumno.slot_libre_para_nota()
        if indice is None:
            print(f"Aviso: {alumno.nombre_completo} ya tiene sus 3 notas registradas.")
            return

        while True:
            try:
                nota = float(input(f"Ingrese nota #{indice + 1} para {alumno.nombre_completo}: "))
                break
            except ValueError:
                print("Error: Ingrese un valor numerico valido")

        alumno.notas[indice] = nota
        # Se apila la accion para poder deshacerla luego (LIFO)
        self.pila_deshacer.append((alumno.cedula, indice))
        self._guardar_alumnos()
        print(f"Nota {nota} registrada correctamente para {alumno.nombre_completo}.")

    # -----------------------------------------------------------------
    # OPCION 4: Deshacer Ultimo Registro de Nota (Pila / LIFO)
    # -----------------------------------------------------------------
    def deshacer_ultima_nota(self):
        print("\n--- DESHACER ULTIMO REGISTRO DE NOTA ---")
        if not self.pila_deshacer:
            print("No hay ninguna accion pendiente para deshacer.")
            return

        cedula, indice = self.pila_deshacer.pop()  # LIFO: se saca el ultimo que entro
        alumno = self._buscar_alumno(cedula)
        if alumno is None:
            print("Aviso: el alumno de esa accion ya no existe en el sistema.")
            return

        valor_anterior = alumno.notas[indice]
        alumno.notas[indice] = 0
        self._guardar_alumnos()
        print(f"Se deshizo la nota {valor_anterior} (posicion {indice + 1}) de {alumno.nombre_completo}.")

    # -----------------------------------------------------------------
    # OPCION 5: Generar Cola de Certificados (Cola / FIFO)
    # -----------------------------------------------------------------
    def generar_cola_certificados(self):
        print("\n--- GENERAR COLA DE CERTIFICADOS ---")
        cola = deque()

        for alumno in self.alumnos:
            if alumno.esta_aprobado():
                cola.append(alumno)

        total = len(cola)
        lineas = []
        lineas.append("=" * 41)
        lineas.append("REPORTE DE CERTIFICADOS PENDIENTES")
        lineas.append("=" * 41)
        lineas.append(f"Total de graduandos en cola: {total}")
        lineas.append("")

        contador = 1
        while cola:  # Procesamiento estrictamente FIFO
            alumno = cola.popleft()
            estatus = "APROBADO"
            if isinstance(alumno.programa, Bootcamp):
                estatus += " (Cumple regla de ninguna nota < 14)"

            lineas.append(f"{contador}. [{alumno.cedula}] {alumno.nombre_completo}")
            lineas.append(f"   - Programa: {alumno.programa.nombre}")
            lineas.append(f"   - Promedio Final: {round(alumno.promedio(), 1)}")
            lineas.append(f"   - Estatus: {estatus}")
            lineas.append("")
            contador += 1

        lineas.append("=" * 41)
        lineas.append("* Fin del reporte - Generado por SGA-DO *")

        with open(ARCHIVO_CERTIFICADOS, "w", encoding="utf-8") as f:
            f.write("\n".join(lineas) + "\n")

        print(f"Cola procesada. Se exportaron {total} graduandos a certificados_pendientes.txt")

    # -----------------------------------------------------------------
    # OPCION 6: Mostrar Reporte General
    # -----------------------------------------------------------------
    def mostrar_reporte_general(self):
        print("\n" + "=" * 50)
        print("REPORTE GENERAL - SGA-DO")
        print("=" * 50)

        print(f"\nPROFESORES ACTIVOS ({len(self.profesores)}):")
        if not self.profesores:
            print("  (No hay profesores registrados)")
        for p in self.profesores:
            print(f"  [{p.cedula}] {p.nombre_completo} - {p.especialidad} - {p.materia} - {p.correo_electronico}")

        print(f"\nALUMNOS REGISTRADOS ({len(self.alumnos)}):")
        if not self.alumnos:
            print("  (No hay alumnos registrados)")
        for a in self.alumnos:
            estatus = "APROBADO" if a.esta_aprobado() else "REPROBADO"
            print(f"  [{a.cedula}] {a.nombre_completo} - {a.programa.nombre} - "
                  f"Notas: {a.notas} - Promedio: {round(a.promedio(), 1)} - Estatus: {estatus}")
        print("=" * 50)


# ===========================================================================
# PARTE III - MENU DE CONSOLA
# ===========================================================================
def mostrar_menu():
    print("\n" + "=" * 50)
    print("SGA-DO: SISTEMA DIPLOMADOSONLINE")
    print("=" * 50)
    print("1. Registrar Alumno")
    print("2. Registrar Profesor")
    print("3. Registrar Notas a un Alumno")
    print("4. Deshacer Ultimo Registro de Nota")
    print("5. Generar Cola de Certificados")
    print("6. Mostrar Reporte General")
    print("7. Salir")
    print("=" * 50)


def main():
    sistema = SistemaSGA()

    while True:
        mostrar_menu()
        try:
            opcion = input("Seleccione una opcion (1-7): ").strip()
            opcion_num = int(opcion)
        except ValueError:
            # EVAL-03: entrada no numerica -> no debe romper el programa
            print("Error: Ingrese un valor numerico valido")
            continue

        if opcion_num == 1:
            sistema.registrar_alumno()
        elif opcion_num == 2:
            sistema.registrar_profesor()
        elif opcion_num == 3:
            sistema.registrar_notas()
        elif opcion_num == 4:
            sistema.deshacer_ultima_nota()
        elif opcion_num == 5:
            sistema.generar_cola_certificados()
        elif opcion_num == 6:
            sistema.mostrar_reporte_general()
        elif opcion_num == 7:
            print("\nGuardando cambios pendientes y cerrando SGA-DO de forma segura...")
            sistema._guardar_alumnos()
            sistema._guardar_profesores()
            print("Hasta luego.")
            break
        else:
            print("Error: Opcion fuera de rango. Seleccione un numero entre 1 y 7.")

        if opcion_num != 7:
            # Pausa para que el resultado impreso arriba no desaparezca
            # empujado por el menu antes de que el usuario lo lea.
            input("\nPresione ENTER para volver al menu...")


if __name__ == "__main__":
    main()