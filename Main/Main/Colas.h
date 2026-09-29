#ifndef COLATAREAS_H
#define COLATAREAS_H

#include <string>
#include "Tarea.h"

class Colas {
    friend class Servidor;
private:
    Tarea* frente;
    Tarea* fin;
    int cantidad;

public:
    Colas(const Colas&) = delete;
    Colas& operator=(const Colas&) = delete;
    Colas();
    ~Colas();

    void enColar(int id, double memoriaGB, char prioridad, const std::string& nombreProceso);

    bool desenColar(Tarea& tareaEjecutada);

    bool cancelar(int idTarea);

    bool contiene(int idTarea) const;

    bool estaVacia() const;
    void mostrar() const;
};

#endif