#include "Tarea.h"
#include <iostream>
#include <iomanip>



using namespace std;



const double Tarea::LIMITE_MEMORIA_GB = 32.0;



Tarea::Tarea() : id(0), memoriaGB(0), prioridad('N'), nombreProceso(""), siguiente(NULL) {
}



Tarea::Tarea(int idTarea, double memoria, char tipoPrioridad, const string& nombre)
	: id(idTarea), memoriaGB(memoria), prioridad(tipoPrioridad),
	nombreProceso(nombre), siguiente(NULL) {
}



int Tarea::getId() const {
	return id;
}

double Tarea::getMemoriaGB() const {
	return memoriaGB;
}

char Tarea::getPrioridad() const {
	return prioridad;
}

string Tarea::getPrioridadTexto() const {
	return (prioridad == 'C') ? "Critica (C)" : "Normal (N)";
}

string Tarea::getNombreProceso() const {
	return nombreProceso;
}

bool Tarea::requiereAltoRendimiento() const {
	return memoriaGB > LIMITE_MEMORIA_GB || prioridad == 'C';
}

bool Tarea::esPrioridadValida(char valor) {
	return valor == 'N' || valor == 'C';
}



void Tarea::mostrarDetalle()
const {
	cout << "   ID de Tarea       : " << id << "\n";
	cout << "   Nombre del Proceso: " << nombreProceso << "\n";
	cout << "   Consumo de Memoria: " << fixed << setprecision(2) << memoriaGB << " GB\n";
	cout << "   Prioridad         : " << getPrioridadTexto() << "\n";
}



void Tarea::mostrarFila(int posicion) const {
	cout << "Posicion | ID | Proceso | Memoria | Prioridad" << endl;
	cout << posicion << " | ";
	cout << id << " | ";
	cout << nombreProceso << " | ";
	cout << memoriaGB << " GB | ";
	cout << getPrioridadTexto() << endl;
}
