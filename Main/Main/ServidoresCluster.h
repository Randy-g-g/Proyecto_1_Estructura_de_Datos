#ifndef SERVIDORESCLUSTER_H
#define SERVIDORESCLUSTER_H

#include <string>
#include "Servidor.h"

using namespace std;

class ServidoresCluster {
private:
	Servidor* primero;
	int cantidad;
	int siguienteIpTarea;

	Servidor* buscarMenosCarga(bool solohighPerformance) const;

	ServidoresCluster(const ServidoresCluster&);
	ServidoresCluster& operator = (const ServidoresCluster&);

public:
	static const int idMin = 1;
	static const int idMax = 8;

	ServidoresCluster();
	~ServidoresCluster();  //destructor para liberar espacio en memoria

	int getCantidad() const;
	int getTotalTareasPendientes() const;
	Servidor* buscarPorId(int id) const;
	Servidor* buscarPorIp(const string& ip) const; //declaracion de metodo que busca sin distingir mayusculas
	Servidor* buscarServidorDeTarea(int idTarea) const;

	//Primer Modulo 1 Servidores
	bool registrarServidor(int id, const string& nombre, const string& ip, int arquitectura, string& mensaje);
	void mostrarEstado() const;
	bool actualizarNombre(int id, const string& nuevoNombre, string& mensaje);
	bool actualizarArquitectura(int id, int nuevaArquitectura, string& mensaje);
	bool eliminarServidor(int id, string& mensaje);

	//Segundo Modulo 2: Tareas
	bool encolarTarea(double memoriaGB, char prioridad, const string& nombreProceso, string& mensaje);
	bool ejecutarTarea(int idServidor, string& mensaje);
	bool eliminarTarea(int idTarea, string& mensaje);
	void mostrarCola(int idServidor) const;
};

#endif