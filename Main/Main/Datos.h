#pragma once
#ifndef Datos_H
#define Datos_H
#include <string>

using namespace std;

class Datos {
public:

	static string recortar(const string& texto);
	static string aMayusculas(const string& texto);
	static bool esEnteroValido(const string& texto, int& valor);
	static bool esRealValido(const string& texto, double& valor);
	static string leerLinea(const string& mesaje);
	static int leerEntero(const string& mensaje);
	static int leerEnteroEnRango(const string& mensaje, int minimo, int maximo);
	static double leerRealPositivo(const string& mensaje);
	static string leerTextoNoVacio(const string& mensaje, int longitudMaxima);
	static char leerCaracterValido(const string& mensaje, const string& opcionesValidas);
	static void pausar();

};

#endif