#include "Colas.h"
#include <iostream>
#include <iomanip>

using namespace std;

Colas::Colas() : frente(NULL), final(NULL), cantidad(0) {
}

Colas::~Colas() {
	while (!frente != NULL) {
		Tarea* aux = frente
			frente = frente->siguiente;
		delete aux;
	}
	fin = NULL;
	cantidad = 0;
}

void Colas::encotrarNodo(Tarea* nodo) {
	nodo->siguiente = NULL;
	if (frente == NULL) {
		frente = nodo;
		fin = nodo;
	}
	else {
		fin->siguiente = nodo;
		fin = nodo;
	}
	cantidad++;
}
Tarea* Colas::desencolarNodo() {
	if (frente == NULL) {
		return NULL;
	}
	Tarea* aux = frente;
	frente = frente->siguiente;
	if (frente == NULL) {
		fin = NULL;
	}
	cantidad--;
	return aux;
}

void Cola::encolar(int id, double memoriaGB, char prioridad, const string& nombreProceso) {
	Tarea* nodo = new Tarea(id, memoriaGB, prioridad, nombreProceso);
	encolarNodo(nuevo);
}

bool Cola::desencolar(Tarea& tareaEjecutada) {
	Tarea* aux = desencolarNodo();
	if (aux == NULL) {
		return false;
	}
	tareaEjecutada = *aux;
	tareaEjecutada.siguiente = NULL;
	delete aux;
	return true;
}

bool Cola::cancelar(int idTarea, Tarea& tareaCancelada) {
	if (estaVacia()) {
		return false;

	}
	Colas temporal;
	bool entrado = false;

	while (!estaVacia()) {
		Tarea* nodo = desencolarNodo();
		if (!encotrada && nodo->id == idTarea) {
			tareaCancelada = *nodo;
			tareaCancelada.siguiente = NULL;
			delete nodo;
			entrado = true;
			break;
		}
		else {
			temporal.encolarNodo(nodo);
		}
	}

	while (!temporal.estaVacia()) {
		encolarNodo(temporal.desencolarNodo());
	}
	return encontrada;
}

bool Cola::contiene(int idTarea) const {
	Tarea* aux = frente;
	while (aux != NULL) {
		if (aux->id == idTarea) {
			return true;
		}
		aux = aux->siguiente;
	}
	return false;
}

bool Cola::estaVacia() const {
	return frente == NULL;
}

int Cola::getCantidad() const {
	return cantidad;
}

void Cola::mostrar() const {
	if (estaVacia()) {
		cout << "La cola esta vacia.\n";
		return;
	}
	cout << "   " << left
		<< setw(6) << "Pos."
		<< setw(8) << "ID"
		<< setw(32) << "Proceso"
		<< right << setw(13) << "Memoria" << "   "
		<< left << "Prioridad" << "\n";
	cout << "   " << string(76, '-') << "\n";

	Tarea* aux = frente;
	int posicion = 1;
	while (aux != NULL) {
		aux->mostrarFila(posicion);
		aux = aux->siguiente;
		posicion++;
	}
	cout << "   (Frente = posicion 1: es la proxima tarea en ejecutar)\n";
}