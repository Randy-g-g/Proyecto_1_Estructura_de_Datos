#ifndef COLATAREAS_H
#define COLATAREAS_H

#include <string>
#include "Tarea.h"

class Colas {
private:
    Tarea* frente;
    Tarea* fin;
    int cantidad;

public:
    Colas();
    ~Colas();

    void encolar(int id, double memoriaGB, char prioridad, const std::string& nombreProceso);

    bool desencolar(Tarea& tareaEjecutada);

    bool cancelar(int idTarea);

    bool contiene(int idTarea) const;

    bool estaVacia() const;
    int getCantidad() const;
    void mostrar() const;
};

#endif