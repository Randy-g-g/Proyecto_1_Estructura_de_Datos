#ifndef SERVIDORESCLUSTER_H
#define SERVIDORESCLUSTER_H

#include <string>
#include "Servidor.h"

using namespace std;

class ServidoresCluster {
	friend class Sistema;// Clase amiga para permitir el acceso a los miembros privados de ServidoresCluster desde Sistema
private:
	Servidor* primero;
	int cantidad;
	int siguienteIdTarea;

	Servidor* buscarMenosCarga(bool solohighPerformance) const;// Busca el servidor con menos carga, si solohighPerformance es true, solo considera servidores de alto rendimiento

	ServidoresCluster(const ServidoresCluster&); 
	ServidoresCluster& operator = (const ServidoresCluster&);

public:
	static const int idMin = 1;
	static const int idMax = 8;

	ServidoresCluster();
	~ServidoresCluster();  //destructor para liberar espacio en memoria

	int getTotalTareasPendientes() const;
	Servidor* buscarPorId(int id) const;
	Servidor* buscarPorIp(const string& ip) const; 
	Servidor* buscarServidorDeTarea(int idTarea) const;

	//Primer Modulo 1 Servidores
	bool registrarServidor(int id, const string& nombre, const string& ip, int arquitectura, string& mensaje);
	void mostrarEstado() const;
	bool actualizarNombre(int id, const string& nuevoNombre, string& mensaje);
	bool actualizarArquitectura(int id, int nuevaArquitectura, string& mensaje);
	bool eliminarServidor(int id, string& mensaje);

	//Segundo Modulo 2: Tareas
	bool encolarTarea(double memoriaGB, char prioridad, const string& nombreProceso, string& mensaje);//encola una tarea en el servidor con menos carga, si no hay servidores disponibles, devuelve false y un mensaje de error
	bool ejecutarTarea(int idServidor, string& mensaje);
	bool eliminarTarea(int idTarea, string& mensaje);
	bool mostrarCola(int idServidor) const;
};

#endif