#include "ServidoresCluster.h"
#include "Datos.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;


static string numeroATexto(double valor, int decimales) { //convierte numero a texto, genuinamente nose como funciona
    ostringstream salida;
    salida << fixed << setprecision(decimales) << valor;
    return salida.str();
}

ServidoresCluster::ServidoresCluster() : primero(NULL), cantidad(0), siguienteIdTarea(1) {
}

    ServidoresCluster::~ServidoresCluster() {
    if (primero == NULL) {
        return;
    }
    // lista doble enlazada circular cambia a doble enlazada para sacar un elemento e ir vaciando la lista 
    primero->atras->siguiente = NULL;
    Servidor* actual = primero;
    while (actual != NULL) {
        Servidor* aux = actual;
        actual = actual->siguiente;
        delete aux
    }
    primero = NULL;
    cantidad = 0;
}

//Consultas para facilitar preguntas en algunas funciones
bool ServidoresCluster::estaVacio() const {
    return primero == NULL;
}

bool ServidoresCluster::estaLleno() const {
    return cantidad >= (idMax - idMin + 1);
}

int ServidoresCluster::getCantidad() const {
    return cantidad;
}

int ServidoresCluster::getTotalTareasPendientes() const {
    if (primero == NULL) {
        return 0;
    }
    int total = 0;
    Servidor* actual = primero;
    do {
        total += actual->getTareasPendientes();
        actual = actual->siguiente;
    } while (actual != primero);
    return total;
}
    //Recorrido circular termina al volver al primero
Servidor* ServidoresCluster::buscarPorId(int id) const {
    if (primero == NULL) {
        return NULL;
    }
    Servidor* actual = primero;
    do {                                
        if (actual->id == id) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != primero);
    return NULL;
}
    //Busca y compara las ip
Servidor* ServidoresCluster::buscarPorIp(const string& ip) const {
    if (primero == NULL) {
        return NULL;
    }
    Servidor* actual = primero;
    do {
        if (actual->ip == ip) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != primero);
    return NULL;
}
    //Retorna el servidor donde se encuntra la tarea que se desea buscar
Servidor* ServidoresCluster::buscarServidorDeTarea(int idTarea) const {
    if (primero == NULL) {
        return NULL;
    }
    Servidor* actual = primero;
    do {
        if (actual->colaTareas->contiene(idTarea)) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != primero);
    return NULL;
}

    //Recorre desde el inicio, busca el servidor con menos carga
Servidor* ServidoresCluster::buscarMenosCarga (bool soloHighPerformance) const {
    if (primero == NULL) {
        return NULL;
    }
    Servidor* mejor = NULL;
    Servidor* actual = primero;
    do {
        if (!soloHighPerformance || actual->arquitectura == HIGH_PERFORMANCE) { // si ambos servidores son iwales
            if (mejor == NULL || actual->getTareasPendientes() < mejor->getTareasPendientes()) {
                mejor = actual; //devuevle el de menor id
            }
        }
        actual = actual->siguiente;
    } while (actual != primero);
    return mejor;
}

//  MODULO 1: SERVIDORES

// Registrar Servidor: valida ID en [1,8] y no repetido, IP unica, nombre y
// arquitectura validos. Inserta en el anillo manteniendo el orden por ID.
bool ServidoresCluster::registrarServidor(int id, const string& nombre, const string& ip,
    int arquitectura, string& mensaje) {
    string nombreLimpio = Datos::recortar(nombre);
    string ipLimpia = Datos::recortar(ip);
    string motivo;

    if (estaLleno()) {
        mensaje = "El cluster ya tiene los 8 servidores permitidos (IDs 1 a 8).";
        return false;
    }
    if (id < idMin || id > idMax) {
        mensaje = "El ID debe estar en el rango [1, 8].";
        return false;
    }
    if (buscarPorId(id) != NULL) {
        mensaje = "Ya existe un servidor con el ID " + numeroATexto(id, 0) + ".";
        return false;
    }
    if (!Servidor::esIpv4Valida(ipLimpia, motivo)) {
        mensaje = motivo;
        return false;
    }
    if (buscarPorIp(ipLimpia) != NULL) {
        mensaje = "La IP '" + ipLimpia + "' ya pertenece a otro servidor.";
        return false;
    }
    if (!Servidor::esNombreValido(nombreLimpio, motivo)) {
        mensaje = motivo;
        return false;
    }
    if (!Servidor::esArquitecturaValida(arquitectura)) {
        mensaje = "Tipo de arquitectura no valido.";
        return false;
    }

    Servidor* nuevo = new Servidor(id, nombreLimpio, ipLimpia, (Arquitectura)arquitectura);

    if (primero == NULL) {
        // Anillo vacio: el nodo se apunta a si mismo en ambas direcciones
        primero = nuevo;
        nuevo->siguiente = nuevo;
        nuevo->atras = nuevo;
    }
    else {
        // Se busca el primer servidor con ID mayor; el nuevo va antes de el.
        // Si no existe, "actual" vuelve a "primero" y el nuevo queda al final.
        Servidor* actual = primero;
        bool hayMayor = false;
        do {
            if (actual->id > id) {
                hayMayor = true;
                break;
            }
            actual = actual->siguiente;
        } while (actual != primero);

        Servidor* anterior = actual->atras;
        anterior->siguiente = nuevo;
        nuevo->atras = anterior;
        nuevo->siguiente = actual;
        actual->atras = nuevo;

        if (hayMayor && actual == primero) {
            primero = nuevo;            // el nuevo tiene el menor ID
        }
    }
    cantidad++;
    mensaje = "Servidor [" + numeroATexto(id, 0) + "] " + nombreLimpio + " registrado correctamente.";
    return true;
}

// Mostrar Estado del Cluster recorrido circular desde "primero" hasta volver a el
void ServidoresCluster::mostrarEstado() const {
    cout << "Estado del Clsuter";
    if (primero == NULL) {
        cout << "   No hay servidores registrados en el cluster.\n";
        cout << "   Registre al menos un servidor desde el menu de Servidores.\n";
        return;
    }

    cout << "   " << left
        << setw(5) << "ID"
        << setw(20) << "Nombre"
        << setw(20) << "Direccion IPv4"
        << setw(19) << "Arquitectura"
        << "Pendientes\n";
    cout << "   " << string(72, '-') << "\n";

    Servidor* actual = primero;
    do {
        cout << "   " << left
            << setw(5) << actual->id
            << setw(20) << actual->nombre.substr(0, 19)
            << setw(20) << actual->ip.substr(0, 19)
            << setw(19) << actual->getArquitecturaTexto()
            << actual->getTareasPendientes() << "\n";
        actual = actual->siguiente;
    } while (actual != primero);

    cout << "   " << string(72, '-') << "\n";
    cout << "   Servidores registrados: " << cantidad << " / " << idMax << "\n";
    cout << "   Total de tareas pendientes en el cluster: " << getTotalTareasPendientes() << "\n";

    // Evidencia del anillo se muestra el recorrido y el regreso al inicio
    cout << "   Recorrido circular: ";
    actual = primero;
    do {
        cout << "[" << actual->id << "] <-> ";
        actual = actual->siguiente;
    } while (actual != primero);
    cout << "(regresa a [" << primero->id << "])\n";
}

bool ServidoresCluster::registrarServidor(int id, const string& nuevoNombre, string& mensaje) {
    Servidor* servidor = buscarPorId(id);
    if (servidor == NULL) {
        mensaje = "No existe un servidor con el ID " + numeroATexto(id, 0) + ".";
        return false;
    }
    string nombreLimpio = Datos::recortar(nuevoNombre);
    string motivo;
    if (!Servidor::esNombreValido(nombreLimpio, motivo)) {
        mensaje = motivo;
        return false;
    }
    string anterior = servidor->nombre;
    servidor->setNombre(nombreLimpio);
    mensaje = "Nombre modificado: '" + anterior + "' -> '" + nombreLimpio + "'.";
    return true;
}

bool ServidoresCluster::actualizarArquitectura(int id, int nuevaArquitectura, string& mensaje) {
    Servidor* servidor = buscarPorId(id);
    if (servidor == NULL) {
        mensaje = "No existe un servidor con el ID " + numeroATexto(id, 0) + ".";
        return false;
    }
    if (!Servidor::esArquitecturaValida(nuevaArquitectura)) {
        mensaje = "Tipo de arquitectura no valido.";
        return false;
    }
    if (servidor->arquitectura == (Arquitectura)nuevaArquitectura) {
        mensaje = "El servidor ya tiene la arquitectura " + servidor->getArquitecturaTexto() + ".";
        return false;
    }
    string anterior = servidor->getArquitecturaTexto();
    servidor->setArquitectura((Arquitectura)nuevaArquitectura);
    mensaje = "Arquitectura modificada: " + anterior + " -> " + servidor->getArquitecturaTexto() + ".";
    return true;
}

// Dar de Baja: no se permite si el servidor aun tiene tareas pendientes
bool ServidoresCluster::eliminarServidor(int id, string& mensaje) {
    Servidor* Servidor = buscarPorId(id);
    if (Servidor == NULL) {
        mensaje = "No existe un servidor con el ID " + numeroATexto(id, 0) + ".";
        return false;
    }
    if (Servidor->getTareasPendientes() > 0) {
        mensaje = "No se puede dar de baja a '" + Servidor->nombre + "': aun tiene "
            + numeroATexto(Servidor->getTareasPendientes(), 0)
            + " tarea(s) pendiente(s). Ejecutelas o cancelelas primero.";
        return false;
    }

    if (Servidor->siguiente == Servidor) {
        primero = NULL;                                 // era el unico servidor
    }
    else {
        servidor->atras->siguiente = servidor->siguiente;   // se "puentea" el nodo
        Servidor->siguiente->atras = servidor->atras;
        if (servidor == primero) {
            primero = servidor->siguiente;
        }
    }
    string nombre = servidor->nombre;
    delete servidor;
    cantidad--;
    mensaje = "Servidor [" + numeroATexto(id, 0) + "] " + nombre + " dado de baja correctamente.";
    return true;
}

// ===========================================================================
//  MODULO 2: TAREAS
// ===========================================================================

// Encolar Tarea (Asignar Proceso):
//  - Mas de 32 GB o prioridad Critica -> servidor High-Performance con menos tareas.
//    Si no hay ningun High-Performance registrado, se asigna al servidor con
//    menos tareas del cluster y se advierte al usuario (caso no definido en el enunciado).
//  - Normal -> servidor con menos tareas del cluster.
bool ServidoresCluster::encolarTarea(double memoriaGB, char prioridad, const string& nombreProceso,
    string& mensaje) {
    string nombreLimpio = Datos::recortar(nombreProceso);

    if (primero == NULL) {
        mensaje = "No hay servidores registrados. Registre un servidor antes de asignar tareas.";
        return false;
    }
    if (!(memoriaGB > 0)) {
        mensaje = "El consumo de memoria debe ser mayor que 0.";
        return false;
    }
    if (nombreLimpio.empty()) {
        mensaje = "El nombre del proceso no puede estar vacio.";
        return false;
    }
    if (!Tarea::esPrioridadValida(prioridad)) {
        mensaje = "La prioridad debe ser N (Normal) o C (Critica).";
        return false;
    }

    Tarea referencia(0, memoriaGB, prioridad, nombreLimpio);
    Servidor* destino = NULL;
    string motivo;

    if (referencia.requiereAltoRendimiento()) {
        destino = buscarMenosCargado(true);
        if (destino != NULL) {
            motivo = (prioridad == 'C')
                ? "tarea Critica -> High-Performance con menos tareas"
                : "requiere mas de 32 GB -> High-Performance con menos tareas";
        }
        else {
            destino = buscarMenosCargado(false);
            motivo = "ADVERTENCIA: no hay servidores High-Performance; "
                "se asigno al servidor con menos tareas del cluster";
        }
    }
    else {
        destino = buscarMenosCarga(false);
        motivo = "tarea Normal -> servidor con menos tareas en cola";
    }

    int idTarea = siguienteIdTarea;
    siguienteIdTarea++;
    destino->colaTareas->encolar(idTarea, memoriaGB, prioridad, nombreLimpio);

    mensaje = "Tarea #" + numeroATexto(idTarea, 0) + " (" + nombreLimpio + ") asignada a ["
        + numeroATexto(destino->id, 0) + "] " + destino->nombre + " - "
        + destino->getArquitecturaTexto() + ".\n   Criterio: " + motivo
        + ".\n   Tareas en cola de ese servidor: "
        + numeroATexto(destino->getTareasPendientes(), 0) + ".";
    return true;
}

// Desencolar Tarea (Ejecutar Proceso): muestra el estado antes y despues
bool ServidoresCluster::ejecutarTarea(int idServidor, string& mensaje) {
    Servidor* Servidor = buscarPorId(idServidor);
    if (Servidor == NULL) {
        mensaje = "No existe un servidor con el ID " + numeroATexto(idServidor, 0) + ".";
        return false;
    }
    if (Servidor->colaTareas->estaVacia()) {
        mensaje = "El servidor '" + Servidor->nombre + "' no tiene tareas pendientes para ejecutar.";
        return false;
    }

    cout << "\n ESTADO ANTES DE DESENCOLAR \n";
    Servidor->mostrarEncabezado();
    Servidor->colaTareas->mostrar();

    Tarea ejecutada;
    Servidor->colaTareas->desencolar(ejecutada);

    cout << "\n TAREA EJECUTADA \n";
    ejecutada.mostrarDetalle();

    cout << "\n ESTADO DESPUES DE DESENCOLAR \n";
    Servidor->mostrarEncabezado();
    Servidor->colaTareas->mostrar();

    mensaje = "Tarea #" + numeroATexto(ejecutada.getId(), 0) + " ejecutada en '"
        + Servidor->nombre + "'. Tareas restantes: "
        + numeroATexto(Servidor->getTareasPendientes(), 0) + ".";
    return true;
}


bool ServidoresCluster::eliminarTarea(int idTarea, string& mensaje) {
    if (primero == NULL) {
        mensaje = "No hay servidores registrados.";
        return false;
    }
    if (idTarea <= 0) {
        mensaje = "El ID de tarea debe ser un entero positivo.";
        return false;
    }
    Servidor* servidor = buscarServidorDeTarea(idTarea);
    if (servidor == NULL) {
        mensaje = "No existe ninguna tarea pendiente con el ID " + numeroATexto(idTarea, 0) + ".";
        return false;
    }

    cout << "\n COLA ANTES DE CANCELAR \n";
    servidor->mostrarEncabezado();
    servidor->colaTareas->mostrar();

    Tarea cancelada;
    servidor->colaTareas->cancelar(idTarea, cancelada);

    cout << "\n TAREA CANCELADA \n";
    cancelada.mostrarDetalle();

    cout << "\n COLA DESPUES DE CANCELAR \n";
    Servidor->mostrarEncabezado();
    Servidor->colaTareas->mostrar();

    mensaje = "Tarea #" + numeroATexto(idTarea, 0) + " cancelada en '" + Servidor->nombre
        + "'. El orden FIFO de las tareas restantes se mantuvo.";
    return true;
}

bool ServidoresCluster::mostrarCola(int idServidor) const {
    Servidor* servidor = buscarPorId(idServidor);
    if (servidor == NULL) {
        return false;
    }
    cout << "\n COLA DE TAREAS \n";
    servidor->mostrarEncabezado();
    servidor->colaTareas->mostrar();
    return true;
}
