#ifndef COLATAREAS_H
#define COLATAREAS_H

#include <string>
#include "Tarea.h"

class Colas {
	friend class Servidor;// Clase amiga para permitir el acceso a los miembros privados de Colas desde Servidor
private:
    Tarea* Inicio;
    Tarea* final;
    int cantidad;

public:
    Colas(const Colas&) = delete;
    Colas& operator=(const Colas&) = delete;
    Colas();
    ~Colas();

	void enColar(int id, double memoriaGB, char prioridad, const std::string& nombreProceso); // Agrega una tarea a la cola

	bool desenColar(Tarea& tareaEjecutada); // Elimina la tarea al frente de la cola y la devuelve a través del parámetro tareaEjecutada

	bool cancelar(int idTarea); // Elimina una tarea específica de la cola según su ID

	bool contiene(int idTarea) const; // Verifica si una tarea con un ID específico está presente en la cola

    bool estaVacia() const;
	void mostrar() const;// Muestra la información de todas las tareas en la cola
};

#endif