#ifndef SISTEMA_H
#define SISTEMA_H

#include <string>
#include "ServidoresCluster.h"

class Sistema {
private:
    ServidoresCluster cluster;

    // Menus
    void menuServidores();
    void menuTareas();

    // Servidores
    void registrarServidor();
    void modificarServidor();
    void darDeBajaServidor();

    // Tareas
    void encolarTarea();
    void ejecutarTarea();
    void cancelarTarea();
    void verColaDeServidor();

    // Otros
    int pedirArquitectura();
    int pedirIdServidorExistente(const std::string& mensaje);
    static void mostrarResultado(bool exito, const std::string& mensaje);

public:
    void iniciar();
};

#endif