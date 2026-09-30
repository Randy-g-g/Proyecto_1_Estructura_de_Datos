#include "Sistema.h"
#include "Datos.h"
#include <iostream>



using namespace std;






//////MENU PRINCIPAL
void Sistema::iniciar() {
	int opcion = -1;
	do {
		system("cls");
		cout << "Gestion de Cluster de Servidores y Tareas" << endl;
		cout << "   Servidores: " << cluster.cantidad << "/" << ServidoresCluster::idMax
			<< "   |   Tareas pendientes: " << cluster.getTotalTareasPendientes() << "\n\n";
		cout << "   1. Modulo de Servidores (Lista Doble Circular)\n";
		cout << "   2. Modulo de Tareas y Procesos (Cola FIFO)\n";
		cout << "   3. Mostrar estado del cluster\n";
		cout << "   0. Salir\n";
		opcion = Datos::leerEnteroEnRango("\n   Escoja una opcion: ", 0, 3);

		switch (opcion) {
		case 1:
			menuServidores();
			break;
		case 2:
			menuTareas();
			break;
		case 3:
			cluster.mostrarEstado();
			Datos::pausar();
			break;
		case 0:
			cout << "\n Liberando memoria del cluster... Programa finalizado.\n";
			break;
		}
	} while (opcion != 0);
}



//////MENU SERVIDORES
void Sistema::menuServidores() {
	int opcion_menu = 0;
	do {
		system("cls");
		cout << "\n 1. Registrar servidor";
		cout << "\n 2. Mostrar estado del cluster";
		cout << "\n 3. Modificar informacion de servidor";
		cout << "\n 4. Dar de baja a servidor";
		cout << "\n 0. Volver al menu principal";
		opcion_menu = Datos::leerEntero("\n\n Escoja una Opcion: ");

		switch (opcion_menu) {
		case 1:
			registrarServidor();
			Datos::pausar();
			break;
		case 2:
			cluster.mostrarEstado();
			Datos::pausar();
			break;
		case 3:
			modificarServidor();
			Datos::pausar();
			break;
		case 4:
			darDeBajaServidor();
			Datos::pausar();
			break;
		case 0:
			cout << "\n\n Regresando al menu principal...\n\n";
			break;
		default:
			cout << "\n\n Opcion No Valida \n\n";
		}
	} while (opcion_menu != 0);
}



//////MENU DE TAREAS
void Sistema::menuTareas() {
	int opcion_menu = 0;
	do {
		system("cls");
		cout << "\n 1. Encolar tarea (Asignar proceso)";
		cout << "\n 2. Desencolar tarea (Ejecutar proceso)";
		cout << "\n 3. Cancelar tarea (Eliminar por ID)";
		cout << "\n 4. Ver cola de tareas de un servidor";
		cout << "\n 0. Volver al menu principal";
		opcion_menu = Datos::leerEntero("\n\n Escoja una Opcion: ");

		switch (opcion_menu) {
		case 1:
			encolarTarea();
			Datos::pausar();
			break;
		case 2:
			ejecutarTarea();
			Datos::pausar();
			break;
		case 3:
			cancelarTarea();
			Datos::pausar();
			break;
		case 4:
			verColaDeServidor();
			Datos::pausar();
			break;
		case 0:
			cout << "\n\n Regresando al menu principal...\n\n";
			break;
		default:
			cout << "\n\n Opcion No Valida \n\n";
		}
	} while (opcion_menu != 0);
}



//  FUNCIONES AUXILIARES
void Sistema::mostrarResultado(bool exito, const string& mensaje) {
	cout << "\n " << (exito ? " valido " : "invalido ") << mensaje << "\n";
}

int Sistema::pedirArquitectura() {
	cout << "   Tipos de arquitectura:\n";
	cout << "     1. High-Performance\n";
	cout << "     2. Standard\n";
	cout << "     3. Memory-Optimized\n";
	return Datos::leerEnteroEnRango("   Seleccione entre 1-3 : ", 1, 3);
}

int Sistema::pedirIdServidorExistente(const string& mensaje) {
	while (true) {
		int id = Datos::leerEntero(mensaje);
		if (id == 0) {
			return 0;
		}
		if (cluster.buscarPorId(id) != NULL) {
			return id;
		}
		cout << "No existe un servidor con ese ID. (0 para cancelar)\n";
	}
}



//  SERVIDORES (METODOS)
void Sistema::registrarServidor() {
	cout << "Registrar un servidor" << endl;
	if (cluster.cantidad >= (ServidoresCluster::idMax - ServidoresCluster::idMin + 1)) {
		mostrarResultado(false, "El cluster ya tiene los 8 servidores permitidos (IDs 1 a 8).");
		return;
	}
	cout << "   (Escriba 0 en el ID para cancelar)\n\n";

	// ID: rango y no repetido
	int id = 0;
	while (true) {
		id = Datos::leerEntero("   ID del servidor (1-8): ");
		if (id == 0) {
			cout << "\n Registro cancelado.\n";
			return;
		}
		if (id < ServidoresCluster::idMin || id > ServidoresCluster::idMax) {
			cout << "El ID debe estar en el rango entre (1, 8).\n";
		}
		else if (cluster.buscarPorId(id) != NULL) {
			cout << "El ID " << id << " ya esta registrado. Use otro.\n";
		}
		else {
			break;
		}
	}

	// IP: formato IPv4
	string ip;
	while (true) {
		ip = Datos::recortar(Datos::leerLinea("   Direccion IPv4 (ej. 192.168.1.10): "));
		string motivo;
		if (!Servidor::esIpv4Valida(ip, motivo)) {
			cout << "Ip Invalida " << motivo << "\n";
		}
		else if (cluster.buscarPorIp(ip) != NULL) {
			cout << "Esa IP ya pertenece a otro servidor.\n";
		}
		else {
			break;
		}
	}

	// Nombre
	string nombre;
	while (true) {
		nombre = Datos::recortar(Datos::leerLinea("   Nombre del servidor (ej. Node-Alpha): "));
		string motivo;
		if (Servidor::esNombreValido(nombre, motivo)) {
			break;
		}
		cout << "invalido " << motivo << "\n";
	}

	// Arquitectura
	int arquitectura = pedirArquitectura();

	string mensaje;
	bool exito = cluster.registrarServidor(id, nombre, ip, arquitectura, mensaje);
	mostrarResultado(exito, mensaje);
}



void Sistema::modificarServidor() {
	cout << "Modificar servidor" << endl;
	if (cluster.primero == NULL) {
		mostrarResultado(false, "No hay servidores registrados.");
		return;
	}
	cluster.mostrarEstado();
	cout << "   Nota: el ID y la IP no se pueden modificar.\n";

	int id = pedirIdServidorExistente("\n   ID del servidor a modificar (0 = cancelar): ");
	if (id == 0) {
		cout << "\n Modificacion cancelada.\n";
		return;
	}
	Servidor* servidor = cluster.buscarPorId(id);
	cout << "\n";
	servidor->mostrarEncabezado();

	cout << "\n   Que desea modificar?\n";
	cout << "     1. Nombre\n";
	cout << "     2. Tipo de arquitectura\n";
	cout << "     3. Ambos\n";
	cout << "     0. Cancelar\n";
	int opcion = Datos::leerEnteroEnRango("   Opcion: ", 0, 3);
	if (opcion == 0) {
		cout << "\n Modificacion cancelada.\n";
		return;
	}

	string mensaje;
	if (opcion == 1 || opcion == 3) {
		while (true) {
			string nombre = Datos::recortar(Datos::leerLinea("   Nuevo nombre: "));
			string motivo;
			if (Servidor::esNombreValido(nombre, motivo)) {
				bool exito = cluster.actualizarNombre(id, nombre, mensaje);
				mostrarResultado(exito, mensaje);
				break;
			}
			cout << "Nombre invalido " << motivo << "\n";
		}
	}
	if (opcion == 2 || opcion == 3) {
		cout << "\n   Arquitectura actual: " << servidor->getArquitecturaTexto() << "\n";
		int arquitectura = pedirArquitectura();
		bool exito = cluster.actualizarArquitectura(id, arquitectura, mensaje);
		mostrarResultado(exito, mensaje);
	}
}



void Sistema::darDeBajaServidor() {
	cout << "Dar de baja a un servidor" << endl;
	if (cluster.primero == NULL) {
		mostrarResultado(false, "No hay servidores registrados.");
		return;
	}
	cluster.mostrarEstado();

	int id = pedirIdServidorExistente("\n   ID del servidor a dar de baja (0 = cancelar): ");
	if (id == 0) {
		cout << "\n Operacion cancelada.\n";
		return;
	}
	Servidor* servidor = cluster.buscarPorId(id);
	if (servidor->getTareasPendientes() == 0) {
		char confirmacion = Datos::leerCaracterValido(
			"   Confirma dar de baja a '" + servidor->getNombre() + "'? (S/N): ", "SN");
		if (confirmacion == 'N') {
			cout << "\n Operacion cancelada.\n";
			return;
		}
	}
	string mensaje;
	bool exito = cluster.eliminarServidor(id, mensaje);
	mostrarResultado(exito, mensaje);
}



//  TAREAS (METODOS)
void Sistema::encolarTarea() {
	cout << "Encolar tarea y asignar" << endl;
	if (cluster.primero == NULL) {
		mostrarResultado(false, "No hay servidores registrados. Registre un servidor primero.");
		return;
	}
	cout << " Regla > 32 GB o Critica -> High-Performance con menos tareas;\n";
	cout << "          Normal             -> servidor con menos tareas.\n\n";

	string nombre = Datos::leerTextoNoVacio("   Nombre del proceso: ", 40);
	double memoria = Datos::leerRealPositivo("   Consumo de memoria (GB): ");
	char prioridad = Datos::leerCaracterValido("   Prioridad (N = Normal, C = Critica): ", "NC");

	string mensaje;
	bool exito = cluster.encolarTarea(memoria, prioridad, nombre, mensaje);
	mostrarResultado(exito, mensaje);
}

void Sistema::ejecutarTarea() {
	cout << "Desencola tarea y ejecutar" << endl;
	if (cluster.primero == NULL) {
		mostrarResultado(false, "No hay servidores registrados.");
		return;
	}
	if (cluster.getTotalTareasPendientes() == 0) {
		mostrarResultado(false, "No hay tareas pendientes en ningun servidor.");
		return;
	}
	cluster.mostrarEstado();

	int id = pedirIdServidorExistente("\n   ID del servidor que ejecutara su tarea (0 = cancelar): ");
	if (id == 0) {
		cout << "\n Operacion cancelada.\n";
		return;
	}
	string mensaje;
	bool exito = cluster.ejecutarTarea(id, mensaje);
	mostrarResultado(exito, mensaje);
}

void Sistema::cancelarTarea() {
	cout << "Cancelar tarea (ELIMINAR POR ID)" << endl;
	if (cluster.getTotalTareasPendientes() == 0) {
		mostrarResultado(false, "No hay tareas pendientes para cancelar.");
		return;
	}
	int idTarea = Datos::leerEntero("   ID de la tarea a cancelar (0 = cancelar): ");
	if (idTarea == 0) {
		cout << "\n Operacion cancelada.\n";
		return;
	}
	string mensaje;
	bool exito = cluster.eliminarTarea(idTarea, mensaje);
	mostrarResultado(exito, mensaje);
}

void Sistema::verColaDeServidor() {
	cout << "Ver la cola de tareas de un servidor" << endl;
	if (cluster.primero == NULL) {
		mostrarResultado(false, "No hay servidores registrados.");
		return;
	}
	cluster.mostrarEstado();
	int id = pedirIdServidorExistente("\n   ID del servidor (0 = cancelar): ");
	if (id == 0) {
		return;
	}
	cluster.mostrarCola(id);
}
