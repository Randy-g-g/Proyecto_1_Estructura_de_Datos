#include "Colas.h"
#include <iostream>

using namespace std;

// Constructor
Colas::Colas() {
    colaInicio = NULL;
    colaFin = NULL;
    cantidad = 0;
}

// Destructor
Colas::~Colas() {
    while (colaInicio != NULL) {
        Tarea* aux = colaInicio;
        colaInicio = colaInicio->siguiente;
        delete aux;
    }
}

// Agregar una tarea a la cola
void Colas::enColar(int id, double memoriaGB, char prioridad, const string& nombreProceso) {

    Tarea* nuevo = new Tarea(id, memoriaGB, prioridad, nombreProceso);

    if (colaInicio == NULL) {
        colaInicio = nuevo;
        colaFin = nuevo;
    }
    else {
        colaFin->siguiente = nuevo;
        colaFin = nuevo;
    }

    cantidad++;
}

// Sacar la primera tarea
bool Colas::desenColar(Tarea& tareaEjecutada) {

    if (colaInicio == NULL) {
        return false;
    }

    Tarea* aux = colaInicio;

    tareaEjecutada = *aux;

    colaInicio = colaInicio->siguiente;

    if (colaInicio == NULL) {
        colaFin = NULL;
    }

    tareaEjecutada.siguiente = NULL;

    delete aux;
    cantidad--;

    return true;
}

// Cancelar una tarea buscando por ID
bool Colas::cancelar(int idTarea) {

    if (colaInicio == NULL) {
        return false;
    }

    Colas temporal;
    Tarea actual;
    bool encontrada = false;

    // 1. Desencolado temporal
    while (desenColar(actual)) {
        if (!encontrada && actual.id == idTarea) {
            encontrada = true;          // esta tarea no se re-encola (se cancela)
        }
        else {
			temporal.enColar(actual.id, actual.memoriaGB, actual.prioridad, actual.nombreProceso);// re-encolado temporal
            }
    }

    // 2. Re-encolado de los elementos sobrantes en el mismo orden
    while (temporal.desenColar(actual)) {       
		enColar(actual.id, actual.memoriaGB, actual.prioridad, actual.nombreProceso);// re-encolado original
    }

	return encontrada;// devuelve true si se encontró y canceló la tarea, false si no se encontró
}

// Buscar si existe una tarea
bool Colas::contiene(int idTarea) const {

    Tarea* aux = colaInicio;

    while (aux != NULL) {

        if (aux->id == idTarea) {
            return true;
        }

        aux = aux->siguiente;
    }

    return false;
}

// Saber si la cola esta vacia
bool Colas::estaVacia() const {
    return colaInicio == NULL;
}

// Mostrar las tareas
void Colas::mostrar() const {

    if (estaVacia()) {
        cout << "La cola esta vacia." << endl;
        return;
    }

    cout << "Pos | ID | Proceso | Memoria | Prioridad" << endl;

    Tarea* aux = colaInicio;
    int posicion = 1;

    while (aux != NULL) {

        aux->mostrarFila(posicion);

        aux = aux->siguiente;
        posicion++;
    }
}