#include "Colas.h"
#include <iostream>

using namespace std;

// Constructor
Colas::Colas() {
    frente = NULL;
    fin = NULL;
    cantidad = 0;
}

// Destructor
Colas::~Colas() {
    while (frente != NULL) {
        Tarea* aux = frente;
        frente = frente->siguiente;
        delete aux;
    }
}

// Agregar una tarea a la cola
void Colas::encolar(int id, double memoriaGB, char prioridad, const string& nombreProceso) {

    Tarea* nuevo = new Tarea(id, memoriaGB, prioridad, nombreProceso);

    if (frente == NULL) {
        frente = nuevo;
        fin = nuevo;
    }
    else {
        fin->siguiente = nuevo;
        fin = nuevo;
    }

    cantidad++;
}

// Sacar la primera tarea
bool Colas::desencolar(Tarea& tareaEjecutada) {

    if (frente == NULL) {
        return false;
    }

    Tarea* aux = frente;

    tareaEjecutada = *aux;

    frente = frente->siguiente;

    if (frente == NULL) {
        fin = NULL;
    }

    tareaEjecutada.siguiente = NULL;

    delete aux;
    cantidad--;

    return true;
}

// Cancelar una tarea buscando por ID
bool Colas::cancelar(int idTarea) {

    if (frente == NULL) {
        return false;
    }

    Tarea* actual = frente;
    Tarea* anterior = NULL;

    while (actual != NULL) {

        if (actual->id == idTarea) {

            // Si es la primera tarea
            if (anterior == NULL) {
                frente = actual->siguiente;
            }
            else {
                anterior->siguiente = actual->siguiente;
            }

            // Si es la ultima tarea
            if (actual == fin) {
                fin = anterior;
            }

            delete actual;
            cantidad--;

            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    return false;
}

// Buscar si existe una tarea
bool Colas::contiene(int idTarea) const {

    Tarea* aux = frente;

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
    return frente == NULL;
}

// Obtener cantidad de tareas
int Colas::getCantidad() const {
    return cantidad;
}

// Mostrar las tareas
void Colas::mostrar() const {

    if (estaVacia()) {
        cout << "La cola esta vacia." << endl;
        return;
    }

    cout << "Pos | ID | Proceso | Memoria | Prioridad" << endl;

    Tarea* aux = frente;
    int posicion = 1;

    while (aux != NULL) {

        aux->mostrarFila(posicion);

        aux = aux->siguiente;
        posicion++;
    }
}