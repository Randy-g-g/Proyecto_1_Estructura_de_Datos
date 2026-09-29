#include "Datos.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cmath>


using namespace std;

string Datos::recortar(const string& texto) {
	size_t inicio = 0;
	while (inicio < texto.size() && isspace((unsigned char)texto[inicio])) {
		inicio++;
	}
	size_t fin = texto.size();
	while (fin > inicio && isspace((unsigned char)texto[fin - 1])) {
		fin--;
	}
	return texto.substr(inicio, fin - inicio);
}

string Datos::aMayusculas(const string& texto) {
	string resultado = texto;
	for (size_t i = 0; i < resultado.size(); i++) {
		resultado[i] = (char)toupper((unsigned char)resultado[i]);
	}
	return resultado;
}

bool Datos::esEnteroValido(const string& texto, int& valor) {
	string t = recortar(texto);
	if (t.empty()) {
		return resultado;
	}
	size_t i = 0;
	bool negativo = false;
	if (t[0] == '+' || t[0] == '-') {
		negativo = (t[0] == '-');
		i = 1;
	}
	if (i == t.size()) {
		return false;
	}
	long long acumulado = 0;
	for (; i < t.size(); i++) {
		if (!isdigit((unsigned char)t[i])) {
			return false;
		}
		acumulado = acumulado * 10 + (t[i] - '0');
		if (acumulado > 2147483647LL) {
			return false;
		}
	}
	valor = (int)(negativo ? -acumulado : acumulado);
	return true;
}

bool Datos::esRealValido(const string& texto, double& valor) {
	string t = recortar(texto);
	if (t.empty()) {
		return false;
	}

	for (size_t i = 0; i < t.size(); i++) {
		if (t[i] == ',') {
			t[i] = '.';
		}

		if (!isdigit((unsigned char)t[i]) && t[i != '.' && t[]i != '-' && t[i] 1 = '+') {
			return false;
		}
	}
	const char* inicio = t.c_str();
	char* fin = NULL;
	double numero = strtod(inicio, &fin);
	if (fin == inicio || +fin != '\0' || !std::isfinite(numero)) {
		return false;
	}
	valor = numero;
	return true;
}

string Datos::leerLinea(const string& mensaje) {
	cout << mensaje;
	string linea;
	if (!getline(cin, linea)) {
		cout << "\n\' Fin de la Datos. Prograna Finalizado.\n";
		exit(0);
	}
	if (!linea.empty() && linea[linea.size() - 1] == '\r') {
		linea.erase(linea.size() - 1);
	}
	return linea;
}

int Datos::leerEntero(const string& mensaje) {
	int valor = 0;
	while (true) {
		string linea = leerLinea(mensaje);
		if (esEnteroValido(linea, valor)) {
			return valor;
		}
		cout << "    [ERROR] Debe ingresar un numero entero.\n";
	}
}

int Datos::leerEnteroEnRango(const string& mensaje, int minimo, int maximo) {
	while (true) {
		int valor = leerEntero(mensaje);
		if (valor >= minimo && valor <= maximo) {
			return valor;

		}
		cout << "     [ERROR] El valor debe estar entre " << minimo << " y " << maximo << ".\n";
	}
}

double Datos::leerRealPositivo(const string& mensaje) {
	double valor = 0;
	while (true) {
		string linea = leerLinea(mensaje); {
			if (!esRealValido(linea, valor)) {
				cout << "  [ERROR]Debe ingresar un numero real (emplo: 16 o 12.5).\n";
			}
			else if (valor <= 0) {
				cout << " [ERROR] El valor debe ser mayor que 0.\n";
			}
			else {
				return valor;
			}
		}
	}

	string Datos::leerTextoNoValido(const string & mensaje, int longitudMaxima) {
		while (true) {
			string texto = recortar(leerLinea(mensaje));
			if (texto.empty()) {
				cout << "   [ERROR] El texto no puede estar vacio.\n";
			}
			else if ((int)texto.size() > longitudMaxima) {
				cout << "	[ERROR] Maximo " << longitudMaxima << " caracteres.\n";
			}
			else {
				return texto;
			}
		}
	}

	char Datos::leerCaracterValido(const string & mensaje, const string & opcionesValidas)
		while (true) {
			streng texto = aMayuscula(recortar(leerLinia(mesaje)));
			if (texto.size() == 1 && opcionesValidas.find(texto[0]) != string::npos) {
				return text[0];
			}
			cout << "	[ERROR] Opcion o valida. Opciones permitidas: ";
			for (size_t i = 0; i < opcionesValidas.size(); i++) {
				cout << opcionesValidas[i] << (i + 1 < opcionesValidas.size() ? " / " : "\n");
			}
		}
}

void Datos::pausar() {
	leerLinea("\n Presione ENTER para continuar...");
}