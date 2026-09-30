#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <string>
#include "Colas.h"

enum Arquitectura {
    HIGH_PERFORMANCE = 1,
    STANDARD = 2,
    MEMORY_OPTIMIZED = 3
}; //agregar enumeración para la arquitectura del servidor

class Servidor {
	friend class ServidoresCluster;// Clase amiga para permitir el acceso a los miembros privados de Servidor desde ServidoresCluster

private:
    int id;
    std::string nombre;
    std::string ip;
    Arquitectura arquitectura;
    Colas* colas;
    Servidor* siguiente;
    Servidor* atras;

    Servidor(const Servidor&);
	Servidor& operator=(const Servidor&);// Eliminar el constructor de copia y el operador de asignación para evitar copias accidentales

public:
    Servidor(int id, const std::string& nombre, const std::string& ip, Arquitectura arquitectura);
    ~Servidor();

    int getId() const;//obtiene el id del servidor
    std::string getNombre() const;//obtiene el nombre del servidor
    std::string getIp() const;//obtiene la ip del servidor
    Arquitectura getArquitectura() const;//obtiene la arquitectura del servidor
    std::string getArquitecturaTexto() const;//obtiene la arquitectura del servidor en texto
    Colas* getColas() const;//obtiene la cola de tareas del servidor
    int getTareasPendientes() const;//obtiene la cantidad de tareas pendientes del servidor

    void setNombre(const std::string& nuevoNombre);//modifica el nombre del servidor
    void setArquitectura(Arquitectura nuevaArquitectura);//modifica la arquitectura del servidor

    void mostrarEncabezado() const;// nombre, IP, arquitectura y tareas pendientes


    static std::string arquitecturaATexto(Arquitectura arquitectura);//convierte la arquitectura a texto
    static bool esArquitecturaValida(int valor);//valida si la arquitectura es valida
    static bool esNombreValido(const std::string& nombre, std::string& motivo);// valida el nombre del servidor
    static bool esIpv4Valida(const std::string& ip, std::string& motivo);   // formato A.B.C.D

    static const int LONGITUD_MAX_NOMBRE = 30;
    static const int LONGITUD_MAX_IP = 15;     // "255.255.255.255"
};

#endif
