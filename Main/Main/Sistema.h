#ifndef SISTEMA_H
#define SISTEMA_H

#include <string>
#include "ClusterServidores.h"

class Sistema {
private:
    ClusterServidores cluster;

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
    void cargarDatosDemostracion();
    int pedirArquitectura();
    int pedirIdServidorExistente(const std::string& mensaje);
    static void titulo(const std::string& texto);
    static void mostrarResultado(bool exito, const std::string& mensaje);

public:
    void iniciar();
};

#endif