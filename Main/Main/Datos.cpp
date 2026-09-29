#include "Datos.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <climits>
#include <cmath>
using namespace std;

string Datos::recortar(const string& texto) {
    size_t inicio = 0, fin = texto.size();
    while (inicio < fin && isspace((unsigned char)texto[inicio])) inicio++;
    while (fin > inicio && isspace((unsigned char)texto[fin - 1])) fin--;
    return texto.substr(inicio, fin - inicio);
}
string Datos::aMayusculas(const string& texto) {
    string copia = texto;
    for (size_t i = 0; i < copia.size(); i++) copia[i] = (char)toupper((unsigned char)copia[i]);
    return copia;
}
bool Datos::esEnteroValido(const string& texto, int& valor) {
    string t = recortar(texto);
    if (t.empty()) 
        return false;
    size_t i = (t[0] == '+' || t[0] == '-') ? 1 : 0;
    if (i == t.size()) 
        return false;
    long long n = 0;
    long long limite = (t[0] == '-') ? (long long)INT_MAX + 1 : INT_MAX;
    for (; i < t.size(); i++) {
        if (!isdigit((unsigned char)t[i])) 
            return false;
        int digito = t[i] - '0';
        if (n > (limite - digito) / 10) 
            return false;
        n = n * 10 + digito;
    }
    valor = (int)((t[0] == '-') ? -n : n);
    return true;
}
bool Datos::esRealValido(const string& texto, double& valor) {
    string t = recortar(texto);
    if (t.empty()) return false;
    for (size_t i = 0; i < t.size(); i++) if (t[i] == ',') t[i] = '.';
    char* fin = NULL;
    double n = strtod(t.c_str(), &fin);
    if (fin == t.c_str() || *fin != '\0' || !isfinite(n)) return false;
    valor = n;
    return true;
}
string Datos::leerLinea(const string& mensaje) {
    cout << mensaje;
    string linea;
    if (!getline(cin, linea)) exit(0);
    return linea;
}
int Datos::leerEntero(const string& mensaje) {
    int valor;
    while (true) {
        if (esEnteroValido(leerLinea(mensaje), valor)) return valor;
        cout << "Debe ingresar un numero entero.\n";
    }
}
int Datos::leerEnteroEnRango(const string& mensaje, int minimo, int maximo) {
    while (true) {
        int valor = leerEntero(mensaje);
        if (valor >= minimo && valor <= maximo) return valor;
        cout << "El valor debe estar entre " << minimo << " y " << maximo << ".\n";
    }
}
double Datos::leerRealPositivo(const string& mensaje) {
    double valor;
    while (true) {
        if (esRealValido(leerLinea(mensaje), valor) && valor > 0) return valor;
        cout << "Ingrese un numero mayor que 0.\n";
    }
}
string Datos::leerTextoNoVacio(const string& mensaje, int longitudMaxima) {
    while (true) {
        string texto = recortar(leerLinea(mensaje));
        if (!texto.empty() && (int)texto.size() <= longitudMaxima) return texto;
        cout << "Texto obligatorio, maximo " << longitudMaxima << " caracteres.\n";
    }
}
char Datos::leerCaracterValido(const string& mensaje, const string& opcionesValidas) {
    while (true) {
        string texto = aMayusculas(recortar(leerLinea(mensaje)));
        if (texto.size() == 1 && opcionesValidas.find(texto[0]) != string::npos) return texto[0];
        cout << "Opciones validas: " << opcionesValidas <<endl;
    }
}
void Datos::pausar() { leerLinea("\n Presione ENTER para continuar..."); }
