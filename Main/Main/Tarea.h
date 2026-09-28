#ifndef TAREA_H
#define TAREA_H

#include <string>

class Tarea {
    friend class ColaTareas;// permite que ColaTareas acceda al enlace "siguiente" de Tarea

private:
    int id;
    double memoriaGB;
    char prioridad;
    std::string nombreProceso;
    Tarea* siguiente;

public:
    static const double LIMITE_MEMORIA_GB;   // 32 GB

    Tarea();
    Tarea(int id, double memoriaGB, char prioridad, const std::string& nombreProceso);

    int getId() const;//obtiene el id de la tarea
    double getMemoriaGB() const;//obtiene la memoria de la tarea
    char getPrioridad() const;//obtiene la prioridad de la tarea
    std::string getPrioridadTexto() const;//obtiene la prioridad de la tarea en texto
    std::string getNombreProceso() const;//obtiene el nombre del proceso de la tarea

    bool requiereAltoRendimiento() const;//determina si la tarea requiere alto rendimiento

    static bool esPrioridadValida(char prioridad);//determina si la prioridad es valida

    void mostrarDetalle() const;             // ficha completa de la tarea
    void mostrarFila(int posicion) const;    // una fila dentro de la tabla de la cola
};

#endif
