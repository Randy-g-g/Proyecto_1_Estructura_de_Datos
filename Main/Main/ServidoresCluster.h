#ifndef SERVIDORESCLUSTER_H
#define SERVIDORESCLUSTER_H

#include <string>
#include "Servidor.h"

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

	bool estaVacio() const;
	bool estaLleno() const;
	int getCantidad() const;
	int getTotalTareasPendientes() const;
	Servidor* buscarPorId(int id) const;
	Servidor* buscarPorIp(const std::string& ip) const; //declaracion de metodo que busca sin distingir mayusculas
	Servidor* buscarServidorDeTarea(int idTarea) const;

	//Primer Modulo 1 Servidores
	bool registrarServidor(int id, const std::string& nombre, const std::string& ip, int arquitectura, std::string& mensaje);
	void mostrarEstado();
	bool actualizarNombre(int id, const std::string& nuevoNombre, std::string& mensaje);
	bool actualizarArquitectura(int id, int nuevaArquitectura, std::string& mensaje);
	bool eliminarServidor(int id, std::string& mensaje);

	//Segundo Modulo 2: Tareas
	bool encolarTarea(double memoriaGB, char prioridad, const std::string& nombreProceso, std::string& mensaje);
	bool ejecutarTarea(int idServidor, std::string& mensaje);
	bool eliminarTarea(int idTarea, std::string& mensaje);
	void mostrarCola(int idServidor) const;
};

#endif