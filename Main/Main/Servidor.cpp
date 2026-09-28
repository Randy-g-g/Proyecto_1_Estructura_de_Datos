#include "Servidor.h"
#include <iostream>
#include <cctype>

using namespace std;

Servidor::Servidor(int idServidor, const string& nombreServidor, const string& ipServidor,
    Arquitectura tipoArquitectura)
    : id(idServidor), nombre(nombreServidor), ip(ipServidor), arquitectura(tipoArquitectura),
    colas(new Colas()), siguiente(NULL), atras(NULL) {}

Servidor::~Servidor() {
    delete colas;
    colas = NULL;
}//destructor para liberar memoria de la cola de tareas

int Servidor::getId() const {
    return id;
}

string Servidor::getNombre() const {
    return nombre;
}

string Servidor::getIp() const {
    return ip;
}

Arquitectura Servidor::getArquitectura() const {
    return arquitectura;
}

string Servidor::getArquitecturaTexto() const {
    return arquitecturaATexto(arquitectura);
}

Colas* Servidor::getColas() const {
    return colas;
}

int Servidor::getTareasPendientes() const {
    return colas->getCantidad();
}

void Servidor::setNombre(const string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Servidor::setArquitectura(Arquitectura nuevaArquitectura) {
    arquitectura = nuevaArquitectura;
}

void Servidor::mostrarEncabezado() const {
    cout << "   Servidor          : [" << id << "] " << nombre << "\n";
    cout << "   Direccion IPv4    : " << ip << "\n";
    cout << "   Arquitectura      : " << getArquitecturaTexto() << "\n";
    cout << "   Tareas pendientes : " << getTareasPendientes() << "\n";
}

//  Validaciones

string Servidor::arquitecturaATexto(Arquitectura tipo) {
    switch (tipo) {
    case HIGH_PERFORMANCE: return "High-Performance";
    case STANDARD:         return "Standard";
    case MEMORY_OPTIMIZED: return "Memory-Optimized";
    }
    return "Desconocida";
}//convierte la arquitectura a texto

bool Servidor::esArquitecturaValida(int valor) {
    return valor == HIGH_PERFORMANCE || valor == STANDARD || valor == MEMORY_OPTIMIZED;
}//valida si la arquitectura es valida

bool Servidor::esNombreValido(const string& texto, string& motivo) {
    const string& nombre = texto;
    if (nombre.empty()) {
        motivo = "El nombre no puede estar vacio.";
        return false;
    }
    if ((int)nombre.size() > LONGITUD_MAX_NOMBRE) {
        motivo = "El nombre no puede superar 30 caracteres.";
        return false;
    }
    bool tieneLetra = false;
    for (size_t i = 0; i < nombre.size(); i++) {
        unsigned char c = (unsigned char)nombre[i];
        if (isalpha(c)) {
            tieneLetra = true;
        }
        else if (!isdigit(c) && c != ' ' && c != '-' && c != '_' && c != '.') {
            motivo = "El nombre solo admite letras, digitos, espacios, '-', '_' y '.' (sin tildes).";
            return false;
        }
    }//verifica que el nombre contenga al menos una letra
    if (!tieneLetra) {
        motivo = "El nombre debe contener al menos una letra.";
        return false;
    }
    return true;
}
//voy por aqui______________________________________________________________________________________________________________________________________________
// IP valida en formato estandar IPv4 (notacion decimal con puntos): A.B.C.D
//   - Exactamente 4 octetos separados por 3 puntos.
//   - Cada octeto: solo digitos, de 1 a 3 cifras, valor entre 0 y 255.
//   - Sin ceros a la izquierda ("010" se rechaza; "0" si es valido), porque
//     "10.0.0.1" y "10.0.0.01" serian la misma IP escrita de dos formas y
//     eso permitiria burlar la validacion de IP unica.
//   - Sin espacios, letras ni otros simbolos.
// Ejemplos validos: 192.168.1.10, 10.0.0.1, 0.0.0.0, 255.255.255.255
// Ejemplos invalidos: 256.1.1.1, 10.0.0, 10..0.1, 10.0.0.1., 192.168.01.1, abc
bool Servidor::esIpv4Valida(const string& direccion, string& motivo) {
    const string& ip = direccion;
    if (ip.empty()) {
        motivo = "La IP no puede estar vacia.";
        return false;
    }
    if ((int)ip.size() > LONGITUD_MAX_IP) {
        motivo = "La IP no puede superar 15 caracteres (formato IPv4: 255.255.255.255).";
        return false;
    }

    int octetos = 0;            // octetos completos leidos
    size_t i = 0;
    while (i < ip.size()) {
        // --- Leer un octeto ---
        size_t inicio = i;
        int valor = 0;
        while (i < ip.size() && isdigit((unsigned char)ip[i])) {
            valor = valor * 10 + (ip[i] - '0');
            i++;
            if (i - inicio > 3) {
                motivo = "Cada octeto de la IP debe tener entre 1 y 3 digitos.";
                return false;
            }
        }
        size_t digitos = i - inicio;
        if (digitos == 0) {
            motivo = "Formato IPv4 invalido: use 4 numeros separados por puntos (ej. 192.168.1.10).";
            return false;
        }
        if (digitos > 1 && ip[inicio] == '0') {
            motivo = "Los octetos no pueden tener ceros a la izquierda (ej. use 1 en vez de 01).";
            return false;
        }
        if (valor > 255) {
            motivo = "Cada octeto de la IP debe estar entre 0 y 255.";
            return false;
        }
        octetos++;

        // --- Despues de un octeto solo puede venir un punto o el final ---
        if (i < ip.size()) {
            if (ip[i] != '.') {
                motivo = "La IP solo puede contener digitos y puntos (ej. 192.168.1.10).";
                return false;
            }
            if (octetos == 4) {
                motivo = "La IP debe tener exactamente 4 octetos.";
                return false;
            }
            i++;                                        // salta el punto
            if (i == ip.size()) {
                motivo = "La IP no puede terminar en punto.";
                return false;
            }
        }
    }
    if (octetos != 4) {
        motivo = "La IP debe tener exactamente 4 octetos (ej. 192.168.1.10).";
        return false;
    }
    return true;
}
